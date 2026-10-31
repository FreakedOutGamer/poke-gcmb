#ifndef GUARD_BERRY_FIX_H
#define GUARD_BERRY_FIX_H

#define ROM_HEADER_MAGIC 0x96
#define ROM_GAME_TITLE_LEN 15

#define RomHeaderGameTitle       ((const char *)0x080000A0)
#define RomHeaderGameCode        ((const char *)0x080000AC)
#define RomHeaderMakerCode       ((const char *)0x080000B0)
#define RomHeaderMagic           ((const u8 *)  0x080000B2)
#define RomHeaderSoftwareVersion ((const u8 *)  0x080000BC)

// Return values for ValidateRomHeader
enum
{
    UPDATE_SAPPHIRE = 2,
    UPDATE_RUBY,
    NO_UPDATE_SAPPHIRE,
    NO_UPDATE_RUBY,
    INVALID
};

// Ruby/Sapphire's SaveBlock1.vars[VAR_PACIFIDLOG_TM_RECEIVED_DAY - VARS_START].
#define PACIFIDLOG_TM_DAY_OFFSET (0x1340 + (0x40C2 - 0x4000) * 2)

// The connected game's save blocks, for the berry fix code.
extern u8 *gSaveBlock2Ptr;
extern u8 *gSaveBlock1Ptr;

s32 BerryFix(u8 *saveBlock2, u8 *saveBlock1);

#endif // GUARD_BERRY_FIX_H
