#include "global.h"
#include "pokemon.h"
#include "constants/pokemon.h"

u32 GetBoxMonMoveBySlot(struct BoxPokemon *boxMon, s32 slot)
{
    u32 move;

    switch (slot)
    {
    case 0: move = GetBoxMonData(boxMon, MON_DATA_MOVE1, NULL); break;
    case 1: move = GetBoxMonData(boxMon, MON_DATA_MOVE2, NULL); break;
    case 2: move = GetBoxMonData(boxMon, MON_DATA_MOVE3, NULL); break;
    default:
    case 3: move = GetBoxMonData(boxMon, MON_DATA_MOVE4, NULL); break;
    }
    return move;
}

u32 GetBoxMonPPByMoveSlot(struct BoxPokemon *boxMon, u8 slot)
{
    u32 pp;

    switch (slot)
    {
    case 0: pp = GetBoxMonData(boxMon, MON_DATA_PP1, NULL); break;
    case 1: pp = GetBoxMonData(boxMon, MON_DATA_PP2, NULL); break;
    case 2: pp = GetBoxMonData(boxMon, MON_DATA_PP3, NULL); break;
    default:
    case 3: pp = GetBoxMonData(boxMon, MON_DATA_PP4, NULL); break;
    }
    return pp;
}
