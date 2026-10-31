#ifndef GUARD_TARGET_GAME_H
#define GUARD_TARGET_GAME_H

#include "global.h"
#include "gflib/sprite.h"

#include "gf_rom_header.h"
#include "libpmagb/save_block_pointers.h"

extern const struct GFRomHeader gTargetGames[6];

// The descriptor DetectROM picked for the connected cartridge, and
// whether its game code is a Ruby/Sapphire one. They are separate questions:
// an Emerald, FireRed or LeafGreen cartridge is not RS but still has a
// descriptor, its own ROM header.
extern const struct GFRomHeader *gAgbPmRomParams;
extern bool8 gTargetIsRubySapphire;

// The loaded save blocks. The backup holds SaveBlock1 as it came off the
// cartridge; a second family of pointers is derived from it with the same
// descriptor offsets, so the program can see what it has changed.

bool8 DetectROM(void);
u8 GetPlayerMapType(void);
void UNUSED TargetGameDummy(void);
bool8 CheckIfPokedexIsObtained(void);
u32 IsFRLG(void);
bool8 CheckGameClear(void);

struct SaveBlock1;
u32 GetRSPlayerMapType(struct SaveBlock1 *sav1);

#endif // GUARD_TARGET_GAME_H
