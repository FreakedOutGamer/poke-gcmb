#include "global.h"
#include "pokemon.h"

extern const struct BattleMove gBattleMoves[];

void DeleteFirstMoveAndGiveMoveToBoxMon(struct BoxPokemon *boxMon, u16 move)
{
    s32 i;
    u16 moves[4];
    u8 pp[4];
    u8 ppBonuses;

    for (i = 0; i < 3; i++)
    {
        moves[i] = GetBoxMonData(boxMon, 14 + i, 0);
        pp[i] = GetBoxMonData(boxMon, 18 + i, 0);
    }
    ppBonuses = GetBoxMonData(boxMon, 21, 0);
    ppBonuses >>= 2;
    moves[3] = move;
    pp[3] = gBattleMoves[move].pp;

    for (i = 0; i < 4; i++)
    {
        SetBoxMonData(boxMon, 13 + i, &moves[i]);
        SetBoxMonData(boxMon, 17 + i, &pp[i]);
    }
    SetBoxMonData(boxMon, 21, &ppBonuses);
}
