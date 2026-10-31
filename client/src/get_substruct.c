#include "global.h"
#include "pokemon.h"

extern union PokemonSubstruct *substructs0;
extern union PokemonSubstruct *substructs1;
extern union PokemonSubstruct *substructs2;
extern union PokemonSubstruct *substructs3;
extern union PokemonSubstruct *substructs4;
extern union PokemonSubstruct *substructs5;
extern union PokemonSubstruct *substructs6;
extern union PokemonSubstruct *substructs7;
extern union PokemonSubstruct *substructs8;
extern union PokemonSubstruct *substructs9;
extern union PokemonSubstruct *substructs10;
extern union PokemonSubstruct *substructs11;
extern union PokemonSubstruct *substructs12;
extern union PokemonSubstruct *substructs13;
extern union PokemonSubstruct *substructs14;
extern union PokemonSubstruct *substructs15;
extern union PokemonSubstruct *substructs16;
extern union PokemonSubstruct *substructs17;
extern union PokemonSubstruct *substructs18;
extern union PokemonSubstruct *substructs19;
extern union PokemonSubstruct *substructs20;
extern union PokemonSubstruct *substructs21;
extern union PokemonSubstruct *substructs22;
extern union PokemonSubstruct *substructs23;

#define SUBSTRUCT_CASE(n, v1, v2, v3, v4)  \
case n:                                     \
    switch (substructType)                  \
    {                                       \
    case 0: substruct = &substructs ## n[v1]; break; \
    case 1: substruct = &substructs ## n[v2]; break; \
    case 2: substruct = &substructs ## n[v3]; break; \
    case 3: substruct = &substructs ## n[v4]; break; \
    }                                       \
    break;

union PokemonSubstruct *GetSubstruct(struct BoxPokemon *boxMon, u32 personality,
                                     u8 substructType)
{
    union PokemonSubstruct *substruct = 0;

    substructs0 = boxMon->secure.substructs;
    substructs1 = boxMon->secure.substructs;
    substructs2 = boxMon->secure.substructs;
    substructs3 = boxMon->secure.substructs;
    substructs4 = boxMon->secure.substructs;
    substructs5 = boxMon->secure.substructs;
    substructs6 = boxMon->secure.substructs;
    substructs7 = boxMon->secure.substructs;
    substructs8 = boxMon->secure.substructs;
    substructs9 = boxMon->secure.substructs;
    substructs10 = boxMon->secure.substructs;
    substructs11 = boxMon->secure.substructs;
    substructs12 = boxMon->secure.substructs;
    substructs13 = boxMon->secure.substructs;
    substructs14 = boxMon->secure.substructs;
    substructs15 = boxMon->secure.substructs;
    substructs16 = boxMon->secure.substructs;
    substructs17 = boxMon->secure.substructs;
    substructs18 = boxMon->secure.substructs;
    substructs19 = boxMon->secure.substructs;
    substructs20 = boxMon->secure.substructs;
    substructs21 = boxMon->secure.substructs;
    substructs22 = boxMon->secure.substructs;
    substructs23 = boxMon->secure.substructs;

    switch (personality % 24)
    {
    SUBSTRUCT_CASE( 0,0,1,2,3)
    SUBSTRUCT_CASE( 1,0,1,3,2)
    SUBSTRUCT_CASE( 2,0,2,1,3)
    SUBSTRUCT_CASE( 3,0,3,1,2)
    SUBSTRUCT_CASE( 4,0,2,3,1)
    SUBSTRUCT_CASE( 5,0,3,2,1)
    SUBSTRUCT_CASE( 6,1,0,2,3)
    SUBSTRUCT_CASE( 7,1,0,3,2)
    SUBSTRUCT_CASE( 8,2,0,1,3)
    SUBSTRUCT_CASE( 9,3,0,1,2)
    SUBSTRUCT_CASE(10,2,0,3,1)
    SUBSTRUCT_CASE(11,3,0,2,1)
    SUBSTRUCT_CASE(12,1,2,0,3)
    SUBSTRUCT_CASE(13,1,3,0,2)
    SUBSTRUCT_CASE(14,2,1,0,3)
    SUBSTRUCT_CASE(15,3,1,0,2)
    SUBSTRUCT_CASE(16,2,3,0,1)
    SUBSTRUCT_CASE(17,3,2,0,1)
    SUBSTRUCT_CASE(18,1,2,3,0)
    SUBSTRUCT_CASE(19,1,3,2,0)
    SUBSTRUCT_CASE(20,2,1,3,0)
    SUBSTRUCT_CASE(21,3,1,2,0)
    SUBSTRUCT_CASE(22,2,3,1,0)
    SUBSTRUCT_CASE(23,3,2,1,0)
    }
    return substruct;
}
