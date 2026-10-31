#ifndef GUARD_TARGET_GAME_H
#define GUARD_TARGET_GAME_H

#include "global.h"


#include "gf_rom_header.h"

// FireRed, LeafGreen and Emerald carry this header at 0x08000100; Ruby and
// Sapphire have none.

// GetTargetGameType's results.
#define TARGET_GAME_RS_JAPANESE   0x10
#define TARGET_GAME_RS_ENGLISH    0x11
#define TARGET_GAME_RS_OTHER      0x12
#define TARGET_GAME_FRLG_JAPANESE 0x20
#define TARGET_GAME_FRLG_ENGLISH  0x21
#define TARGET_GAME_FRLG_OTHER    0x22
#define TARGET_GAME_OTHER         0x32 // always 0x32; see GetTargetGameType
#define TARGET_GAME_NONE          0xFF

extern u8 gTargetGameType;

// Save layout of the connected game, from its GFRomHeader (or hard-coded
// for Ruby/Sapphire).
extern u32 gSaveBlock2Size;
extern u32 gSaveBlock1Size;
extern u32 gPartyCountOffset;
extern u32 gPartyOffset;
extern u32 gPokedexOffset;
extern u32 gDexSeen2Offset;
extern u32 gDexSeen3Offset;
// The first byte of the header's flag word at 0xB8 (blockLinkBoxRS,
// skipStatusFlags, blockJirachiDistribution); 0 for Ruby/Sapphire.
extern u8 gGFRomHeaderFlags;
#define GF_HEADER_BLOCK_JIRACHI_DISTRIBUTION (1 << 2)
extern u32 gWarpFlagsOffset;
extern u32 gExternalEventFlagsOffset;

s32 GetTargetGameType(void);
bool32 InitSaveBlockChunks(void);

#endif // GUARD_TARGET_GAME_H
