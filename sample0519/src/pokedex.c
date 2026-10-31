#include "global.h"
#include "pokemon.h"
#include "constants/pokedex.h"
#include "save_blocks.h"
#include "pokedex_util.h"

// pokeruby's GetSetPokedexFlag.

s8 GetSetPokedexFlag(u16 nationalDexNo, u8 caseID)
{
    u8 index;
    u8 bit;
    u8 mask;
    s8 retVal;

    nationalDexNo--;
    index = nationalDexNo / 8;
    bit = nationalDexNo % 8;
    mask = 1 << bit;
    retVal = 0;
    switch (caseID)
    {
    case FLAG_GET_SEEN:
        if (gSaveBlock2.pokedex.seen[index] & mask)
        {
            if ((gSaveBlock2.pokedex.seen[index] & mask) == (gSaveBlock1.dexSeen2[index] & mask)
             && (gSaveBlock2.pokedex.seen[index] & mask) == (gSaveBlock1.dexSeen3[index] & mask))
                retVal = 1;
            else
            {
                gSaveBlock2.pokedex.seen[index] &= ~mask;
                gSaveBlock1.dexSeen2[index] &= ~mask;
                gSaveBlock1.dexSeen3[index] &= ~mask;
                retVal = 0;
            }
        }
        break;
    case FLAG_GET_CAUGHT:
        if (gSaveBlock2.pokedex.owned[index] & mask)
        {
            if ((gSaveBlock2.pokedex.owned[index] & mask) == (gSaveBlock2.pokedex.seen[index] & mask)
             && (gSaveBlock2.pokedex.owned[index] & mask) == (gSaveBlock1.dexSeen2[index] & mask)
             && (gSaveBlock2.pokedex.owned[index] & mask) == (gSaveBlock1.dexSeen3[index] & mask))
                retVal = 1;
            else
            {
                gSaveBlock2.pokedex.owned[index] &= ~mask;
                gSaveBlock2.pokedex.seen[index] &= ~mask;
                gSaveBlock1.dexSeen2[index] &= ~mask;
                gSaveBlock1.dexSeen3[index] &= ~mask;
                retVal = 0;
            }
        }
        break;
    case FLAG_SET_SEEN:
        gSaveBlock2.pokedex.seen[index] |= mask;
        gSaveBlock1.dexSeen2[index] |= mask;
        gSaveBlock1.dexSeen3[index] |= mask;
        break;
    case FLAG_SET_CAUGHT:
        gSaveBlock2.pokedex.owned[index] |= mask;
        break;
    }
    return retVal;
}
