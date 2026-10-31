#include "global.h"
#include "rtc.h"

// RTC initialization and time conversion shared with Ruby/Sapphire.

// Static bss in the original. A payload build places it at its original
// address itself (SDK_EXTERN_BSS), so there it is referred to by name.
// sSavedIme in pret, but agb_flash.c has its own.
#ifdef SDK_EXTERN_BSS
extern u16 sErrorStatus;
extern struct SiiRtcInfo sRtc;
extern u8 sProbeResult;
extern u16 sRtcSavedIme;
#else
static u16 sErrorStatus;
static struct SiiRtcInfo sRtc;
static u8 sProbeResult;
static u16 sRtcSavedIme;
#endif

static u16 RtcCheckInfo(struct SiiRtcInfo *rtc);

static const struct SiiRtcInfo sRtcDummy = {0, MONTH_JAN, 1}; // 2000 Jan 1

// pokechan's build defines these before sNumDaysInMonths, which moves the
// table after sRtcDummy in .rodata.
#ifdef PROGRAM_POKECHAN
void RtcDisableInterrupts()
{
    sRtcSavedIme = REG_IME;
    REG_IME = 0;
}

void RtcRestoreInterrupts()
{
    REG_IME = sRtcSavedIme;
}
#endif

// Not static, because the Berry Program Update's rtc.c uses it too.
const s32 sNumDaysInMonths[12] =
{
    31,
    28,
    31,
    30,
    31,
    30,
    31,
    31,
    30,
    31,
    30,
    31,
};

#ifndef PROGRAM_POKECHAN
void RtcDisableInterrupts()
{
    sRtcSavedIme = REG_IME;
    REG_IME = 0;
}

void RtcRestoreInterrupts()
{
    REG_IME = sRtcSavedIme;
}
#endif

s32 ConvertBcdToBinary(u8 bcd)
{
    if (bcd > 0x9F || (bcd & 0xF) > 0x9)
        return 0xFF;

    return 10 * ((bcd >> 4) & 0xF) + (bcd & 0xF);
}

bool8 IsLeapYear(u8 year)
{
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
        return TRUE;
    else
        return FALSE;
}

// Same as Ruby/Sapphire's, but with dayCount declared before i. RS has them
// the other way round, which puts dayCount at [r7, #8] and costs an instruction per
// access.
//
// The loop runs while i > 0, so the year 2000 contributes no days. That is
// the berry glitch itself, left in. This program carries the fix in
// src/berry_fix*.c and does not correct the cartridge's own arithmetic.
static u16 ConvertDateToDayCount(u8 year, u8 month, u8 day)
{
    u16 dayCount = 0;
    s32 i;

    for (i = year - 1; i > 0; i--)
    {
        dayCount += 365;

        if (IsLeapYear(i) == TRUE)
            dayCount++;
    }

    for (i = 0; i < month - 1; i++)
        dayCount += sNumDaysInMonths[i];

    // "+= 1" here and "++" in the loop above. The two compile differently at
    // -O0 and only this pairing gives the target's instruction counts.
    if (month > MONTH_FEB && IsLeapYear(year) == TRUE)
        dayCount += 1;

    dayCount += day;

    return dayCount;
}
// RS names three locals here; this build passes the conversions straight
// through, so nothing is spilled to the stack.
u16 RtcGetDayCount(struct SiiRtcInfo *rtc)
{
    return ConvertDateToDayCount(ConvertBcdToBinary(rtc->year),
                                 ConvertBcdToBinary(rtc->month),
                                 ConvertBcdToBinary(rtc->day));
}

void RtcInit()
{
    sErrorStatus = 0;

    RtcDisableInterrupts();
    SiiRtcUnprotect();
    sProbeResult = SiiRtcProbe();
    RtcRestoreInterrupts();

    // sample0519's older build only counts a probe result of 0 as a failure;
    // the later ones (pokeruby's too) require it to be 1.
#ifdef PROGRAM_SAMPLE0519
    if ((sProbeResult & 0xF) == 0)
#else
    if ((sProbeResult & 0xF) != 1)
#endif
    {
        sErrorStatus = RTC_INIT_ERROR;
        return;
    }

    if (sProbeResult & 0xF0)
        sErrorStatus = RTC_INIT_WARNING;
    else
        sErrorStatus = 0;

    RtcGetRawInfo(&sRtc);
    sErrorStatus = RtcCheckInfo(&sRtc);
}

u16 RtcGetErrorStatus()
{
    return sErrorStatus;
}

void RtcGetInfo(struct SiiRtcInfo *rtc)
{
    if (sErrorStatus & RTC_ERR_FLAG_MASK)
        *rtc = sRtcDummy;
    else
        RtcGetRawInfo(rtc);
}

void RtcGetDateTime(struct SiiRtcInfo *rtc)
{
    RtcDisableInterrupts();
    SiiRtcGetDateTime(rtc);
    RtcRestoreInterrupts();
}

