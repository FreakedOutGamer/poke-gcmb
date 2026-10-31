#ifndef GUARD_RTC_TIME_H
#define GUARD_RTC_TIME_H

// A time of day plus a day count, as Ruby/Sapphire's RTC code keeps it.
struct Time
{
    /*0x00*/ s16 days;
    /*0x02*/ s8 hours;
    /*0x03*/ s8 minutes;
    /*0x04*/ s8 seconds;
};

#endif // GUARD_RTC_TIME_H
