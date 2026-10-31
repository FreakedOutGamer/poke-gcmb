#include "global.h"
#include "libpmagb/save_block_pointers.h"
#include "string_util.h"
#include "constants/species.h"



















void GetSpeciesName(u8 *name, s32 species)
{
    s32 i;
    const u8 (*monSpeciesNames)[][(GAME_LANGUAGE == LANGUAGE_JAPANESE ? 5 : POKEMON_NAME_LENGTH) + 1] = (const u8 (*)[][(GAME_LANGUAGE == LANGUAGE_JAPANESE ? 5 : POKEMON_NAME_LENGTH) + 1])gAgbPmRomParams->monSpeciesNames;

    for (i = 0; i < gAgbPmRomParams->pokemonNameLength1 + gAgbPmRomParams->endOfStringLength; i++)
    {
        if (species > NUM_SPECIES)
            name[i] = (*monSpeciesNames)[0][i];
        else
            name[i] = (*monSpeciesNames)[species][i];

        if (name[i] == EOS)
            break;
    }

    name[i] = EOS;
}
