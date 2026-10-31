#ifndef GUARD_POKEBALL_H
#define GUARD_POKEBALL_H

#include "global.h"

// pokeemerald's pokeball.h. Index order for gGFRomHeaderPtr's ball gfx
// arrays (target_game.h) and ItemIdToBallId's return value, confirmed by
// the item ids matched in src/ball_gfx.c's own remap switch.
enum
{
    BALL_POKE,
    BALL_GREAT,
    BALL_SAFARI,
    BALL_ULTRA,
    BALL_MASTER,
    BALL_NET,
    BALL_DIVE,
    BALL_NEST,
    BALL_REPEAT,
    BALL_TIMER,
    BALL_LUXURY,
    BALL_PREMIER,
    POKEBALL_COUNT
};

u8 UNUSED ItemIdToBallId(u16 ballItem);

#endif // GUARD_POKEBALL_H
