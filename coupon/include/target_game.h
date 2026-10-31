#ifndef GUARD_TARGET_GAME_H
#define GUARD_TARGET_GAME_H

#include "global.h"

#include "gf_rom_header.h"
#include "libpmagb/save_block_pointers.h"
#include "gflib/sprite.h"

extern const struct GFRomHeader *gAgbPmRomParams;
extern bool8 gTargetIsRubySapphire;
extern const struct GFRomHeader gTargetGames[2];

void DetectROM(void);
bool8 CheckIfPokedexIsObtained(void);
u8 GetPlayerMapType(void);
void UNUSED TargetGameDummy(void);
struct SaveBlock1;
u32 GetRSPlayerMapType(struct SaveBlock1 *sav1);
u32 IsFRLG(void);
bool8 CheckGameClear(void);

#endif // GUARD_TARGET_GAME_H
