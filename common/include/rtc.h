#ifndef GUARD_RTC_UTIL_H
#define GUARD_RTC_UTIL_H

#include "rtc_time.h"

#include "siirtc.h"

#define RTC_INIT_ERROR         0x0001
#define RTC_INIT_WARNING       0x0002

#define RTC_ERR_12HOUR_CLOCK   0x0010
#define RTC_ERR_POWER_FAILURE  0x0020
#define RTC_ERR_INVALID_YEAR   0x0040
#define RTC_ERR_INVALID_MONTH  0x0080
#define RTC_ERR_INVALID_DAY    0x0100
#define RTC_ERR_INVALID_HOUR   0x0200
#define RTC_ERR_INVALID_MINUTE 0x0400
#define RTC_ERR_INVALID_SECOND 0x0800

#define RTC_ERR_FLAG_MASK      0x0FF0

extern struct Time gLocalTime;
extern struct Time gLocalTimeOffset; // SaveBlock2.localTimeOffset in Ruby/Sapphire

// pret/berry-fix's rtc.c globals.
extern struct Time gRtcUTCTime;
extern struct Time gTimeSinceBerryUpdate;

extern const s32 sNumDaysInMonths[12];

bool32 BerryFix_TryInitRtc(void);
bool32 BerryFix_CalcTimeDifference(u8 *year);
void BerryFix_TryFixDate(void);

void RtcDisableInterrupts(void);
void RtcRestoreInterrupts(void);
s32 ConvertBcdToBinary(u8 bcd);
bool8 IsLeapYear(u8 year);
u16 RtcGetDayCount(struct SiiRtcInfo *rtc);
void RtcInit(void);
u16 RtcGetErrorStatus(void);
void RtcGetInfo(struct SiiRtcInfo *rtc);
void RtcGetDateTime(struct SiiRtcInfo *rtc);
void RtcGetStatus(struct SiiRtcInfo *rtc);
void RtcGetRawInfo(struct SiiRtcInfo *rtc);
void RtcReset(void);
void RtcCalcTimeDifference(struct SiiRtcInfo *rtc, struct Time *result, struct Time *t);
void RtcCalcLocalTime(void);
void RtcInitLocalTimeOffset(s32 hour, s32 minute);
void RtcCalcLocalTimeOffset(s32 days, s32 hours, s32 minutes, s32 seconds);
void CalcTimeDifference(struct Time *result, struct Time *t1, struct Time *t2);
u32 RtcGetMinuteCount(void);

#endif // GUARD_RTC_UTIL_H
