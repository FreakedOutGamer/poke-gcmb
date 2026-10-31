#include "global.h"
#include "pokemon_data.h"
#include "move_pp.h"

extern const struct BattleMove gBattleMoves[];
extern const u8 gPPUpReadMasks[];

// pokeruby's.
u8 CalculatePPWithBonus(u16 move, u8 ppBonuses, u8 moveIndex)
{
    u8 basePP = gBattleMoves[move].pp;
    return basePP + ((basePP * 20 * ((gPPUpReadMasks[moveIndex] & ppBonuses) >> (2 * moveIndex))) / 100);
}
