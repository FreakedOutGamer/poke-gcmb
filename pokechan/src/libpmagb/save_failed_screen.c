#include "global.h"
#include "gba/flash_internal.h"
#include "libpmagb/save_sector.h"

// Clears a failed flash sector by programming zeroes and verifying the result.

static inline bool8 VerifySectorWipe(u16 sector)
{
    u32 *ptr;
    u16 i;

    ReadFlash(sector, 0, gSaveReadBuffer, 4096);
    ptr = (u32 *)gSaveReadBuffer;

    for (i = 0; i < 0x400; i++, ptr++)
        if (*ptr != 0)
            return TRUE;

    return FALSE;
}

bool8 WipeSector(u16 sector)
{
    u16 i, j;
    bool8 failed = TRUE;

    for (i = 0; failed && i < 130; i++)
    {
        for (j = 0; j < 0x1000; j++)
            ProgramFlashByte(sector, j, 0);

        failed = VerifySectorWipe(sector);
    }

    return failed;
}
