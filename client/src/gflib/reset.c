
/* Performs the reset preparation, then invokes the BIOS reset routine. */
#include "global.h"
#include "gba/syscall.h"
#include "gflib/keys.h"
#include "main.h"

void SoftResetRom(void)
{
    while (1)
    {
        ReadKeys();
        VBlankIntrWait();
    }
}
