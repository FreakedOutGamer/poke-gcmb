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

    for (i = 0; i < (s32)ARRAY_COUNT(substruct0->raw); i++)
        checksum += substruct0->raw[i];

    for (i = 0; i < (s32)ARRAY_COUNT(substruct1->raw); i++)
        checksum += substruct1->raw[i];

    for (i = 0; i < (s32)ARRAY_COUNT(substruct2->raw); i++)
        checksum += substruct2->raw[i];

    for (i = 0; i < (s32)ARRAY_COUNT(substruct3->raw); i++)
        checksum += substruct3->raw[i];

    return checksum;
}
