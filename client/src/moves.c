#include "global.h"
#include "pokemon.h"
#include "constants/pokemon.h"

extern const struct BattleMove gBattleMoves[];
extern const u16 gJirachiLevelUpLearnset[];

u16 GiveMoveToMon(struct Pokemon *mon, u16 move)
{
    return GiveMoveToBoxMon(&mon->box, move);
}

u16 GiveMoveToBoxMon(struct BoxPokemon *boxMon, u16 move)
{
    s32 i;

    for (i = 0; i < 4; i++)
    {
        u16 existingMove = GetBoxMonData(boxMon, 13 + i, 0);
        if (!existingMove)
        {
            SetBoxMonData(boxMon, 13 + i, &move);
            SetBoxMonData(boxMon, 17 + i, &gBattleMoves[move].pp);
            return move;
        }
        if (existingMove == move)
            return -2;
    }
    return -1;
}

// Only Jirachi's learnset is carried, so the species is read but unused.
void GiveBoxMonInitialMoveset(struct BoxPokemon *boxMon)
{
    u16 species = GetBoxMonData(boxMon, MON_DATA_SPECIES, NULL);
    s32 level = GetLevelFromBoxMonExp(boxMon);
    s32 i;

    for (i = 0; gJirachiLevelUpLearnset[i] != LEVEL_UP_END; i++)
    {
        u16 moveLevel;
        u16 move;

        moveLevel = (gJirachiLevelUpLearnset[i] & LEVEL_UP_MOVE_LV);

        // pokeruby and pokeemerald break here; this returns, which is
        // equivalent (nothing follows the loop) and compiles as the original.
        if (moveLevel > (level << 9))
            return;

        move = (gJirachiLevelUpLearnset[i] & LEVEL_UP_MOVE_ID);

        if (GiveMoveToBoxMon(boxMon, move) == MON_HAS_MAX_MOVES)
            DeleteFirstMoveAndGiveMoveToBoxMon(boxMon, move);
    }
}
