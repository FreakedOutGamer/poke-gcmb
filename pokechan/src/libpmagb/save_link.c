#include "global.h"
#include "gba/gba.h"
#include "gba/flash_internal.h"
#include "libpmagb/save_sector.h"
#include "target_game.h"

// WriteLinkSave runs one phase of the save the GameCube drives (0, then 1),
// retrying damaged sectors as it goes. Phase 0 wipes the whole flash first
// when SaveBlock2 is empty.

extern u8 gSaveSectorWipeAttempts;

static inline u32 Setup(const struct SaveBlockChunk *chunks)
{
    gReadWriteSector = gSaveReadBuffer;
    gLastKnownGoodSector = gLastWrittenSector;
    gLastSaveCounter = gSaveCounter;
    gLastWrittenSector++;
    gLastWrittenSector %= NUM_SECTORS_PER_SAVE_SLOT;
    gSaveCounter++;
    gIncrementalSectorId = 0;
    gDamagedSaveSectors = 0;

    return 0;
}

u32 WriteLinkSave(s32 saveType)
{
    switch (saveType)
    {
    case 0:
    {
        {
            u8 *base = gSaveStatusFlags;
            *(u32 *)(base + 8) |= 1;
            base[4] &= 0xFE;
            *(u32 *)(base + 8) &= ~6;
        }

        {
            u8 *sb2 = (u8 *)gSaveBlock2Ptr;
            u32 i;
            s32 j;

            for (i = 0; i < gAgbPmRomParams->saveBlock2Size; i++)
                if (sb2[i] != 0)
                    goto writeSetup;

            for (j = 0; j < 32; j++)
                EraseFlashSector(j);

            gSaveCounter = 0;
            gLastWrittenSector = 0;
            gDamagedSaveSectors = 0;
            gSaveFileStatus = 0x80;

            {
                u8 *base = gSaveStatusFlags;
                *(u32 *)(base + 8) |= 8;
                base[1] = 4;
                *(u16 *)(base + 16) = 0;
            }
        }

    writeSetup:

        Setup(gSaveBlockChunks);
        {
            u8 *status = gSaveStatusFlags;

            status[1]++;
            *(u16 *)(status + 16) = 0;
            DelayFrames(5);

            // FullSaveAndLoopOverFailedSectors returns u8, but the result is tested
            // here without being narrowed, as if it returned int.
            while (!((u32 (*)(void))FullSaveAndLoopOverFailedSectors)())
            {
                if (status[4] & 1)
                    goto returnTrue;
            }
        }

        gSaveSectorWipeAttempts = 0;
    replaceRetry:
        HandleReplaceSector(13, gSaveBlockChunks);
        if (gDamagedSaveSectors == 0)
            goto sharedTail;
        if (!WipeFailedSectors())
            goto replaceRetry;
        gSaveStatusFlags[4] |= 1;
        goto sharedTail;
    }

    case 1:
    {
        u8 i = 0;

    retry1:
        WriteSectorSignatureByte(14, gSaveBlockChunks);
        if (gDamagedSaveSectors == 0)
            goto sharedTail;
        i++;
        if (i > 3)
        {
            gSaveStatusFlags[4] |= 1;
            goto sharedTail;
        }
        gDamagedSaveSectors = 0;
        goto retry1;
    }

sharedTail:
    if (gSaveStatusFlags[4] & 1)
        goto returnTrue;
    return FALSE;

returnTrue:
    return TRUE;
    case 2:
    {
        u8 i = 0;

    retry2:
        WriteSectorSignatureByteInverse(14, gSaveBlockChunks);
        if (gDamagedSaveSectors == 0)
            goto returnFalse;
        i++;
        if (i > 3)
        {
            gSaveStatusFlags[4] |= 1;
            goto returnFalse;
        }
        gDamagedSaveSectors = 0;
        goto retry2;
    }
    }

returnFalse:
    return FALSE;

}
