#include "global.h"
#include "constants/species.h"
#include "pokedex_util.h"

#define POKEMON_SLOTS_NUMBER NUM_SPECIES

// pokeruby's Pokédex number conversions (the ones this program keeps).

extern const u16 gSpeciesToHoennPokedexNum[];
extern const u16 gSpeciesToNationalPokedexNum[];

u16 UNUSED HoennPokedexNumToSpecies(u16 hoennNum)
{
    u16 species;

    if (!hoennNum)
        return 0;

    species = 0;

    while (species < POKEMON_SLOTS_NUMBER - 1 && gSpeciesToHoennPokedexNum[species] != hoennNum)
        species++;

    if (species == POKEMON_SLOTS_NUMBER - 1)
        return 0;

    return species + 1;
}

u16 UNUSED NationalPokedexNumToSpecies(u16 nationalNum)
{
    u16 species;

    if (!nationalNum)
        return 0;

    species = 0;

    while (species < POKEMON_SLOTS_NUMBER - 1 && gSpeciesToNationalPokedexNum[species] != nationalNum)
        species++;

    if (species == POKEMON_SLOTS_NUMBER - 1)
        return 0;

    return species + 1;
}

u16 SpeciesToNationalPokedexNum(u16 species)
{
    if (!species)
        return 0;

    return gSpeciesToNationalPokedexNum[species - 1];
}
