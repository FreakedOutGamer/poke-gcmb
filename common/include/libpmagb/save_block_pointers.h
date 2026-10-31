#ifndef PMAGB_SAVE_BLOCK_POINTERS_H
#define PMAGB_SAVE_BLOCK_POINTERS_H

#include "gf_rom_header.h"

extern void * gSaveBlock1Ptr;
extern void * gSaveBlock1BackupPtr;
extern void * gSaveBlock2Ptr;
extern struct PokemonStorage * gPokemonStoragePtr;

extern struct Pokemon * gPlayerPartyPtr;
extern struct Pokemon * gPlayerPartyBackupPtr;
extern u8 * gPlayerPartyCountPtr;
// The three copies of the seen flags Ruby/Sapphire cross-checks. Each is
// precomputed once from the descriptor's offsets, because they live in the
// connected cartridge's save blocks (pokedex and dexSeen1 in SaveBlock2,
// dexSeen2 and dexSeen3 in SaveBlock1).
extern struct Pokedex * gPokedexPtr;
extern u8 * gDexSeen2Ptr;
extern u8 * gDexSeen3Ptr;
extern struct ItemSlot * gPcItemsPtr;
extern u8 * gFlagsPtr;
extern u16 * gVarsPtr;
extern u8 * gGiftRibbonsPtr;
extern struct EnigmaBerry * gEnigmaBerryPtr;

void SaveBlocksInit(void);

#endif //PMAGB_SAVE_BLOCK_POINTERS_H
