#include "global.h"
#include "gba/gba.h"
#include "gba/flash_internal.h"
#include "libpmagb/save_sector.h"
#include "target_game.h"

// The signature field's offset in struct SaveSector.
#define SECTOR_SIGNATURE_OFFSET 0xFF8

static inline u16 CalculateChecksum(void *data, s32 size)
{
    s32 i;
    u32 checksum = 0;

    for (i = 0; i < size / 4; i++)
        checksum += *((u32 *)data)++;

    return (checksum >> 16) + checksum;
}

static inline void SetDamagedSectorBits(u8 op, u8 sectorNum)
{
    switch (op)
    {
    case SECTOR_DAMAGED:
        gDamagedSaveSectors |= (1 << sectorNum);
        break;
    case SECTOR_OK:
        gDamagedSaveSectors &= ~(1 << sectorNum);
        break;
    }
}

// u32, not RS's u8: as a u8 the result gets narrowed again on the way
// out of HandleWriteSector, which the target does not do.
static inline u32 TryWriteSector(u8 sectorNum, u8 *data)
{
    if (ProgramFlashSectorAndVerify(sectorNum, data))
    {
        SetDamagedSectorBits(SECTOR_DAMAGED, sectorNum);
        return SAVE_STATUS_ERROR;
    }
    else
    {
        SetDamagedSectorBits(SECTOR_OK, sectorNum);
        return SAVE_STATUS_OK;
    }
}

u8 HandleWriteSector(u16 chunkId, const struct SaveBlockChunk *chunks)
{
    u16 i;
    u16 sectorNum;
    u8 *chunkData;
    u16 chunkSize;

    // select sector number
    sectorNum = chunkId + gLastWrittenSector;
    sectorNum %= NUM_SECTORS_PER_SAVE_SLOT;
    // select save slot
    sectorNum += NUM_SECTORS_PER_SAVE_SLOT * (gSaveCounter % 2);

    chunkData = chunks[chunkId].data;
    chunkSize = chunks[chunkId].size;

    for (i = 0; i < sizeof(struct SaveSector); i++)
        ((u8 *)gReadWriteSector)[i] = 0;

    gReadWriteSector->id = chunkId;
    gReadWriteSector->signature = FILE_SIGNATURE;
    gReadWriteSector->counter = gSaveCounter;

    for (i = 0; i < chunkSize; i++)
        gReadWriteSector->data[i] = chunkData[i];

    gReadWriteSector->checksum = CalculateChecksum(chunkData, chunkSize);

    return TryWriteSector(sectorNum, gReadWriteSector->data);
}

// Same as RS's, minus the gUnknown_03005EB4 and gDamagedSaveSectors clears,
// and taking no arguments since it never used them. It has to stay a separate
// inline. Written out into WriteSaveSlot, gSaveCounter is read once and the
// increment reuses the value instead of loading it again.
static inline void UNUSED RestoreSaveBackupVarsAndIncrement(void)
{
    gReadWriteSector = gSaveReadBuffer;
    gLastKnownGoodSector = gLastWrittenSector;
    gLastSaveCounter = gSaveCounter;
    gLastWrittenSector++;
    gLastWrittenSector %= NUM_SECTORS_PER_SAVE_SLOT;
    gSaveCounter++;
}

u32 WriteSaveSlot(void)
{
    const struct SaveBlockChunk *chunks = gSaveBlockChunks;
    u32 retVal;
    s32 i;

    RestoreSaveBackupVarsAndIncrement();
    retVal = SAVE_STATUS_OK;

    for (i = 0; i < NUM_SECTORS_PER_SAVE_SLOT; i++)
        HandleWriteSector(i, chunks);

    if (gDamagedSaveSectors)
    {
        retVal = SAVE_STATUS_ERROR;
        gLastWrittenSector = gLastKnownGoodSector;
        gSaveCounter = gLastSaveCounter;
    }

    return retVal;
}

u8 HandleReplaceSector(u16 chunkId, const struct SaveBlockChunk *chunks)
{
    u16 i;
    u16 sectorNum;
    u8 *chunkData;
    u16 chunkSize;
    u8 status;

    sectorNum = chunkId + gLastWrittenSector;
    sectorNum %= NUM_SECTORS_PER_SAVE_SLOT;
    sectorNum += NUM_SECTORS_PER_SAVE_SLOT * (gSaveCounter % 2);

    chunkData = chunks[chunkId].data;
    chunkSize = chunks[chunkId].size;

    for (i = 0; i < sizeof(struct SaveSector); i++)
        ((u8 *)gReadWriteSector)[i] = 0;

    gReadWriteSector->id = chunkId;
    gReadWriteSector->signature = FILE_SIGNATURE;
    gReadWriteSector->counter = gSaveCounter;

    for (i = 0; i < chunkSize; i++)
        gReadWriteSector->data[i] = chunkData[i];

    gReadWriteSector->checksum = CalculateChecksum(chunkData, chunkSize);

    EraseFlashSector(sectorNum);

    status = SAVE_STATUS_OK;

    for (i = 0; i < SECTOR_SIGNATURE_OFFSET; i++)
    {
        if (ProgramFlashByte(sectorNum, i, ((u8 *)gReadWriteSector)[i]))
        {
            status = SAVE_STATUS_ERROR;
            break;
        }
    }

    if (status == SAVE_STATUS_ERROR)
    {
        SetDamagedSectorBits(SECTOR_DAMAGED, sectorNum);
        return SAVE_STATUS_ERROR;
    }
    else
    {
        status = SAVE_STATUS_OK;

        for (i = 0; i < sizeof(struct SaveSector) - (SECTOR_SIGNATURE_OFFSET + 1); i++)
        {
            if (ProgramFlashByte(sectorNum, SECTOR_SIGNATURE_OFFSET + 1 + i, ((u8 *)gReadWriteSector)[SECTOR_SIGNATURE_OFFSET + 1 + i]))
            {
                status = SAVE_STATUS_ERROR;
                break;
            }
        }

        if (status == SAVE_STATUS_ERROR)
        {
            SetDamagedSectorBits(SECTOR_DAMAGED, sectorNum);
            return SAVE_STATUS_ERROR;
        }
        else
        {
            SetDamagedSectorBits(SECTOR_OK, sectorNum);
            return SAVE_STATUS_OK;
        }
    }
}
