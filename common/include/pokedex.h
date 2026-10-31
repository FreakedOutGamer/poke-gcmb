#ifndef GUARD_POKEDEX_H
#define GUARD_POKEDEX_H

#include "global.h"
#include "pokemon.h"
#include "constants/species.h"
#include "libpmagb/save_block_pointers.h"

#define DEX_FLAGS_NO ((NUM_SPECIES / 8) + ((NUM_SPECIES % 8) ? 1 : 0))

// Ruby/Sapphire's Pokedex block, laid out exactly as it sits in the
// connected cartridge's SaveBlock2.
struct Pokedex
{
    /*0x00*/ u8 order;
    /*0x01*/ u8 mode;
    /*0x02*/ u8 nationalMagic; // must equal 0xDA in order to have National mode
    /*0x03*/ u8 unknown2;
    /*0x04*/ u32 unownPersonality; // Set when the first Unown is seen
    /*0x08*/ u32 spindaPersonality; // Set when the first Spinda is seen
    /*0x0C*/ u32 unknown3;
    /*0x10*/ u8 owned[DEX_FLAGS_NO];
    /*0x44*/ u8 seen[DEX_FLAGS_NO];
};


extern const u16 gSpeciesToNationalPokedexNum[];

bool32 GetSetPokedexFlag(u16 nationalDexNo, u8 caseID);
void SetSpeciesCaughtFlag(u32 species, struct Pokemon *mon);

#endif // GUARD_POKEDEX_H
