#ifndef GUARD_TARGET_GAME_H
#define GUARD_TARGET_GAME_H

// Where the player saved (GetPlayerMapType).
#define CONTINUE_GAME_WARP           (1 << 0)

// The Ruby/Sapphire ROM descriptors (src/target_games.c).
extern struct GFRomHeader gAgbPmRomParams_AXVE_rev0;
extern struct GFRomHeader gAgbPmRomParams_AXVE_rev2;
extern struct GFRomHeader gAgbPmRomParams_AXPE_rev0;
extern struct GFRomHeader gAgbPmRomParams_AXPE_rev2;

u8 GetPlayerMapType(void);
u8 GetRSPlayerMapType(struct SaveBlock1 *sav1);
u8 CheckIfPokedexIsObtained(void);
bool32 IsFRLG(void);
bool32 CheckGameClear(void);

#endif // GUARD_TARGET_GAME_H
