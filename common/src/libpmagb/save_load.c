#include "global.h"
#include "gba/gba.h"
#include "gba/flash_internal.h"
#include "libpmagb/save_sector.h"
#include "target_game.h"

static inline u16 CalculateChecksum(void *data, s32 size)
{
    s32 i;
    u32 checksum = 0;

    for (i = 0; i < size / 4; i++)
        checksum += *((u32 *)data)++;

    return (checksum >> 16) + checksum;
}

static inline u8 ReadFlashSector(u8 sector, struct SaveSector *section)
{
    ReadFlash(sector, 0, section->data, sizeof(struct SaveSector));
    return 1;
}

u16 GetSaveValidStatus(const struct SaveBlockChunk *chunks)
{
    u16 sector;
    bool8 signatureValid;
    u16 checksum;
    u32 slot1SaveCounter = 0;
    u32 slot2SaveCounter = 0;
    u32 validSectors;
    u8 slot1Status;
    u8 slot2Status;

    validSectors = 0;
    signatureValid = FALSE;
    for (sector = 0; sector < NUM_SECTORS_PER_SAVE_SLOT; sector++)
    {
        ReadFlashSector(sector, gReadWriteSector);
        if (gReadWriteSector->signature == FILE_SIGNATURE)
        {
            signatureValid = TRUE;
            checksum = CalculateChecksum(gReadWriteSector->data,
                                          chunks[gReadWriteSector->id].size);
            if (gReadWriteSector->checksum == checksum)
            {
                slot1SaveCounter = gReadWriteSector->counter;
                validSectors |= 1 << gReadWriteSector->id;
            }
        }
    }

    if (signatureValid)
    {
        if (validSectors == 0x3FFF)
            slot1Status = SAVE_STATUS_OK;
        else
            slot1Status = SAVE_STATUS_ERROR;
    }
    else
    {
        slot1Status = SAVE_STATUS_EMPTY;
    }

    validSectors = 0;
    signatureValid = FALSE;
    for (sector = 0; sector < NUM_SECTORS_PER_SAVE_SLOT; sector++)
    {
        ReadFlashSector(NUM_SECTORS_PER_SAVE_SLOT + sector, gReadWriteSector);
        if (gReadWriteSector->signature == FILE_SIGNATURE)
        {
            signatureValid = TRUE;
            checksum = CalculateChecksum(gReadWriteSector->data,
                                          chunks[gReadWriteSector->id].size);
            if (gReadWriteSector->checksum == checksum)
            {
                slot2SaveCounter = gReadWriteSector->counter;
                validSectors |= 1 << gReadWriteSector->id;
            }
        }
    }

    if (signatureValid)
    {
        if (validSectors == 0x3FFF)
            slot2Status = SAVE_STATUS_OK;
        else
            slot2Status = SAVE_STATUS_ERROR;
    }
    else
    {
        slot2Status = SAVE_STATUS_EMPTY;
    }

    if (slot1Status == SAVE_STATUS_OK)
    {
        if (slot2Status == SAVE_STATUS_OK)
        {
            if ((slot1SaveCounter == (u32)-1 && slot2SaveCounter == 0)
             || (slot1SaveCounter == 0 && slot2SaveCounter == (u32)-1))
            {
                if ((u32)(slot1SaveCounter + 1) < (u32)(slot2SaveCounter + 1))
                    gSaveCounter = slot2SaveCounter;
                else
                    gSaveCounter = slot1SaveCounter;
            }
            else
            {
                if (slot1SaveCounter < slot2SaveCounter)
                    gSaveCounter = slot2SaveCounter;
                else
                    gSaveCounter = slot1SaveCounter;
            }
            return SAVE_STATUS_OK;
        }

        gSaveCounter = slot1SaveCounter;
        if (slot2Status == SAVE_STATUS_ERROR)
            return SAVE_STATUS_ERROR;
        return SAVE_STATUS_OK;
    }

    if (slot2Status == SAVE_STATUS_OK)
    {
        gSaveCounter = slot2SaveCounter;
        if (slot1Status == SAVE_STATUS_ERROR)
            return SAVE_STATUS_ERROR;
        return SAVE_STATUS_OK;
    }

    if (slot1Status == SAVE_STATUS_EMPTY && slot2Status == SAVE_STATUS_EMPTY)
    {
        gSaveCounter = 0;
        gLastWrittenSector = 0;
        return SAVE_STATUS_EMPTY;
    }

    gSaveCounter = 0;
    gLastWrittenSector = 0;
    return 2;
}

// Reads one sector into gSaveReadBuffer and, if its signature and checksum
// hold up, copies it into the chunk it claims to belong to. A sector that
// does not check out is simply skipped, leaving the chunk as it was.
void ReadSaveChunk(u32 sectorNum, u32 chunkId, const struct SaveBlockChunk *chunks)
{
    u16 id;
    u16 checksum;
    s32 i;

    ReadFlashSector(sectorNum, gReadWriteSector);

    id = gReadWriteSector->id;

    if (id == 0)
        gLastWrittenSector = chunkId;

    checksum = CalculateChecksum(gReadWriteSector->data, chunks[id].size);

    if (gReadWriteSector->signature == FILE_SIGNATURE
     && gReadWriteSector->checksum == checksum)
    {
        for (i = 0; i < chunks[id].size; i++)
            chunks[id].data[i] = gReadWriteSector->data[i];
    }
}

u32 ReadSaveBlockChunks(void)
{
    const struct SaveBlockChunk *chunks = gSaveBlockChunks;
    s32 i;
    u32 slot;
    u32 status;

    gReadWriteSector = gSaveReadBuffer;
    slot = (gSaveCounter & 1) * NUM_SECTORS_PER_SAVE_SLOT;

    for (i = 0; i < NUM_SECTORS_PER_SAVE_SLOT; i++)
        ReadSaveChunk(i + slot, i, chunks);

    status = gSaveFileStatus;
    CpuCopy16(gSaveBlock1Ptr, gSaveBlock1BackupPtr, gAgbPmRomParams->saveBlock1Size);

    return status;
}

u8 *ReadFirstSaveSector(void)
{
    const struct SaveBlockChunk *chunks = gSaveBlockChunkBuffer;

    gReadWriteSector = gSaveReadBuffer;
    gSaveFileStatus = GetSaveValidStatus(chunks);
    ReadSaveChunk((gSaveCounter & 1) * NUM_SECTORS_PER_SAVE_SLOT, 0, chunks);
    return (u8 *)gSaveReadBuffer;
}