void RtcGetStatus(struct SiiRtcInfo *rtc)
{
    RtcDisableInterrupts();
    SiiRtcGetStatus(rtc);
    RtcRestoreInterrupts();
}

void RtcGetRawInfo(struct SiiRtcInfo *rtc)
{
    RtcGetStatus(rtc);
    RtcGetDateTime(rtc);
}

static u16 RtcCheckInfo(struct SiiRtcInfo *rtc)
{
    u16 errorFlags = 0;
    s32 year;
    s32 month;
    s32 value;

    if (rtc->status & SIIRTCINFO_POWER)
        errorFlags |= RTC_ERR_POWER_FAILURE;

    if (!(rtc->status & SIIRTCINFO_24HOUR))
        errorFlags |= RTC_ERR_12HOUR_CLOCK;

    year = ConvertBcdToBinary(rtc->year);
    if (year == 0xFF)
        errorFlags |= RTC_ERR_INVALID_YEAR;

    month = ConvertBcdToBinary(rtc->month);
    if (month == 0xFF || month == 0 || month > 12)
        errorFlags |= RTC_ERR_INVALID_MONTH;

    value = ConvertBcdToBinary(rtc->day);
    if (value == 0xFF)
        errorFlags |= RTC_ERR_INVALID_DAY;

    if (month == MONTH_FEB)
    {
        if (value > IsLeapYear(year) + sNumDaysInMonths[1])
            errorFlags |= RTC_ERR_INVALID_DAY;
    }
    else
    {
        if (value > sNumDaysInMonths[month - 1])
            errorFlags |= RTC_ERR_INVALID_DAY;
    }

    value = ConvertBcdToBinary(rtc->hour);
    if (value == 0xFF || value > 24)
        errorFlags |= RTC_ERR_INVALID_HOUR;

    value = ConvertBcdToBinary(rtc->minute);
    if (value == 0xFF || value > 60)
        errorFlags |= RTC_ERR_INVALID_MINUTE;

    value = ConvertBcdToBinary(rtc->second);
    if (value == 0xFF || value > 60)
        errorFlags |= RTC_ERR_INVALID_SECOND;

    return errorFlags;
}

void UNUSED RtcReset()
{
    RtcDisableInterrupts();
    SiiRtcReset();
    RtcRestoreInterrupts();
}

void RtcCalcTimeDifference(struct SiiRtcInfo *rtc, struct Time *result, struct Time *t)
{
    u16 days = RtcGetDayCount(rtc);
    result->seconds = ConvertBcdToBinary(rtc->second) - t->seconds;
    result->minutes = ConvertBcdToBinary(rtc->minute) - t->minutes;
    result->hours = ConvertBcdToBinary(rtc->hour) - t->hours;
    result->days = days - t->days;

    if (result->seconds < 0)
    {
        result->seconds += 60;
        --result->minutes;
    }

    if (result->minutes < 0)
    {
        result->minutes += 60;
        --result->hours;
    }

    if (result->hours < 0)
    {
        result->hours += 24;
        --result->days;
    }
}

#ifdef PROGRAM_SAMPLE0519
void UNUSED RtcCalcLocalTime(void)
{
    RtcGetInfo(&sRtc);
    RtcCalcTimeDifference(&sRtc, &gLocalTime, &gLocalTimeOffset);
}

void UNUSED RtcInitLocalTimeOffset(s32 hour, s32 minute)
{
    RtcCalcLocalTimeOffset(0, hour, minute, 0);
}
#endif

void CalcTimeDifference(struct Time *result, struct Time *t1, struct Time *t2)
{
    result->seconds = t2->seconds - t1->seconds;
    result->minutes = t2->minutes - t1->minutes;
    result->hours = t2->hours - t1->hours;
    result->days = t2->days - t1->days;

    if (result->seconds < 0)
    {
        result->seconds += 60;
        --result->minutes;
    }

    if (result->minutes < 0)
    {
        result->minutes += 60;
        --result->hours;
    }

    if (result->hours < 0)
    {
        result->hours += 24;
        --result->days;
    }
}

#ifdef PROGRAM_SAMPLE0519
u32 RtcGetMinuteCount(void)
{
    RtcGetInfo(&sRtc);
    return (24 * 60) * RtcGetDayCount(&sRtc) + 60 * sRtc.hour + sRtc.minute;
}

void RtcCalcLocalTimeOffset(s32 days, s32 hours, s32 minutes, s32 seconds)
{
    gLocalTime.days = days;
    gLocalTime.hours = hours;
    gLocalTime.minutes = minutes;
    gLocalTime.seconds = seconds;
    RtcGetInfo(&sRtc);
    RtcCalcTimeDifference(&sRtc, &gLocalTimeOffset, &gLocalTime);
}
#endif
