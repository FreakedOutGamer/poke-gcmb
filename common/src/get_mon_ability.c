#include "global.h"
#include "libpmagb/save_block_pointers.h"
#include "pokemon.h"

u32 GetBoxMonAbility(struct BoxPokemon *boxMon)
{
    u32 ability;
    const struct SpeciesInfo *speciesInfo = gAgbPmRomParams->speciesInfo;
    u32 species = GetBoxMonData(boxMon, MON_DATA_SPECIES, NULL);
    u32 abilityNum = GetBoxMonData(boxMon, MON_DATA_ABILITY_NUM, NULL);

    if (abilityNum == 0)
        ability = speciesInfo[species].abilities[0];
    else
        ability = speciesInfo[species].abilities[1];

    return ability;
}
