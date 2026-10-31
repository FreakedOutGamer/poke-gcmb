#include "global.h"
#include "gba/flash_internal.h"
#include "save.h"
#include "delivery.h"
#include "main.h"
#include "text.h"

typedef void (*IntrFunc)(void);

extern IntrFunc *gSaveTimerIntrSlotPtr;

void UNUSED Save_ResetSaveCounters(void)
{
    gSaveCounter = 0;
    gLastWrittenSector = 0;
    gDamagedSaveSectors = 0;
}

u8 ReadFlashSector(u8 sector, struct SaveSector *section)
{
    ReadFlash(sector, 0, section->data, sizeof(struct SaveSector));
    return 1;
}

bool32 SetDamagedSectorBits(u8 op, u8 sectorNum)
{
    bool32 retVal = FALSE;

    switch (op)
    {
    case SECTOR_DAMAGED:
        gDamagedSaveSectors |= (1 << sectorNum);
        break;
    case SECTOR_OK:
        gDamagedSaveSectors &= ~(1 << sectorNum);
        break;
    case SECTOR_CHECK: // unused
        if (gDamagedSaveSectors & (1 << sectorNum))
            retVal = TRUE;
        break;
    }

    return retVal;
}

u8 TryWriteSector(u8 sectorNum, u8 *data)
{
    if (ProgramFlashSectorAndVerify(sectorNum, data) != 0) // is damaged?
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

u16 CalculateChecksum(void *data, u16 size)
{
    u16 i;
    u32 checksum = 0;

    for (i = 0; i < (size / 4); i++)
    {
        checksum += *((u32 *)data);
        data += sizeof(u32);
    }

    return ((checksum >> 16) + checksum);
}

bool32 CheckForFlashMemory(void)
{
    if (IdentifyFlash() == 0)
    {
        SetFlashTimerIntr(2, gSaveTimerIntrSlotPtr);
        return TRUE;
    }
    return FALSE;
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
    u32 slot1FirstSector;
    u32 slot2FirstSector;

    // check save slot 1.
    validSectors = 0;
    signatureValid = FALSE;
    for (sector = 0; sector < NUM_SECTORS_PER_SAVE_SLOT; sector++)
    {
        ReadFlashSector(sector, &gSaveSectorBuffer);
        if (gSaveSectorBuffer.id < NUM_SECTORS_PER_SAVE_SLOT)
        {
            CalculateChecksum(gSaveSectorBuffer.data, chunks[gSaveSectorBuffer.id].size);
            if (gSaveSectorBuffer.signature == FILE_SIGNATURE)
            {
                signatureValid = TRUE;
                checksum = CalculateChecksum(gSaveSectorBuffer.data, chunks[gSaveSectorBuffer.id].size);
                if (gSaveSectorBuffer.id == 0)
                {
                    slot1FirstSector = (u8)sector;
                    gFirstSectorChecksum = gSaveSectorBuffer.checksum;
                }
                if (gSaveSectorBuffer.checksum == checksum)
                {
                    slot1SaveCounter = gSaveSectorBuffer.counter;
                    validSectors |= 1 << gSaveSectorBuffer.id;
                }
            }
        }
    }

    gSlot1ValidSectors = validSectors;
    if (signatureValid)
    {
        if (validSectors == (1 << NUM_SECTORS_PER_SAVE_SLOT) - 1)
        {
            slot1Status = SAVE_STATUS_OK;
        }
        else
        {
#if REVISION == 0
            PrintHex(validSectors, 0, 17, 0, 2);
#endif
            slot1Status = SAVE_STATUS_ERROR;
        }
    }
    else
    {
        slot1Status = SAVE_STATUS_EMPTY;
    }

    // check save slot 2.
    validSectors = 0;
    signatureValid = FALSE;
    for (sector = 0; sector < NUM_SECTORS_PER_SAVE_SLOT; sector++)
    {
        ReadFlashSector(NUM_SECTORS_PER_SAVE_SLOT + sector, &gSaveSectorBuffer);
        if (gSaveSectorBuffer.signature == FILE_SIGNATURE)
        {
            signatureValid = TRUE;
            checksum = CalculateChecksum(gSaveSectorBuffer.data, chunks[gSaveSectorBuffer.id].size);
            if (gSaveSectorBuffer.id == 0)
                slot2FirstSector = (u8)sector;
            if (gSaveSectorBuffer.checksum == checksum)
            {
                slot2SaveCounter = gSaveSectorBuffer.counter;
                validSectors |= 1 << gSaveSectorBuffer.id;
            }
        }
    }

    gSlot2ValidSectors = validSectors;
    if (signatureValid)
    {
        if (validSectors == (1 << NUM_SECTORS_PER_SAVE_SLOT) - 1)
        {
            slot2Status = SAVE_STATUS_OK;
        }
        else
        {
#if REVISION == 0
            PrintHex(validSectors, 0, 18, 0, 2);
#endif
            slot2Status = SAVE_STATUS_ERROR;
        }
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
                {
                    gSaveCounter = slot2SaveCounter;
                    gCurrentSaveSlot = 1;
                    gSaveSlotFirstSector = slot2FirstSector;
                }
                else
                {
                    gSaveCounter = slot1SaveCounter;
                    gSaveSlotFirstSector = slot1FirstSector;
                    gCurrentSaveSlot = 0;
                }
            }
            else
            {
                if (slot1SaveCounter < slot2SaveCounter)
                {
                    gSaveCounter = slot2SaveCounter;
                    gCurrentSaveSlot = 1;
                    gSaveSlotFirstSector = slot2FirstSector;
                }
                else
                {
                    gSaveCounter = slot1SaveCounter;
                    gCurrentSaveSlot = 0;
                    gSaveSlotFirstSector = slot1FirstSector;
                }
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
        gSaveCounter = slot2SaveCounter;
        gCurrentSaveSlot = 1;
        gSaveSlotFirstSector = slot2FirstSector;
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
    return SAVE_STATUS_CORRUPT;
}

// Writes every chunk into the other save slot, rotating each sector one
// place along. Chunks without a data pointer are copied over from the
// current slot. Declared to return a value but never sets one.
u8 WriteSaveSectorOrSlot(const struct SaveBlockChunk *chunks)
{
    s32 i;
    u32 newSlot = gCurrentSaveSlot == 1 ? 0 : 1;

    gSaveCounter++;
    for (i = 0; i < NUM_SECTORS_PER_SAVE_SLOT; i++)
    {
        if (gSaveBlockChunkData[i] == NULL)
        {
            ReadFlashSector((i + gSaveSlotFirstSector) % NUM_SECTORS_PER_SAVE_SLOT
                                    + gCurrentSaveSlot * NUM_SECTORS_PER_SAVE_SLOT, &gSaveSectorBuffer);
            gSaveSectorBuffer.counter = gSaveCounter;
            TryWriteSector((i + gSaveSlotFirstSector + 1) % NUM_SECTORS_PER_SAVE_SLOT
                           + newSlot * NUM_SECTORS_PER_SAVE_SLOT, gSaveSectorBuffer.data);
        }
        else
        {
            DmaCopy32(3, gSaveBlockChunkData[i], &gSaveSectorBuffer, SECTOR_DATA_SIZE);
            gSaveSectorBuffer.counter = gSaveCounter;
            gSaveSectorBuffer.id = i;
            gSaveSectorBuffer.signature = FILE_SIGNATURE;
            gSaveSectorBuffer.checksum = CalculateChecksum(gSaveSectorBuffer.data, chunks[i].size);
            TryWriteSector((i + gSaveSlotFirstSector + 1) % NUM_SECTORS_PER_SAVE_SLOT
                           + newSlot * NUM_SECTORS_PER_SAVE_SLOT, gSaveSectorBuffer.data);
        }
#if REVISION == 0
        UpdateSaveIcon();
#else
        UpdateSaveIcon(20, 18);
#endif
        VBlankIntr();
    }
}

void CopySaveSlotData(void)
{
    s32 i;

    for (i = 0; i < NUM_SECTORS_PER_SAVE_SLOT; i++)
    {
        ReadFlashSector(i + gCurrentSaveSlot * NUM_SECTORS_PER_SAVE_SLOT, &gSaveSectorBuffer);
        if (gSaveBlockChunkData[gSaveSectorBuffer.id] != NULL)
            DmaCopy32(3, &gSaveSectorBuffer, gSaveBlockChunkData[gSaveSectorBuffer.id], SECTOR_DATA_SIZE);
    }
}

s32 LoadGameSave(void)
{
    u16 status = GetSaveValidStatus(gSaveBlockChunks);

    if (status == SAVE_STATUS_OK)
    {
        CopySaveSlotData();
        return SAVE_STATUS_OK;
    }
    return status;
}
