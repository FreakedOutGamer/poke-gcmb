#include <string.h>
#include "global.h"
#include "pokemon.h"
#include "pokemon_data.h"
#include "save_blocks.h"
#include "constants/pokemon.h"
#include "constants/species.h"
#include "move_pp.h"

// pokeruby's CopyMon, GiveMonToPlayer and CopyMonToPC.

// A fixed EWRAM address rather than a symbol, because the target folds the box
// addresses the way it only does for a constant.
#define gPokemonStorage (*(struct PokemonStorage *)0x02008000)

u8 CopyMonToPC(struct Pokemon *mon);

void CopyMon(void *dest, void *src, size_t size)
{
    memcpy(dest, src, size);
}

u8 UNUSED GiveMonToPlayer(struct Pokemon *mon)
{
    s32 i;

    SetMonData(mon, MON_DATA_OT_NAME, gSaveBlock2.playerName);
    SetMonData(mon, MON_DATA_OT_GENDER, &gSaveBlock2.playerGender);
    SetMonData(mon, MON_DATA_OT_ID, gSaveBlock2.playerTrainerId);

    i = 0;

    while (i < 6 && GetMonData(&gPlayerParty[i], MON_DATA_SPECIES, NULL) != SPECIES_NONE)
        i++;

    if (i >= 6)
        return CopyMonToPC(mon);

    CopyMon(&gPlayerParty[i], mon, sizeof(*mon));
    gPlayerPartyCount = i + 1;
    return 0;
}

u8 CopyMonToPC(struct Pokemon *mon)
{
    s32 i = gPokemonStorage.currentBox;

    do
    {
        s32 j;
        for (j = 0; j < 30; j++)
        {
            if (GetBoxMonData(&gPokemonStorage.boxes[i][j], MON_DATA_SPECIES, NULL) == SPECIES_NONE)
            {
                MonRestorePP(mon);
                CopyMon(&gPokemonStorage.boxes[i][j], &mon->box, sizeof(mon->box));
                return 1;
            }
        }

        i++;
        if (i == 14)
            i = 0;
    } while (i != gPokemonStorage.currentBox);

    return 2;
}
