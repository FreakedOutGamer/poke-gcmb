#ifndef GUARD_SAVE_BLOCKS_H
#define GUARD_SAVE_BLOCKS_H

#include "global.h"
#include "pokemon.h"

// The parts of Ruby/Sapphire's save blocks this program touches. Unlike the
// games, it keeps both blocks in IWRAM (gSaveBlock2 at 0x03005AA0,
// gSaveBlock1 at 0x03001D80).

#define DEX_FLAGS_NO 52

struct Pokedex
{
    /*0x00*/ u8 order;
    /*0x01*/ u8 mode;
    /*0x02*/ u8 nationalMagic;
    /*0x03*/ u8 unknown2;
    /*0x04*/ u32 unownPersonality;
    /*0x08*/ u32 spindaPersonality;
    /*0x0C*/ u32 unknown3;
    /*0x10*/ u8 owned[DEX_FLAGS_NO];
    /*0x44*/ u8 seen[DEX_FLAGS_NO];
};

struct SaveBlock2
{
    /*0x00*/ u8 playerName[8];
    /*0x08*/ u8 playerGender;
    /*0x09*/ u8 specialSaveWarp;
    /*0x0A*/ u8 playerTrainerId[4];
    /*0x0E*/ u8 filler_0E[0x0A];
    /*0x18*/ struct Pokedex pokedex;
};

struct WarpData
{
    s8 mapGroup;
    s8 mapNum;
    s8 warpId;
    s16 x, y;
};

struct ObjectEvent
{
    u32 filler[9];
};

struct MailStruct
{
    /*0x00*/ u16 words[9];
    /*0x12*/ u8 playerName[8];
    /*0x1A*/ u8 trainerId[4];
    /*0x1E*/ u16 species;
    /*0x20*/ u16 itemId;
} ALIGNED(4);

#define OBJECT_EVENTS_COUNT 16
// This save layout holds six mail.
#define SAVE_MAIL_COUNT 6
#define NUM_GAME_STATS 50

struct SaveBlock1
{
    /*0x0000*/ u8 filler_0000[0x0C];
    /*0x000C*/ struct WarpData warp1;
    /*0x0014*/ struct WarpData dynamicWarp;
    /*0x001C*/ u8 filler_001C[0x234 - 0x1C];
    /*0x0234*/ u8 playerPartyCount;
    /*0x0238*/ struct Pokemon playerParty[PARTY_SIZE];
    /*0x0490*/ u8 filler_0490[0x938 - 0x490];
    /*0x0938*/ u8 dexSeen2[DEX_FLAGS_NO];
    /*0x096C*/ u8 filler_096C[0x9E0 - 0x96C];
    /*0x09E0*/ struct ObjectEvent objectEvents[OBJECT_EVENTS_COUNT];
    /*0x0C20*/ u8 filler_0C20[0x1220 - 0xC20];
    /*0x1220*/ u8 flags[0x120];
    /*0x1340*/ u16 vars[0x100];
    /*0x1540*/ u32 gameStats[NUM_GAME_STATS];
    /*0x1608*/ u8 filler_1608[0x2B4C - 0x1608];
    /*0x2B4C*/ struct MailStruct mail[SAVE_MAIL_COUNT];
    /*0x2C24*/ u8 filler_2C24[0x3A8C - 0x2C24];
    /*0x3A8C*/ u8 dexSeen3[DEX_FLAGS_NO];
};

extern struct SaveBlock2 gSaveBlock2;
extern struct SaveBlock1 gSaveBlock1;

// sample0519's own party, in IWRAM (payload_raw.s places it).
extern struct Pokemon gPlayerParty[PARTY_SIZE];
extern u8 gPlayerPartyCount;

#endif // GUARD_SAVE_BLOCKS_H
