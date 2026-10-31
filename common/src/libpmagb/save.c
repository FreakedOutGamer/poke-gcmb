#include "global.h"
#include "libpmagb/save_block_pointers.h"
#include "pokemon.h"
#include "libpmagb/save.h"
#include "gba/flash_internal.h"
#include "gflib/sound.h"
#include "gflib/init.h"

#define FILE_SIGNATURE 0x08012025  // signature value to determine if a sector is in use

//#define TOTAL_FLASH_SECTORS ((ARRAY_COUNT(gSaveBlockChunks) * 2) + (ARRAY_COUNT(sHallOfFameChunks) * 2)) // there are 2 slots, so double each array count and get the sum.
#define TOTAL_FLASH_SECTORS 32

struct SaveBlockChunk
{
    u8 *data;
    u16 size;
};

struct SaveSector
{
    u8 data[0xFF4];
    u16 id;
    u16 checksum;
    u32 signature;
    u32 counter;
}; // size is 0x1000

// Each 4 KiB flash sector contains 3968 bytes of actual data followed by a 128 byte footer
#define SECTOR_DATA_SIZE 3968u
#define SECTOR_FOOTER_SIZE 128
#define SECTOR_SIGNATURE_OFFSET offsetof(struct SaveSector, signature)

#define HALL_OF_FAME_SECTOR 28

#define NUM_SECTORS_PER_SAVE_SLOT 14  // Number of sectors occupied by a save slot
#define NUM_HALL_OF_FAME_SECTORS 2

#define SAVEBLOCK_CHUNK(structure, chunkNum)                            \
{                                                                       \
    (u8 *)&structure + chunkNum * SECTOR_DATA_SIZE, SECTOR_DATA_SIZE    \
}                                                                       \

#define SAVEBLOCK_CHUNK_FALSE_SIZE(structure, chunkNum, size)           \
{                                                                       \
    (u8 *)&structure + chunkNum * SECTOR_DATA_SIZE, size                \
}                                                                       \

u16 gLastWrittenSector;
u32 gLastSaveCounter;
u16 gLastKnownGoodSector;
u32 gDamagedSaveSectors;
u32 gSaveCounter;
struct SaveSector *gReadWriteSector; // the pointer is in fast IWRAM but may sometimes point to the slower EWRAM.
// Storage remains in the version-specific EWRAM/BSS layout for now. Keeping
// this as an external symbol lets the shared code relink at each ROM's
// original address without emitting a second overlapping 4 KiB buffer.
extern struct SaveSector gSaveReadBuffer;
// These unused globals remain part of the version-specific raw BSS layout.
u16 gIncrementalSectorId;
struct {
    u8 unused1;
    u8 neverRead1;
    u8 unused2;
    u8 unused3;
    u8 failedSectorsWiped;
    u8 unused4;
    u8 unused5;
    u8 unused6;
    u32 neverRead2;
    u8 unused7;
    u8 unused8;
    u8 unused9;
    u8 unused10;
    u16 neverRead3;
} gSaveStatusFlags;

#ifdef PROGRAM_COUPON
// coupon.bin shares these with pokechan's save code (save_retry.c and
// save_write_sector_or_slot.c).
extern u8 gSaveSectorWipeAttempts;
extern u32 gSaveFileStatus;
#else
static u8 gSaveSectorWipeAttempts;
static u32 gSaveFileStatus;
#endif

static bool8 WipeFailedSectors(void);

