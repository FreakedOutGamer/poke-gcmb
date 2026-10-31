#include "global.h"
#include "gba/gba.h"
#include "joybus.h"
#include "gflib/init.h"


void InitLink(const u8 *gameCode)
{
    const u8 *src = gameCode;
    u16 imeBackup = REG_IME;
    u32 i;

    REG_IME = 0;

    for (i = 0; i <= 0x2AA7; i++)
        ((u8 *)&gJoyBusState)[i] = 0;

    gJoyBusState.skipRcntInit = 1;
    InitJoyBus();

    REG_IE |= INTR_FLAG_SERIAL;

    if (src[0] == 'T' && src[1] == 'E' && src[2] == 'S' && src[3] == 'T')
        gJoyBusState.linkTimeoutLimit = 254;
    else
        gJoyBusState.linkTimeoutLimit = 30;

    gJoyBusState.ownGameCode = (src[3] << 24) | (src[2] << 16) | (src[1] << 8) | src[0];
    gJoyBusState.gameCodeRaw = gJoyBusState.ownGameCode;
    gJoyBusState.gameCodeLower = gJoyBusState.ownGameCode | 0x20202020;
    gJoyBusState.makerCodeWord = (src[4] << 24) | (src[5] << 16) | (src[6] << 8) | src[7];

    SetIntrFunc(0, JoyBusVCountIntr);

    REG_IME = imeBackup;
}
