#include "global.h"
#include "libpmagb/save_block_pointers.h"
#include "libpmagb/save.h"
#include "gflib/characters.h"
#include "gflib/keys.h"
#include "gflib/init.h"
#include "pokeball.h"
#include "colbtl.h"
#include "pokedex.h"
#include "data.h"
#include "joybus.h"
#include "flag.h"
#include "target_game.h"
#include "string_util.h"
#include "constants/items.h"
#include "constants/maps.h"
#include "gba/syscall.h"

bool32 VBlankCB(void)
{
    if (*(u8*)(0x80000B2) != 0x96) // gRomHeader.gameCode[6] != 0x96
        SoftReset(0);

    if (gJoyBusState.linkTimeoutCounter <= gJoyBusState.linkTimeoutLimit)
    {
        REG_IME = 0;
        gJoyBusState.linkTimeoutCounter++;
        REG_IME = 1;
        return FALSE;
    }

    gJoyBusState.linkTimedOut = 1;
    SoftReset(0);
}
