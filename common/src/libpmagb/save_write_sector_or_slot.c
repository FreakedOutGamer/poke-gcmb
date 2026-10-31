#include "global.h"
#include "libpmagb/save_sector.h"

#define WriteSingleChunkWide ((u32 (*)(u16, const struct SaveBlockChunk *))HandleWriteSector)

u32 UNUSED UnusedReadSaveData(u32 mode, const struct SaveBlockChunk *chunks)
{
    gReadWriteSector = gSaveReadBuffer;

    switch (mode)
    {
    case 0:
        gSaveFileStatus = GetSaveValidStatus(chunks);
        ReadSaveChunk((gSaveCounter & 1) * NUM_SECTORS_PER_SAVE_SLOT, 0, chunks);
        return gSaveFileStatus;
    case 1:
    {
        u32 slot = (gSaveCounter & mode) * NUM_SECTORS_PER_SAVE_SLOT;
        s32 i;

        for (i = 0; i < NUM_SECTORS_PER_SAVE_SLOT; i++)
            ReadSaveChunk(i + slot, i, chunks);

        return gSaveFileStatus;
    }
    }
}

u32 UNUSED WriteSaveSectorOrSlot(u16 chunkId, const struct SaveBlockChunk *chunks)
{
    u32 retVal;

    gReadWriteSector = gSaveReadBuffer;

    if (chunkId != 0xFFFF)
    {
        retVal = WriteSingleChunkWide(chunkId, chunks);
    }
    else
    {
        s32 i;

        gLastKnownGoodSector = gLastWrittenSector;
        gLastSaveCounter = gSaveCounter;
        gLastWrittenSector++;
        gLastWrittenSector %= NUM_SECTORS_PER_SAVE_SLOT;
        gSaveCounter++;
        retVal = SAVE_STATUS_OK;

        for (i = 0; i < NUM_SECTORS_PER_SAVE_SLOT; i++)
            WriteSingleChunkWide(i, chunks);

        if (gDamagedSaveSectors != 0)
        {
            retVal = SAVE_STATUS_ERROR;
            gLastWrittenSector = gLastKnownGoodSector;
            gSaveCounter = gLastSaveCounter;
        }
    }

    return retVal;
}
