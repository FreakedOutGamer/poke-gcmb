#include "global.h"
#include "gflib/intr.h"

void IntrDummy(void)
{
}

void VBlankIntr(void)
{
    INTR_CHECK = INTR_FLAG_VBLANK;
}
