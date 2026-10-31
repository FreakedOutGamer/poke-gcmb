#include "global.h"
#include "gba/flash_internal.h"
#include "libpmagb/save_sector.h"

struct SaveSector * UNUSED GetSaveReadBufferPointer(void)
{
    return gSaveReadBuffer;
}

void UNUSED Save_ResetSaveCounters(void)
{
    gSaveCounter = 0;
    gLastWrittenSector = 0;
    gDamagedSaveSectors = 0;
    gSaveFileStatus = 0x80;
}

void UNUSED ClearSaveData(void)
{
    s32 i;

    for (i = 0; i < 32; i++)
        EraseFlashSector(i);

    gSaveCounter = 0;
    gLastWrittenSector = 0;
    gDamagedSaveSectors = 0;
    gSaveFileStatus = 0x80;
}