struct SaveBlockChunk gSaveBlockChunks[] = {
    SAVEBLOCK_CHUNK_FALSE_SIZE(gSaveBlock2Ptr, 0, 0x0004),
    SAVEBLOCK_CHUNK(gSaveBlock1Ptr, 0),
    SAVEBLOCK_CHUNK(gSaveBlock1Ptr, 1),
    SAVEBLOCK_CHUNK(gSaveBlock1Ptr, 2),
    SAVEBLOCK_CHUNK_FALSE_SIZE(gSaveBlock1Ptr, 3, 0x0C40),
    SAVEBLOCK_CHUNK(gPokemonStoragePtr, 0),
    SAVEBLOCK_CHUNK(gPokemonStoragePtr, 1),
    SAVEBLOCK_CHUNK(gPokemonStoragePtr, 2),
    SAVEBLOCK_CHUNK(gPokemonStoragePtr, 3),
    SAVEBLOCK_CHUNK(gPokemonStoragePtr, 4),
    SAVEBLOCK_CHUNK(gPokemonStoragePtr, 5),
    SAVEBLOCK_CHUNK(gPokemonStoragePtr, 6),
    SAVEBLOCK_CHUNK(gPokemonStoragePtr, 7),
    SAVEBLOCK_CHUNK_FALSE_SIZE(gPokemonStoragePtr, 8, 0x0004),
};

struct SaveBlockChunk gSaveBlockChunkBuffer[] = {
    {(void *)&gSaveReadBuffer, SECTOR_DATA_SIZE},
    {(void *)&gSaveReadBuffer, SECTOR_DATA_SIZE},
    {(void *)&gSaveReadBuffer, SECTOR_DATA_SIZE},
    {(void *)&gSaveReadBuffer, SECTOR_DATA_SIZE},
    {(void *)&gSaveReadBuffer, SECTOR_DATA_SIZE},
    {(void *)&gSaveReadBuffer, SECTOR_DATA_SIZE},
    {(void *)&gSaveReadBuffer, SECTOR_DATA_SIZE},
    {(void *)&gSaveReadBuffer, SECTOR_DATA_SIZE},
    {(void *)&gSaveReadBuffer, SECTOR_DATA_SIZE},
    {(void *)&gSaveReadBuffer, SECTOR_DATA_SIZE},
    {(void *)&gSaveReadBuffer, SECTOR_DATA_SIZE},
    {(void *)&gSaveReadBuffer, SECTOR_DATA_SIZE},
    {(void *)&gSaveReadBuffer, SECTOR_DATA_SIZE},
    {(void *)&gSaveReadBuffer, SECTOR_DATA_SIZE},
};

inline void UNUSED Save_ResetSaveCounters(void)
{
    gSaveCounter = 0;
    gLastWrittenSector = 0;
    gDamagedSaveSectors = 0;
    gSaveFileStatus = 0x80;
}

inline void UNUSED ClearSaveData(void)
{
    s32 i;

    for (i = 0; i < NUM_SECTORS; i++)
    {
        EraseFlashSector(i);
    }
    Save_ResetSaveCounters();
}

s32 InitFlash(u32 timerNum, IntrFunc * intrFunc)
{
    gSaveBlockChunks[0].size = gAgbPmRomParams->saveBlock2Size;
    gSaveBlockChunks[4].size = gAgbPmRomParams->saveBlock1Size % SECTOR_DATA_SIZE;
    gSaveBlockChunks[13].size = sizeof(struct PokemonStorage) % SECTOR_DATA_SIZE;

    if (!IdentifyFlash() && !SetFlashTimerIntr(timerNum, intrFunc))
    {
        Save_ResetSaveCounters();
        return 0;
    }

    return -1;
}

