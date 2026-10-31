#include "global.h"
#include "berry_fix.h"
#include "rtc.h"
#ifdef PROGRAM_POKECHAN
#include "target_game.h"
#endif

// Detects the berry clock glitch and advances the clock past the affected
// date. pret/berry-fix's payload/src/rtc.c.

// pret/berry-fix's static bss. A payload build places it at its original
// address itself (SDK_EXTERN_BSS), so there it is referred to by name.
#ifdef SDK_EXTERN_BSS
extern struct SiiRtcInfo sRtcInfoWork;
#else
static struct SiiRtcInfo sRtcInfoWork;
#endif

bool32 BerryFix_TryInitRtc(void)
{
    RtcInit();
    if (RtcGetErrorStatus() & RTC_ERR_FLAG_MASK)
        return FALSE;
    else
        return TRUE;
}

static void RtcSetDateTime(struct SiiRtcInfo *rtc)
{
    vu16 imeBak = REG_IME;
    REG_IME = 0;
    SiiRtcSetDateTime(rtc);
    REG_IME = imeBak;
}

// Ruby/Sapphire's SaveBlock2.localTimeOffset and .lastBerryTreeUpdate.
// pret/berry-fix hardcodes the addresses; the cartridge's save block is not
// at a fixed address here, so they come off gSaveBlock2Ptr.
#define LocalTimeOffset     ((struct Time *)(gSaveBlock2Ptr + 0x98))
#define LastBerryTreeUpdate ((struct Time *)(gSaveBlock2Ptr + 0xA0))

bool32 BerryFix_CalcTimeDifference(u8 *year)
{
    s32 minutes;

    RtcGetRawInfo(&sRtcInfoWork);
    *year = ConvertBcdToBinary(sRtcInfoWork.year);
    RtcCalcTimeDifference(&sRtcInfoWork, &gRtcUTCTime, LocalTimeOffset);
    CalcTimeDifference(&gTimeSinceBerryUpdate, LastBerryTreeUpdate, &gRtcUTCTime);
    minutes = gTimeSinceBerryUpdate.days * 24 * 60
            + gTimeSinceBerryUpdate.hours * 60
            + gTimeSinceBerryUpdate.minutes;
    if (minutes >= 0)
        return TRUE;
    else
        return FALSE;
}

static u32 ConvertBinaryToBcd(u8 binary)
{
    u32 bcd = 0;

    if (binary > 99)
        return 0xFF;

    bcd = Div(binary, 10) << 4;
    bcd |= Mod(binary, 10);
    return bcd;
}

static void RtcIncrement(u8 *val)
{
    *val = ConvertBinaryToBcd(ConvertBcdToBinary(*val) + 1);
}

static void RtcIncrementMonth(struct SiiRtcInfo *rtc)
{
    RtcIncrement(&rtc->month);
    if (ConvertBcdToBinary(rtc->month) <= 12)
        return;

    RtcIncrement(&rtc->year);
    rtc->month = MONTH_JAN;
}

static void RtcIncrementDay(struct SiiRtcInfo *rtc)
{
    RtcIncrement(&rtc->day);
    if (ConvertBcdToBinary(rtc->day) <= sNumDaysInMonths[ConvertBcdToBinary(rtc->month) - 1])
        return;

    if (!IsLeapYear(ConvertBcdToBinary(rtc->year)) || ConvertBcdToBinary(rtc->month) != MONTH_FEB || ConvertBcdToBinary(rtc->day) != 29)
    {
        rtc->day = 1;
        RtcIncrementMonth(rtc);
    }
}

// When fixing the RTC, consider if the leap day on Feb 29, 2000 was reached
static bool32 RtcNeedsLeapDayIncrement(struct SiiRtcInfo *rtc)
{
    if (ConvertBcdToBinary(rtc->year) == 0)
    {
        if (ConvertBcdToBinary(rtc->month) == MONTH_JAN)
            return FALSE;
        if (ConvertBcdToBinary(rtc->month) > MONTH_FEB)
            return TRUE;
        if (ConvertBcdToBinary(rtc->day) == 29)
            return TRUE;
        return FALSE;
    }
    if (ConvertBcdToBinary(rtc->year) == 1)
        return TRUE;

    // After 2001 (shouldn't occur)
    return FALSE;
}

void BerryFix_TryFixDate(void)
{
    RtcGetRawInfo(&sRtcInfoWork);

    // If the year is anything but 2000 or 2001 then the Berry Glitch has already passed
    if (ConvertBcdToBinary(sRtcInfoWork.year) == 0 || ConvertBcdToBinary(sRtcInfoWork.year) == 1)
    {
        if (ConvertBcdToBinary(sRtcInfoWork.year) == 1)
        {
            // Year is 2001, the Berry Glitch is occurring
            // Set date forward to January 2, 2002
            sRtcInfoWork.year = 2;
            sRtcInfoWork.month = MONTH_JAN;
            sRtcInfoWork.day = 2;
            RtcSetDateTime(&sRtcInfoWork);
        }
        else
        {
            // Year is 2000, the Berry Glitch hasn't begun yet
            // Set the date forward 365/366 days to avoid the glitch
            if (RtcNeedsLeapDayIncrement(&sRtcInfoWork) == TRUE)
            {
                RtcIncrementDay(&sRtcInfoWork);
                RtcIncrement(&sRtcInfoWork.year);
            }
            else
            {
                RtcIncrement(&sRtcInfoWork.year);
            }
            RtcSetDateTime(&sRtcInfoWork);
        }
    }
}

#ifdef PROGRAM_POKECHAN
// pokechan's build only. Not in pret/berry-fix, and nothing calls PrintRtc.
// The pair build an ASCII dump of the RTC, presumably for the GameCube side
// to display while the update runs. Plain ASCII, not the Ruby/Sapphire
// charmap, and the digits are "0" + nybble, which only comes out right
// because the RTC registers are BCD.
static u8 *PrintBcd(u8 *dest, u8 bcd)
{
    *dest = ((u8)(bcd & 0xF0) >> 4) + '0';
    dest++;
    *dest = (bcd & 0x0F) + '0';
    dest++;
    *dest = 0;
    return dest;
}

static u8 * UNUSED PrintRtc(u8 *dest)
{
    if (gGameVersion == VERSION_SAPPHIRE || gGameVersion == VERSION_RUBY)
        RtcGetRawInfo(&sRtcInfoWork);

    dest = PrintBcd(dest, sRtcInfoWork.year);
    *dest++ = ' ';
    dest = PrintBcd(dest, sRtcInfoWork.month);
    *dest++ = ' ';
    dest = PrintBcd(dest, sRtcInfoWork.day);
    *dest++ = ' ';
    dest = PrintBcd(dest, sRtcInfoWork.hour);
    *dest++ = ':';
    dest = PrintBcd(dest, sRtcInfoWork.minute);
    *dest++ = ':';
    dest = PrintBcd(dest, sRtcInfoWork.second);
    *dest++ = 0;
    return dest;
}
#endif
