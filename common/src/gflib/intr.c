#include "global.h"
#include "gba/gba.h"
#include "gflib/sprite.h"
#include "gflib/bg.h"
#include "gflib/intr.h"

// The VBlank interrupt handler and the handler for interrupts nothing uses.

typedef void (*IntrFunc)(void);

extern u32 gVBlankCounter;
extern IntrFunc gVBlankCallback;

void VBlankIntr(void)
{
    DoOamBufferTransfer();
    DoGpuUpdateAndTilemapTransfers();
    gVBlankCounter++;
    INTR_CHECK = INTR_FLAG_VBLANK;
    if (gVBlankCallback != NULL)
        gVBlankCallback();
}

void IntrDummy(void)
{
}
