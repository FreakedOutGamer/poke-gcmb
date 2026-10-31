#include "global.h"
#include "gba/gba.h"
#include "joybus.h"
#include "target_game.h"

#define ROM_HEADER_MAGIC 0x96

bool32 VBlankCB(void)
{
    if (CART_ROM[0xB2] != ROM_HEADER_MAGIC)
        SoftReset(0);

    if (gJoyBusState.linkTimeoutCounter <= gJoyBusState.linkTimeoutLimit)
    {
        REG_IME = 0;
        gJoyBusState.linkTimeoutCounter++;
        REG_IME = 1;
        return 0;
    }

    gJoyBusState.linkTimedOut = 1;
    SoftReset(0);
}

// Unused, and nothing holds its address either.
void UNUSED CopyBytes(void *dest, const void *src, u32 size)
{
    u8 *d = dest;
    const u8 *s = src;

    while (size != 0)
    {
        *d = *s;
        s++;
        d++;
        size--;
    }
}
