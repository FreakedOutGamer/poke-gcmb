#include "global.h"
#include "pokemon.h"
#include "libpmagb/save_block_pointers.h"
#include "constants/pokemon.h"
#include "gflib/sprite.h"
#include "pokeball.h"
#include "constants/items.h"

const u32 *BoxMonGetCaughtBallItemSpriteSheet(struct BoxPokemon *boxMon)
{
    u32 id;
    u16 ball = GetBoxMonData(boxMon, MON_DATA_POKEBALL, NULL);

    switch (ball)
    {
    case ITEM_MASTER_BALL:  id = BALL_MASTER;   break;
    case ITEM_ULTRA_BALL:   id = BALL_ULTRA;    break;
    case ITEM_GREAT_BALL:   id = BALL_GREAT;    break;
    case ITEM_POKE_BALL:    id = BALL_POKE;     break;
    case ITEM_SAFARI_BALL:  id = BALL_SAFARI;   break;
    case ITEM_NET_BALL:     id = BALL_NET;      break;
    case ITEM_DIVE_BALL:    id = BALL_DIVE;     break;
    case ITEM_NEST_BALL:    id = BALL_NEST;     break;
    case ITEM_REPEAT_BALL:  id = BALL_REPEAT;   break;
    case ITEM_TIMER_BALL:   id = BALL_TIMER;    break;
    case ITEM_LUXURY_BALL:  id = BALL_LUXURY;   break;
    case ITEM_PREMIER_BALL: id = BALL_PREMIER;  break;
    default:                id = BALL_POKE;     break;
    }

    return gAgbPmRomParams->ballGfx[id].data;
}

const u32 *BoxMonGetCaughtBallItemPalette(struct BoxPokemon *boxMon)
{
    u32 id;
    u16 ball = GetBoxMonData(boxMon, MON_DATA_POKEBALL, NULL);

    switch (ball)
    {
    case ITEM_MASTER_BALL:  id = BALL_MASTER;   break;
    case ITEM_ULTRA_BALL:   id = BALL_ULTRA;    break;
    case ITEM_GREAT_BALL:   id = BALL_GREAT;    break;
    case ITEM_POKE_BALL:    id = BALL_POKE;     break;
    case ITEM_SAFARI_BALL:  id = BALL_SAFARI;   break;
    case ITEM_NET_BALL:     id = BALL_NET;      break;
    case ITEM_DIVE_BALL:    id = BALL_DIVE;     break;
    case ITEM_NEST_BALL:    id = BALL_NEST;     break;
    case ITEM_REPEAT_BALL:  id = BALL_REPEAT;   break;
    case ITEM_TIMER_BALL:   id = BALL_TIMER;    break;
    case ITEM_LUXURY_BALL:  id = BALL_LUXURY;   break;
    case ITEM_PREMIER_BALL: id = BALL_PREMIER;  break;
    default:                id = BALL_POKE;     break;
    }

    return gAgbPmRomParams->ballPalettes[id].data;
}