void SetSaveSectorPtrs(void)
{
    gSaveBlockChunks[0].data = gSaveBlock2Ptr;
    gSaveBlockChunks[1].data = gSaveBlock1Ptr + 0 * SECTOR_DATA_SIZE;
    gSaveBlockChunks[2].data = gSaveBlock1Ptr + 1 * SECTOR_DATA_SIZE;
    gSaveBlockChunks[3].data = gSaveBlock1Ptr + 2 * SECTOR_DATA_SIZE;
    gSaveBlockChunks[4].data = gSaveBlock1Ptr + 3 * SECTOR_DATA_SIZE;
    gSaveBlockChunks[5].data = (void *)gPokemonStoragePtr + 0 * SECTOR_DATA_SIZE;
    gSaveBlockChunks[6].data = (void *)gPokemonStoragePtr + 1 * SECTOR_DATA_SIZE;
    gSaveBlockChunks[7].data = (void *)gPokemonStoragePtr + 2 * SECTOR_DATA_SIZE;
    gSaveBlockChunks[8].data = (void *)gPokemonStoragePtr + 3 * SECTOR_DATA_SIZE;
    gSaveBlockChunks[9].data = (void *)gPokemonStoragePtr + 4 * SECTOR_DATA_SIZE;
    gSaveBlockChunks[10].data = (void *)gPokemonStoragePtr + 5 * SECTOR_DATA_SIZE;
    gSaveBlockChunks[11].data = (void *)gPokemonStoragePtr + 6 * SECTOR_DATA_SIZE;
    gSaveBlockChunks[12].data = (void *)gPokemonStoragePtr + 7 * SECTOR_DATA_SIZE;
    gSaveBlockChunks[13].data = (void *)gPokemonStoragePtr + 8 * SECTOR_DATA_SIZE;
}

static inline u16 CalculateChecksum(void * data, s32 size)
{
    s32 i;
    u32 checksum = 0;

    for (i = 0; i < (size / 4); i++)
    {
        checksum += *((u32 *)data);
        data += sizeof(u32);
    }

    return ((checksum >> 16) + checksum);
}

static inline void ReadFlashSector(u8 sector, void * dest)
{
    ReadFlash(sector, 0, dest, sizeof(struct SaveSector));
}

enum
{
    SECTOR_DAMAGED,
    SECTOR_OK,
    SECTOR_CHECK, // unused
};

static inline bool32 SetDamagedSectorBits(u8 op, u8 sectorNum)
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

