#include "global.h"
#include "pokemon.h"

u16 CalculateBoxMonChecksum(struct BoxPokemon *boxMon)
{
    u16 checksum = 0;
    union PokemonSubstruct *substruct0 = GetSubstruct(boxMon, boxMon->personality, 0);
    union PokemonSubstruct *substruct1 = GetSubstruct(boxMon, boxMon->personality, 1);
    union PokemonSubstruct *substruct2 = GetSubstruct(boxMon, boxMon->personality, 2);
    union PokemonSubstruct *substruct3 = GetSubstruct(boxMon, boxMon->personality, 3);
    s32 i;

    for (i = 0; i < 6; i++)
        checksum += substruct0->raw[i];
    for (i = 0; i < 6; i++)
        checksum += substruct1->raw[i];
    for (i = 0; i < 6; i++)
        checksum += substruct2->raw[i];
    for (i = 0; i < 6; i++)
        checksum += substruct3->raw[i];

    return checksum;
}

