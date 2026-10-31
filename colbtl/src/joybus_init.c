#include "global.h"
#include <stddef.h>
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


void InitJoyBus(void)
{
    u16 UNUSED joyRcv;
    u16 ime = REG_IME;

    REG_IME = 0;
    if (gJoyBusState.skipRcntInit == 0)
        REG_RCNT = RCNT_JOYBUS2;
    REG_RCNT = RCNT_JOYBUS1 | RCNT_JOYBUS2;
    REG_JOYSTAT = 0;
    joyRcv = REG_JOY_RECV;
    REG_JOY_TRANS = 0;
    REG_JOYCNT = JOYCNT_RESET | JOYCNT_RECV_CMPLT | JOYCNT_TRAN_CMPLT | JOYCNT_RESET_IRQ;
    REG_IF = INTR_FLAG_SERIAL;
    REG_IE |= INTR_FLAG_SERIAL;

    gJoyBusState.linkTimeoutCounter = 0;
    gJoyBusState.linked = 0;
    gJoyBusState.handshakeStep = 0;
    gJoyBusState.skipRcntInit = 0;
    gJoyBusState.command = LINK_CMD_RESET;
    gJoyBusState.recvActive = 0;

    REG_IME = ime;
}

void InitLink(const u8 *gameCode)
{
    s32 i;
    u16 ime = REG_IME;

    REG_IME = 0;

    // colbtl's link state ends where pokechan's and coupon's recvFlags starts.
    for (i = 0; i < offsetof(struct JoyBusState, recvFlags); i++)
    {
        *((u8 *)(&gJoyBusState) + i) = 0;
    }

    gJoyBusState.skipRcntInit = 1;
    InitJoyBus();
    REG_IE |= INTR_FLAG_SERIAL;

    if (gameCode[0] == 'T' && gameCode[1] == 'E' && gameCode[2] == 'S' && gameCode[3] == 'T')
        gJoyBusState.linkTimeoutLimit = 0xFE;
    else
        gJoyBusState.linkTimeoutLimit = 0x1E;

    gJoyBusState.ownGameCode = (gameCode[3] << 24) | (gameCode[2] << 16) | (gameCode[1] << 8) | (gameCode[0]);
    gJoyBusState.gameCodeRaw = gJoyBusState.ownGameCode;
    gJoyBusState.gameCodeLower = gJoyBusState.ownGameCode | 0x20202020; // Turns upper case ASCII to lower case
    gJoyBusState.makerCodeWord = (gameCode[4] << 24) | (gameCode[5] << 16) | (gameCode[6] << 8) | (gameCode[7]);
    SetIntrFunc(0, JoyBusVCountIntr);

    REG_IME = ime;
}
