#include "global.h"
#include "gba/flash_internal.h"
#include "libpmagb/save_sector.h"

// Channel-specific. Called by FullSaveAndLoopOverFailedSectors (src/libpmagb/save_write.c)
// in place of RS's save-failed screen. Instead of telling the player and
// giving up, it tries to wipe every sector gDamagedSaveSectors has flagged,
// up to three calls total (gSaveSectorWipeAttempts counts them), and reports
// whether the write is worth retrying right away.

extern u8 gSaveSectorWipeAttempts;
bool8 WipeFailedSectors(void)
{
    u32 badSectors;
    u16 i;

    {
        u8 *base = gSaveStatusFlags;
        *(u32 *)(base + 8) |= 2;
    }

    if (gDamagedSaveSectors == 0)
        return TRUE;

    if (gSaveSectorWipeAttempts > 2)
        return TRUE;

    badSectors = gDamagedSaveSectors;

    do
    {
        for (i = 0; i < 32; i++)
        {
            if (badSectors & (1 << i))
            {
                bool8 wipeFailed = WipeSector(i);
                if (!wipeFailed)
                    badSectors &= ~(1 << i);
            }
        }

        gDamagedSaveSectors = badSectors;
        gSaveSectorWipeAttempts++;

        if (badSectors != 0)
            return TRUE;
    } while (0);

    return FALSE;
}