u16 GetSaveValidStatus(const struct SaveBlockChunk *chunks)
{
    u16 sector;
    bool8 signatureValid;
    u16 checksum;
    u32 slot1saveCounter = 0;
    u32 slot2saveCounter = 0;
    u8 slot1Status;
    u8 slot2Status;
    u32 validSectors;
    const u32 ALL_SECTORS = (1 << NUM_SECTORS_PER_SAVE_SLOT) - 1;  // bitmask of all saveblock sectors

    // check save slot 1.
    validSectors = 0;
    signatureValid = FALSE;
    for (sector = 0; sector < NUM_SECTORS_PER_SAVE_SLOT; sector++)
    {
        ReadFlashSector(sector, gReadWriteSector);
        if (gReadWriteSector->signature == FILE_SIGNATURE)
        {
            signatureValid = TRUE;
            checksum = CalculateChecksum(gReadWriteSector->data, chunks[gReadWriteSector->id].size);
            if (gReadWriteSector->checksum == checksum)
            {
                slot1saveCounter = gReadWriteSector->counter;
                validSectors |= 1 << gReadWriteSector->id;
            }
        }
    }

    if (signatureValid)
    {
        if (validSectors == ALL_SECTORS)
            slot1Status = SAVE_STATUS_OK;
        else
            slot1Status = SAVE_STATUS_ERROR;
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
        ReadFlashSector(NUM_SECTORS_PER_SAVE_SLOT + sector, gReadWriteSector);
        if (gReadWriteSector->signature == FILE_SIGNATURE)
        {
            signatureValid = TRUE;
            checksum = CalculateChecksum(gReadWriteSector->data, chunks[gReadWriteSector->id].size);
            if (gReadWriteSector->checksum == checksum)
            {
                slot2saveCounter = gReadWriteSector->counter;
                validSectors |= 1 << gReadWriteSector->id;
            }
        }
    }

    if (signatureValid)
    {
        if (validSectors == ALL_SECTORS)
            slot2Status = SAVE_STATUS_OK;
        else
            slot2Status = SAVE_STATUS_ERROR;
    }
    else
    {
        slot2Status = SAVE_STATUS_EMPTY;
    }

    if (slot1Status == SAVE_STATUS_OK && slot2Status == SAVE_STATUS_OK)
    {
        // Choose counter of the most recent save file
        if ((slot1saveCounter == -1 && slot2saveCounter == 0) || (slot1saveCounter == 0 && slot2saveCounter == -1))
        {
            if ((u32)(slot1saveCounter + 1) < (u32)(slot2saveCounter + 1))
                gSaveCounter = slot2saveCounter;
            else
                gSaveCounter = slot1saveCounter;
        }
        else
        {
            if (slot1saveCounter < slot2saveCounter)
                gSaveCounter = slot2saveCounter;
            else
                gSaveCounter = slot1saveCounter;
        }
        return SAVE_STATUS_OK;
    }

    if (slot1Status == SAVE_STATUS_OK)
    {
        gSaveCounter = slot1saveCounter;
        if (slot2Status == SAVE_STATUS_ERROR)
            return SAVE_STATUS_ERROR;
        else
            return SAVE_STATUS_OK;
    }

    if (slot2Status == SAVE_STATUS_OK)
    {
        gSaveCounter = slot2saveCounter;
        if (slot1Status == SAVE_STATUS_ERROR)
            return SAVE_STATUS_ERROR;
        else
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

void ReadSaveChunk(u32 sector, u32 chunk, const struct SaveBlockChunk * chunks)
{
    s32 i;
    u32 checksum;
    u16 sectorId;

    ReadFlashSector(sector, gReadWriteSector);
    sectorId = gReadWriteSector->id;

    if (sectorId == 0)
    {
        gLastWrittenSector = chunk;
    }

    checksum = CalculateChecksum(gReadWriteSector->data, chunks[sectorId].size);

    if (gReadWriteSector->signature == FILE_SIGNATURE && gReadWriteSector->checksum == checksum)
    {
        for (i = 0; i < chunks[sectorId].size; i++)
            chunks[sectorId].data[i] = gReadWriteSector->data[i];
    }
}

inline bool32 UNUSED ReadAllSaveChunks(const struct SaveBlockChunk * chunks)
{
    s32 i;
    u32 firstSector = (gSaveCounter % 2) * NUM_SECTORS_PER_SAVE_SLOT;

    for (i = 0; i < NUM_SECTORS_PER_SAVE_SLOT; i++)
    {
        ReadSaveChunk(i + firstSector, i, chunks);
    }
    return TRUE;
}

inline UNUSED u32 UnusedReadSaveData(s32 allChunks, const struct SaveBlockChunk *chunks)
{
    gReadWriteSector = &gSaveReadBuffer;

    switch (allChunks)
    {
    case FALSE:
        gSaveFileStatus = GetSaveValidStatus(chunks);
        ReadSaveChunk((gSaveCounter % 2) * NUM_SECTORS_PER_SAVE_SLOT, 0, chunks);
    
        return gSaveFileStatus;
    case TRUE:
        ReadAllSaveChunks(chunks);
    
        return gSaveFileStatus;
    }
}

u32 ReadSaveBlockChunks(void)
{
    u32 status;
    const struct SaveBlockChunk * chunks = gSaveBlockChunks;

    gReadWriteSector = &gSaveReadBuffer;
    ReadAllSaveChunks(chunks);
    status = gSaveFileStatus;
    CpuCopy16(gSaveBlock1Ptr, gSaveBlock1BackupPtr, gAgbPmRomParams->saveBlock1Size);

    return status;
}

u8 * ReadFirstSaveSector(void)
{
    const struct SaveBlockChunk * chunks = gSaveBlockChunkBuffer;

    gReadWriteSector = &gSaveReadBuffer;
    gSaveFileStatus = GetSaveValidStatus(chunks);
    ReadSaveChunk((gSaveCounter % 2) * NUM_SECTORS_PER_SAVE_SLOT, 0, chunks);

    return gSaveReadBuffer.data;
}

static inline u32 TryWriteSector(u8 sectorNum, void * data)
{
    u32 ret;

    SoundVSyncOff();
    ret = ProgramFlashSectorAndVerify(sectorNum, data);
    SoundVSyncOn();

    if (ret != 0)
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

static u32 HandleWriteSector(u16 sectorId, const struct SaveBlockChunk * chunks)
{
    u16 i;
    u16 sector;
    u8 * chunkData;
    u16 chunkSize;

    sector = sectorId + gLastWrittenSector;
    sector %= NUM_SECTORS_PER_SAVE_SLOT;
    sector += (gSaveCounter % 2) * NUM_SECTORS_PER_SAVE_SLOT;
    chunkData = chunks[sectorId].data;
    chunkSize = chunks[sectorId].size;

    for (i = 0; i < sizeof(struct SaveSector); i++)
    {
        ((u8 *)gReadWriteSector)[i] = 0;
    }

    gReadWriteSector->id = sectorId;
    gReadWriteSector->signature = FILE_SIGNATURE;
    gReadWriteSector->counter = gSaveCounter;

    for (i = 0; i < chunkSize; i++)
    {
        gReadWriteSector->data[i] = chunkData[i];
    }

    gReadWriteSector->checksum = CalculateChecksum(chunkData, chunkSize);

    return TryWriteSector(sector, (void *)gReadWriteSector);
}

inline u32 UNUSED WriteSaveSectorOrSlot(u16 sectorId, const struct SaveBlockChunk * chunks)
{
    u32 retVal;
    s32 i;

    gReadWriteSector = &gSaveReadBuffer;

    if (sectorId != 0xFFFF)  // write single chunk
    {
        retVal = HandleWriteSector(sectorId, chunks);
    }
    else  // write all chunks
    {
        gLastKnownGoodSector = gLastWrittenSector;
        gLastSaveCounter = gSaveCounter;
        gLastWrittenSector++;
        gLastWrittenSector %= NUM_SECTORS_PER_SAVE_SLOT;
        gSaveCounter++;
        retVal = SAVE_STATUS_OK;

        for (i = 0; i < NUM_SECTORS_PER_SAVE_SLOT; i++)
            HandleWriteSector(i, chunks);

        // Check for any bad sectors
        if (gDamagedSaveSectors != 0) // skip the damaged sector.
        {
            retVal = SAVE_STATUS_ERROR;
            gLastWrittenSector = gLastKnownGoodSector;
            gSaveCounter = gLastSaveCounter;
        }
    }

    return retVal;
}

u32 WriteSaveSlot(void)
{
    return WriteSaveSectorOrSlot(0xFFFF, gSaveBlockChunks);
}

static u8 HandleReplaceSector(u16 sectorId, const struct SaveBlockChunk * chunks)
{
    u16 i;
    u16 sector;
    u8 * chunkData;
    u16 chunkSize;
    u8 ret;

    sector = sectorId + gLastWrittenSector;
    sector %= NUM_SECTORS_PER_SAVE_SLOT;
    sector += (gSaveCounter % 2) * NUM_SECTORS_PER_SAVE_SLOT;
    chunkData = chunks[sectorId].data;
    chunkSize = chunks[sectorId].size;

    for (i = 0; i < sizeof(struct SaveSector); i++)
    {
        ((u8 *)gReadWriteSector)[i] = 0;
    }

    gReadWriteSector->id = sectorId;
    gReadWriteSector->signature = FILE_SIGNATURE;
    gReadWriteSector->counter = gSaveCounter;

    for (i = 0; i < chunkSize; i++)
    {
        gReadWriteSector->data[i] = chunkData[i];
    }

    gReadWriteSector->checksum = CalculateChecksum(chunkData, chunkSize);
    EraseFlashSector(sector);
    ret = SAVE_STATUS_OK;

    for (i = 0; i < 0xFF8; i++)
    {
        if (ProgramFlashByte(sector, i, gReadWriteSector->data[i]) != 0)
        {
            ret = SAVE_STATUS_ERROR;
            break;
        }
    }

    if (ret == SAVE_STATUS_ERROR)
    {
        SetDamagedSectorBits(SECTOR_DAMAGED, sector);
        return SAVE_STATUS_ERROR;
    }
    else
    {
        ret = SAVE_STATUS_OK;
        for (i = 0; i < 7; i++)
        {
            if (ProgramFlashByte(sector, i + 0xFF9, ((u8 *)gReadWriteSector)[i + 0xFF9]) != 0)
            {
                ret = SAVE_STATUS_ERROR;
                break;
            }
        }
        if (ret == SAVE_STATUS_ERROR)
        {
            SetDamagedSectorBits(SECTOR_DAMAGED, sector);
            return SAVE_STATUS_ERROR;
        }
        else
        {
            SetDamagedSectorBits(SECTOR_OK, sector);
            return SAVE_STATUS_OK;
        }
    }
}

static u8 WriteSectorSignatureByte(u16 sectorId, const struct SaveBlockChunk * chunks)
{
    u16 sector = sectorId + gLastWrittenSector - 1;

    sector %= NUM_SECTORS_PER_SAVE_SLOT;
    sector += (gSaveCounter % 2) * NUM_SECTORS_PER_SAVE_SLOT;
    SoundVSyncOff();

    if (ProgramFlashByte(sector, SECTOR_SIGNATURE_OFFSET, ((u8 *)gReadWriteSector)[SECTOR_SIGNATURE_OFFSET]) != 0)
    {
        SetDamagedSectorBits(SECTOR_DAMAGED, sector);
        SoundVSyncOn();
        return SAVE_STATUS_ERROR;
    }
    else
    {
        SetDamagedSectorBits(SECTOR_OK, sector);
        SoundVSyncOn();
        return SAVE_STATUS_OK;
    }
}

#ifdef PROGRAM_COUPON
// pokeemerald's WriteSectorSignatureByte. It writes the signature's first
// byte itself rather than the read buffer's, and on failure rolls the slot back
// to the last good save. coupon.bin carries it but never calls it.
u8 UNUSED WriteSectorSignatureByteOrRevert(u16 sectorId, const struct SaveBlockChunk * chunks)
{
    u16 sector = sectorId + gLastWrittenSector - 1;

    sector %= NUM_SECTORS_PER_SAVE_SLOT;
    sector += (gSaveCounter % 2) * NUM_SECTORS_PER_SAVE_SLOT;
    SoundVSyncOff();

    if (ProgramFlashByte(sector, SECTOR_SIGNATURE_OFFSET, FILE_SIGNATURE & 0xFF) != 0)
    {
        SetDamagedSectorBits(SECTOR_DAMAGED, sector);
        SoundVSyncOn();
        gLastWrittenSector = gLastKnownGoodSector;
        gSaveCounter = gLastSaveCounter;
        return SAVE_STATUS_ERROR;
    }
    else
    {
        SetDamagedSectorBits(SECTOR_OK, sector);
        SoundVSyncOn();
        return SAVE_STATUS_OK;
    }
}
#endif

static u8 WriteSectorSignatureByteInverse(u16 sectorId, const struct SaveBlockChunk * chunks)
{
    u16 sector = sectorId + gLastWrittenSector - 1;

    sector %= NUM_SECTORS_PER_SAVE_SLOT;
    sector += (gSaveCounter % 2) * NUM_SECTORS_PER_SAVE_SLOT;
    SoundVSyncOff();

    if (ProgramFlashByte(sector, SECTOR_SIGNATURE_OFFSET, ~((u8 *)gReadWriteSector)[SECTOR_SIGNATURE_OFFSET]) != 0)
    {
        SetDamagedSectorBits(SECTOR_DAMAGED, sector);
        SoundVSyncOn();
        return SAVE_STATUS_ERROR;
    }
    else
    {
        SetDamagedSectorBits(SECTOR_OK, sector);
        SoundVSyncOn();
        return SAVE_STATUS_OK;
    }
}

inline u32 UNUSED RestoreSaveBackupVarsAndIncrement(void)
{
    gReadWriteSector = &gSaveReadBuffer;
    gLastKnownGoodSector = gLastWrittenSector;
    gLastSaveCounter = gSaveCounter;
    gLastWrittenSector++;
    gLastWrittenSector %= NUM_SECTORS_PER_SAVE_SLOT;
    gSaveCounter++;
    gIncrementalSectorId = 0;
    gDamagedSaveSectors = 0;

    return 0;
}

inline UNUSED u32 RestoreSaveBackupVars(void)
{
    gReadWriteSector = &gSaveReadBuffer;
    gLastKnownGoodSector = gLastWrittenSector;
    gLastSaveCounter = gSaveCounter;
    gIncrementalSectorId = 0;
    gDamagedSaveSectors = 0;

    return 0;
}

inline UNUSED u32 RevertSaveCounters(void)
{
    gLastWrittenSector = gLastKnownGoodSector;
    gSaveCounter = gLastSaveCounter;

    return gSaveCounter;
}

inline u8 UNUSED HandleWriteIncrementalSector(u16 numSectors, const struct SaveBlockChunk * chunks)
{
    u8 status;

    if (gIncrementalSectorId < numSectors - 1)
    {
        HandleWriteSector(gIncrementalSectorId, chunks);
        if (gDamagedSaveSectors)
            status = SAVE_STATUS_ERROR;
        else
        {
            status = SAVE_STATUS_OK;
            gIncrementalSectorId++;
        }
    }
    else
    {
        status = SAVE_STATUS_ERROR;
    }

    return status;
}

inline u32 UNUSED HandleReplaceSectorAndVerify(u16 chunk, const struct SaveBlockChunk * chunks)
{
    u32 status = SAVE_STATUS_OK;

#ifdef PROGRAM_POKECHAN
    HandleReplaceSector(chunk - 1, chunks);
#else
    SoundVSyncOff();
    HandleReplaceSector(chunk - 1, chunks);
    SoundVSyncOn();
#endif

    if (gDamagedSaveSectors != 0)
        status = SAVE_STATUS_ERROR;

    return status;
}

static bool32 FullSaveAndLoopOverFailedSectors(void)
{
    u8 status;

retry:
    status = HandleWriteIncrementalSector(NUM_SECTORS_PER_SAVE_SLOT, gSaveBlockChunks);

    if (gDamagedSaveSectors != 0)
    {
        if (!WipeFailedSectors())
        {
            goto retry;
        }
        gSaveStatusFlags.failedSectorsWiped |= 1;
    }

    if (status == SAVE_STATUS_ERROR)
        return TRUE;
    else
        return FALSE;
}

bool32 WriteLinkSave(s32 a)
{
    u8 i;

    switch (a)
    {
    case 0:
    {
        u32 i;
        s32 j;
        u8 *sav2;

        gSaveStatusFlags.neverRead2 |= 1;
        gSaveStatusFlags.failedSectorsWiped &= ~(1);
        gSaveStatusFlags.neverRead2 &= ~(2 | 4);
        sav2 = gSaveBlock2Ptr;

        for (i = 0; i < gAgbPmRomParams->saveBlock2Size; i++)
        {
            if (sav2[i] != 0)
                goto _DontClearSaveData;
        }

        ClearSaveData();
        gSaveStatusFlags.neverRead2 |= 8;
        gSaveStatusFlags.neverRead1 = 4;
        gSaveStatusFlags.neverRead3 = 0;
    _DontClearSaveData:
        RestoreSaveBackupVarsAndIncrement();
        gSaveStatusFlags.neverRead1++;
        gSaveStatusFlags.neverRead3 = 0;
        DelayFrames(5);

        while (1)
        {
            if (FullSaveAndLoopOverFailedSectors() != FALSE)
                break;
            if (gSaveStatusFlags.failedSectorsWiped & 1)
                return TRUE;
        }

        gSaveSectorWipeAttempts = 0;
retryReplace:
        {
            const struct SaveBlockChunk *chunks = gSaveBlockChunks;
            HandleReplaceSectorAndVerify(14, chunks);
            if (gDamagedSaveSectors != 0)
            {
                if (WipeFailedSectors())
                {
                    gSaveStatusFlags.failedSectorsWiped |= 1;
                    goto checkFailed;
                }
                goto retryReplace;
            }
        }
        goto checkFailed;
    }
    case 1:
        i = 0;
retrySignature:
        {
            const struct SaveBlockChunk *chunks = gSaveBlockChunks;
            WriteSectorSignatureByte(14, chunks);
            if (gDamagedSaveSectors != 0)
            {
                if (++i >= 4)
                {
                    gSaveStatusFlags.failedSectorsWiped |= 1;
                    goto checkFailed;
                }
                gDamagedSaveSectors = 0;
                goto retrySignature;
            }
        }
checkFailed:
        if (gSaveStatusFlags.failedSectorsWiped & 1)
            return TRUE;
        else
            return FALSE;
#ifndef PROGRAM_COUPON
    case 2:
        i = 0;
retryInverse:
        {
            const struct SaveBlockChunk *chunks = gSaveBlockChunks;
            WriteSectorSignatureByteInverse(14, chunks);
            if (gDamagedSaveSectors != 0)
            {
                if (++i >= 4)
                {
                    gSaveStatusFlags.failedSectorsWiped |= 1;
                    return FALSE;
                }
                gDamagedSaveSectors = 0;
                goto retryInverse;
            }
        }
        return FALSE;
#endif
    }

    return FALSE;
}

static inline bool8 WipeSector_Sub(void)
{
    u16 i;
    u32 *ptr = (void *)&gSaveReadBuffer;

    for (i = 0; i < sizeof(struct SaveSector) / 4; i++, ptr++)
    {
        if (*ptr != 0)
            return TRUE;
    }

    return FALSE;
}

static bool8 WipeSector(u16 sectorNum)
{
    u16 i;
    bool8 ret;
    u16 tries = 0;

    SoundVSyncOff();

    while (tries < 130)
    {
        for (i = 0; i < sizeof(struct SaveSector); i++)
        {
            ProgramFlashByte(sectorNum, i, 0);
        }
        ReadFlash(sectorNum, 0, &gSaveReadBuffer, sizeof(struct SaveSector));
        ret = WipeSector_Sub();
        tries++;
        if (!ret)
            break;
    }

    SoundVSyncOn();

    return ret;
}

static bool8 WipeFailedSectors(void)
{
    u32 bits;
    u16 i;

    gSaveStatusFlags.neverRead2 |= 2;

    if (gDamagedSaveSectors != 0 && gSaveSectorWipeAttempts <= 2)
    {
        bits = gDamagedSaveSectors;
        // Cancels out and generates nothing, but puts bits in r6 and i in r4
        // rather than the other way round.
        bits++;
        bits--;

        for (i = 0; i < NUM_SECTORS; i++)
        {
            #ifdef NONMATCHING
            u32 currBits = 1 << i;
            #else
            register u32 currBits asm("r5") = 1 << i;
            #endif // NONMATCHING

            if (bits & (currBits))
            {
                if (!WipeSector(i))
                    bits &= ~(currBits);
            }
        }

        gDamagedSaveSectors = bits;
        gSaveSectorWipeAttempts++;

        if (bits == 0)
            return FALSE;
    }

    return TRUE;
}

static UNUSED void *GetSaveReadBufferPointer(void)
{
    return &gSaveReadBuffer;
}
