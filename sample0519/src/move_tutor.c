#include "global.h"
#include "pokemon.h"

#include "pokemon_data.h"
#include "constants/pokemon.h"
#include "constants/species.h"
#include "move_pp.h"

#define MAX_MOVE_TUTOR_MOVES 20

// pokeruby's GetMoveTutorMoves, MonRestorePP and BoxMonRestorePP.

extern const u16 *const gLevelUpLearnsets[];
extern const u8 gPPUpWriteMasks[];

u8 UNUSED GetMoveTutorMoves(struct Pokemon *mon, u16 *moves)
{
    u16 knownMoves[4];
    u8 numMoves = 0;
    u16 species = GetMonData(mon, MON_DATA_SPECIES, 0);
    u8 level = GetMonData(mon, MON_DATA_LEVEL, 0);
    int i, j, k;

    for (i = 0; i < 4; i++)
        knownMoves[i] = GetMonData(mon, MON_DATA_MOVE1 + i, 0);

    for (i = 0; i < MAX_MOVE_TUTOR_MOVES; i++)
    {
        u16 moveLevel;

        if (gLevelUpLearnsets[species][i] == 0xFFFF)
            break;

        moveLevel = gLevelUpLearnsets[species][i] & 0xFE00;
        if (moveLevel <= (level << 9))
        {
            for (j = 0; j < 4 && knownMoves[j] != (gLevelUpLearnsets[species][i] & 0x1FF); j++)
                ;

            if (j == 4)
            {
                for (k = 0; k < numMoves && moves[k] != (gLevelUpLearnsets[species][i] & 0x1FF); k++)
                    ;

                if (k == numMoves)
                    moves[numMoves++] = gLevelUpLearnsets[species][i] & 0x1FF;
            }
        }
    }

    return numMoves;
}

void MonRestorePP(struct Pokemon *mon)
{
    BoxMonRestorePP(&mon->box);
}

void BoxMonRestorePP(struct BoxPokemon *boxMon)
{
    int i;

    for (i = 0; i < 4; i++)
    {
        if (GetBoxMonData(boxMon, MON_DATA_MOVE1 + i, 0))
        {
            u16 move = GetBoxMonData(boxMon, MON_DATA_MOVE1 + i, 0);
            u16 bonus = GetBoxMonData(boxMon, MON_DATA_PP_BONUSES, 0);
            u8 pp = CalculatePPWithBonus(move, bonus, i);
            SetBoxMonData(boxMon, MON_DATA_PP1 + i, &pp);
        }
    }
}
