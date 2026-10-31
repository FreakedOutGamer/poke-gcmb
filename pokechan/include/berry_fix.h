#ifndef GUARD_BERRY_FIX_H
#define GUARD_BERRY_FIX_H

#include "global.h"
#include "constants/vars.h"

#define ROM_HEADER_MAGIC 0x96
#define ROM_GAME_TITLE_LEN 15

#define RomHeaderGameTitle       ((const char *)0x080000A0)
#define RomHeaderGameCode        ((const char *)0x080000AC)
#define RomHeaderMakerCode       ((const char *)0x080000B0)
#define RomHeaderMagic           ((const u8 *)  0x080000B2)
#define RomHeaderSoftwareVersion ((const u8 *)  0x080000BC)

// States for BerryFix
enum {
    STATE_INIT,
    STATE_CHECK_RTC,
    STATE_CHECK_FLASH,
    STATE_READ_SAVE,
    STATE_CHECK_TIME,
    STATE_FIX_DATE,
    STATE_NO_NEED_TO_FIX,
    STATE_ERROR_YEAR,
    STATE_FINISHED,
    STATE_CHECK_PACIFIDLOG_TM,
    STATE_FIX_PACIFIDLOG_TM,
    STATE_ERROR
};

// Return values for ValidateRomHeader
enum
{
    UPDATE_SAPPHIRE = 2,
    UPDATE_RUBY,
    NO_UPDATE_SAPPHIRE,
    NO_UPDATE_RUBY,
    INVALID
};


extern u32 gGameVersion;
extern u32 gUpdateSuccessful;

// Pointer to the connected cartridge's SaveBlock1.vars, the same way
// gFlagsPtr points at its flags.
extern u16 *gVarsPtr;

void BerryFix(u32 *state, void *unused1, void *unused2);
bool32 BerryFix_IsPacifidlogTMCorrect(void);
bool32 BerryFix_ResetPacifidlogTM(void);

s32 ValidateRomHeader(void);

#endif // GUARD_BERRY_FIX_H
