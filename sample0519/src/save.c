#include "global.h"
#include "gba/flash_internal.h"
#include "pokemon.h"
#include "save_blocks.h"
#include "save.h"

// pokeruby's game stats (overworld.c), InitFlashTimer (main.c), most of
// load_save.c and save.c, as one file. This is the saving and loading around
// the flash sector rotation, writing and verifying.

#define FILE_SIGNATURE 0x08012025  // signature value to determine if a sector is in use
#define NUM_SECTORS 32
#define SECTOR_DATA_SIZE 3968
#define NUM_SECTORS_PER_SAVE_SLOT 14

#define SAVE_STATUS_EMPTY    0
#define SAVE_STATUS_OK       1
#define SAVE_STATUS_NO_FLASH 4
#define SAVE_STATUS_ERROR    0xFF

#define HALL_OF_FAME_SECTOR 28
#define TOTAL_FLASH_SECTORS 32
#define NUM_HALL_OF_FAME_SECTORS 2

#define GAME_STAT_ENTERED_HOF 10

enum
{
    SAVE_NORMAL,
    SAVE_LINK,
    SAVE_EREADER,
    SAVE_HALL_OF_FAME,
    SAVE_OVERWRITE_DIFFERENT_FILE,
    SAVE_HALL_OF_FAME_ERASE_BEFORE, // unused
};

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

struct UnkSaveSection
{
    u8 data[0xFF4];
    u32 signature;
}; // size is 0xFF8

static u8 WriteSaveSectorOrSlot(u16 chunkId, const struct SaveBlockChunk *chunks);
static u8 HandleWriteSector(u16, const struct SaveBlockChunk *);
static u8 HandleWriteSectorNBytes(u8 sector, u8 *data, u16 size);
static u8 TryWriteSector(u8, u8 *);
static u8 TryLoadSaveSlot(u16 sectorId, const struct SaveBlockChunk *location);
static u8 CopySaveSlotData(u16, const struct SaveBlockChunk *location);
static u8 GetSaveValidStatus(const struct SaveBlockChunk *location);
static u8 TryLoadSaveSector(u8 sector, u8 *data, u16 size);
static u8 ReadFlashSector(u8, struct SaveSector *);
static u16 CalculateChecksum(void *, u16);

extern u16 gLastWrittenSector;
extern u32 gLastSaveCounter;
extern u16 gLastKnownGoodSector;
extern u32 gDamagedSaveSectors;
extern u32 gSaveCounter;
extern struct SaveSector *gReadWriteSector;
extern u16 gSaveFileStatus;
extern bool32 gFlashMemoryPresent;
extern struct ObjectEvent gObjectEvents[OBJECT_EVENTS_COUNT];
extern IntrFunc *gFlashTimerIntrFunc;
// From pokeruby's ewram.h. The save sector buffer is the start of EWRAM.
#define gSharedMem ((u8 *)0x02000000)
#define gSaveReadBuffer (struct SaveSector *)(gSharedMem + 0x0)
#define gDecompressionBuffer (gSharedMem + 0x4000)
#define gPokemonStorage (gSharedMem + 0x8000)

#define SAVEBLOCK_CHUNK(structure, size, chunkNum)             \
{                                                             \
    (u8 *)(structure) + chunkNum * SECTOR_DATA_SIZE,          \
    min(size - chunkNum * SECTOR_DATA_SIZE, SECTOR_DATA_SIZE) \
}

#define min(a, b) ((a) < (b) ? (a) : (b))

// Unused. pokeemerald's trade.c has the same table (with one more entry).
const u32 sUnusedStructSizes[] =
{
    0x890,  // sizeof(struct SaveBlock2)
    0x3AC0, // sizeof(struct SaveBlock1)
    0x1C,   // sizeof(struct MapHeader)
    0x530,
    0x24,   // sizeof(struct MailStruct)
    0x64,   // sizeof(struct Pokemon)
};

static const struct SaveBlockChunk gSaveBlockChunks[] =
{
    SAVEBLOCK_CHUNK(&gSaveBlock2, 0x890, 0),

    SAVEBLOCK_CHUNK(&gSaveBlock1, 0x3AC0, 0),
    SAVEBLOCK_CHUNK(&gSaveBlock1, 0x3AC0, 1),
    SAVEBLOCK_CHUNK(&gSaveBlock1, 0x3AC0, 2),
    SAVEBLOCK_CHUNK(&gSaveBlock1, 0x3AC0, 3),

    SAVEBLOCK_CHUNK(gPokemonStorage, 0x83D0, 0),
    SAVEBLOCK_CHUNK(gPokemonStorage, 0x83D0, 1),
    SAVEBLOCK_CHUNK(gPokemonStorage, 0x83D0, 2),
    SAVEBLOCK_CHUNK(gPokemonStorage, 0x83D0, 3),
    SAVEBLOCK_CHUNK(gPokemonStorage, 0x83D0, 4),
    SAVEBLOCK_CHUNK(gPokemonStorage, 0x83D0, 5),
    SAVEBLOCK_CHUNK(gPokemonStorage, 0x83D0, 6),
    SAVEBLOCK_CHUNK(gPokemonStorage, 0x83D0, 7),
    SAVEBLOCK_CHUNK(gPokemonStorage, 0x83D0, 8),
};

static const struct SaveBlockChunk sHallOfFameChunks[] =
{
    SAVEBLOCK_CHUNK(gDecompressionBuffer, 2 * SECTOR_DATA_SIZE, 0),
    SAVEBLOCK_CHUNK(gDecompressionBuffer, 2 * SECTOR_DATA_SIZE, 1),
};

const u8 sUnusedFlashSectors[] = { 30, 31 };

void UNUSED ResetGameStats(void)
{
    s32 i;

    for (i = 0; i < NUM_GAME_STATS; i++)
        gSaveBlock1.gameStats[i] = 0;
}

void IncrementGameStat(u8 index)
{
    if (index < NUM_GAME_STATS)
    {
        if (gSaveBlock1.gameStats[index] < 0xFFFFFF)
            gSaveBlock1.gameStats[index]++;
        else
            gSaveBlock1.gameStats[index] = 0xFFFFFF;
    }
}

u32 GetGameStat(u8 stat)
{
    if (stat >= NUM_GAME_STATS)
        return 0;

    return gSaveBlock1.gameStats[stat];
}

void UNUSED SetGameStat(u8 stat, u32 value)
{
    if (stat < NUM_GAME_STATS)
        gSaveBlock1.gameStats[stat] = value;
}

void InitFlashTimer(void)
{
    SetFlashTimerIntr(2, gFlashTimerIntrFunc);
}

void SetContinueGameWarpToDynamicWarp()
{
    gSaveBlock1.warp1 = gSaveBlock1.dynamicWarp;
}

void CheckForFlashMemory(void)
{
    if (!IdentifyFlash())
    {
        gFlashMemoryPresent = TRUE;
        InitFlashTimer();
    }
    else
        gFlashMemoryPresent = FALSE;
}

bool32 UNUSED UseContinueGameWarp(void)
{
    return gSaveBlock2.specialSaveWarp;
}

void UNUSED ClearContinueGameWarpStatus(void)
{
    gSaveBlock2.specialSaveWarp = 0;
}

void UNUSED SetContinueGameWarpStatus(void)
{
    gSaveBlock2.specialSaveWarp = 1;
}

void UNUSED SetContinueGameWarpStatusToDynamicWarp(void)
{
    SetContinueGameWarpToDynamicWarp(0);
    gSaveBlock2.specialSaveWarp = 1;
}

void UNUSED ClearContinueGameWarpStatus2(void) // duplicate function
{
    gSaveBlock2.specialSaveWarp = 0;
}

void SavePlayerParty(void)
{
    int i;

    gSaveBlock1.playerPartyCount = gPlayerPartyCount;

    for (i = 0; i < PARTY_SIZE; i++)
        gSaveBlock1.playerParty[i] = gPlayerParty[i];
}

void LoadPlayerParty(void)
{
    int i;

    gPlayerPartyCount = gSaveBlock1.playerPartyCount;

    for (i = 0; i < PARTY_SIZE; i++)
        gPlayerParty[i] = gSaveBlock1.playerParty[i];
}

static void SaveObjectEvents(void)
{
    int i;

    for (i = 0; i < OBJECT_EVENTS_COUNT; i++)
        gSaveBlock1.objectEvents[i] = gObjectEvents[i];
}

static void LoadObjectEvents(void)
{
    int i;

    for (i = 0; i < OBJECT_EVENTS_COUNT; i++)
        gObjectEvents[i] = gSaveBlock1.objectEvents[i];
}

void CopyPartyAndObjectsToSave(void)
{
    SavePlayerParty();
    SaveObjectEvents();
}

void CopyPartyAndObjectsFromSave(void)
{
    LoadPlayerParty();
    LoadObjectEvents();
}

u8 HandleSavingData(u8 saveType)
{
    u8 i;

    switch (saveType)
    {
    case SAVE_HALL_OF_FAME_ERASE_BEFORE: // wipes all hall of fame data, then saves hall of fame. unused
        for (i = HALL_OF_FAME_SECTOR; i < TOTAL_FLASH_SECTORS; i++)
            EraseFlashSector(i);
        // fall through
    case SAVE_HALL_OF_FAME: // hall of fame.
        if (GetGameStat(GAME_STAT_ENTERED_HOF) < 999)
            IncrementGameStat(GAME_STAT_ENTERED_HOF);
        for (i = 0; i < NUM_HALL_OF_FAME_SECTORS; i++)
            HandleWriteSectorNBytes(HALL_OF_FAME_SECTOR + i, sHallOfFameChunks[i].data, sHallOfFameChunks[i].size);
        CopyPartyAndObjectsToSave();
        WriteSaveSectorOrSlot(0xFFFF, gSaveBlockChunks);
        break;
    case SAVE_NORMAL: // Also used when overwriting an existing save
    default:
        CopyPartyAndObjectsToSave();
        WriteSaveSectorOrSlot(0xFFFF, gSaveBlockChunks);
        break;
    case SAVE_LINK: // link save. updates only gSaveBlock1 and gSaveBlock2.
        CopyPartyAndObjectsToSave();
        for (i = 0; i < 5; i++)
            WriteSaveSectorOrSlot(i, gSaveBlockChunks);
        break;
    case SAVE_EREADER: // used in mossdeep "game corner" before/after battling old man e-reader trainer
        CopyPartyAndObjectsToSave();
        WriteSaveSectorOrSlot(0, gSaveBlockChunks);
        break;
    case SAVE_OVERWRITE_DIFFERENT_FILE: // there is a different file, so overwrite it completely.
        // Erase Hall of Fame.
        for (i = HALL_OF_FAME_SECTOR; i < TOTAL_FLASH_SECTORS; i++)
            EraseFlashSector(i);
        CopyPartyAndObjectsToSave();
        WriteSaveSectorOrSlot(0xFFFF, gSaveBlockChunks);
        break;
    }
    return 0;
}

u8 TrySavingData(u8 saveType) // TrySave
{
    if (gFlashMemoryPresent != TRUE)
        return SAVE_STATUS_ERROR;

    HandleSavingData(saveType);
    if (!gDamagedSaveSectors)
        return SAVE_STATUS_OK;

    return SAVE_STATUS_ERROR;
}

u8 LoadGameSave(u8 saveType)
{
    u8 result;

    if (gFlashMemoryPresent != TRUE)
    {
        gSaveFileStatus = SAVE_STATUS_NO_FLASH;
        return SAVE_STATUS_ERROR;
    }

    switch (saveType)
    {
    case SAVE_NORMAL:
    default:
        result = TryLoadSaveSlot(0xFFFF, gSaveBlockChunks);
        CopyPartyAndObjectsFromSave();
        gSaveFileStatus = result;
        break;
    case SAVE_HALL_OF_FAME:
        result = TryLoadSaveSector(HALL_OF_FAME_SECTOR, sHallOfFameChunks[0].data, sHallOfFameChunks[0].size);
        if (result == SAVE_STATUS_OK)
            result = TryLoadSaveSector(HALL_OF_FAME_SECTOR + 1, sHallOfFameChunks[1].data, sHallOfFameChunks[1].size);
        break;
    }

    return result;
}

bool8 UNUSED TryReadUnusedSaveSector(struct UnkSaveSection *section)
{
    u16 i;
    char *raw = (char *)section;

    for (i = 0; i < sizeof(struct SaveSector); i++)
        raw[i] = 0;

    ReadFlash(sUnusedFlashSectors[0], 0, section->data, 4096);

    if (section->signature != FILE_SIGNATURE)
        return FALSE;

    return TRUE;
}

u8 UNUSED SaveAndEraseUnusedSaveSectors(void)
{
    u16 i;
    u8 status = TrySavingData(SAVE_NORMAL);

    for (i = 0; i < 2; i++)
        EraseFlashSector(sUnusedFlashSectors[i]);

    if (status == SAVE_STATUS_ERROR)
    {
        return 3;
    }
    else if (status == 3)
    {
        return 2;
    }
    else
    {
        LoadGameSave(SAVE_NORMAL);
        return 1;
    }
}

void UNUSED ClearSaveData(void)
{
    u16 i;

    for (i = 0; i < NUM_SECTORS; i++)
        EraseFlashSector(i);
}

void Save_ResetSaveCounters(void)
{
    gSaveCounter = 0;
    gLastWrittenSector = 0;
    gDamagedSaveSectors = 0;
}

enum
{
    SECTOR_DAMAGED,
    SECTOR_OK,
    SECTOR_CHECK, // unused
};

static bool32 SetDamagedSectorBits(u8 op, u8 sectorNum)
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

// If chunkId is 0xFFFF, this function will write all of the chunks pointed to by 'chunks'.
// Otherwise, it will write a single chunk with the given 'chunkId'.
static u8 WriteSaveSectorOrSlot(u16 chunkId, const struct SaveBlockChunk *chunks)
{
    u32 retVal;
    u16 i;

    gReadWriteSector = gSaveReadBuffer;

    if (chunkId != 0xFFFF)  // write single chunk
    {
        retVal = HandleWriteSector(chunkId, chunks);
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

static u8 HandleWriteSector(u16 chunkId, const struct SaveBlockChunk *chunks)
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

    // clear save section.
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

static u8 HandleWriteSectorNBytes(u8 sectorNum, u8 *data, u16 size)
{
    u16 i;
    struct SaveSector *section = gSaveReadBuffer;

    for (i = 0; i < sizeof(struct SaveSector); i++)
        ((char *)section)[i] = 0;

    section->signature = FILE_SIGNATURE;
    for (i = 0; i < size; i++)
        section->data[i] = data[i];
    section->id = CalculateChecksum(data, size); // though this appears to be incorrect, it might be some sector checksum instead of a whole save checksum and only appears to be relevent to HOF data, if used.

    return TryWriteSector(sectorNum, section->data);
}

static u8 TryWriteSector(u8 sectorNum, u8 *data)
{
    if (ProgramFlashSectorAndVerify(sectorNum, data) != 0) // is damaged?
    {
        SetDamagedSectorBits(SECTOR_DAMAGED, sectorNum); // set damaged sector bits.
        return SAVE_STATUS_ERROR;
    }
    else
    {
        SetDamagedSectorBits(SECTOR_OK, sectorNum); // unset damaged sector bits. it's safe now.
        return SAVE_STATUS_OK;
    }
}

static u8 TryLoadSaveSlot(u16 sectorId, const struct SaveBlockChunk *chunk)
{
    u8 retVal;
    gReadWriteSector = gSaveReadBuffer;
    if (sectorId != 0xFFFF)
    {
        retVal = SAVE_STATUS_ERROR;
    }
    else
    {
        retVal = GetSaveValidStatus(chunk);
        CopySaveSlotData(0xFFFF, chunk);
    }

    return retVal;
}

static u8 CopySaveSlotData(u16 sectorId, const struct SaveBlockChunk *chunks)
{
    u16 i;
    u16 checksum;
    u16 sector = NUM_SECTORS_PER_SAVE_SLOT * (gSaveCounter % 2);
    u16 id;

    for (i = 0; i < NUM_SECTORS_PER_SAVE_SLOT; i++)
    {
        ReadFlashSector(i + sector, gReadWriteSector);
        id = gReadWriteSector->id;
        if (id == 0)
            gLastWrittenSector = i;
        checksum = CalculateChecksum(gReadWriteSector->data, chunks[id].size);
        if (gReadWriteSector->signature == FILE_SIGNATURE
         && gReadWriteSector->checksum == checksum)
        {
            u16 j;
            for (j = 0; j < chunks[id].size; j++)
                chunks[id].data[j] = gReadWriteSector->data[j];
        }
    }

    return 1;
}

static u8 GetSaveValidStatus(const struct SaveBlockChunk *chunks)
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
            if ((unsigned)(slot1saveCounter + 1) < (unsigned)(slot2saveCounter + 1))
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

static u8 TryLoadSaveSector(u8 sector, u8 *data, u16 size)
{
    u16 i;
    struct SaveSector *section = gSaveReadBuffer;

    ReadFlashSector(sector, section);
    if (section->signature == FILE_SIGNATURE)
    {
        u16 checksum = CalculateChecksum(section->data, size);
        if (section->id == checksum)
        {
            for (i = 0; i < size; i++)
                data[i] = section->data[i];
            return SAVE_STATUS_OK;
        }
        else
        {
            return 2;
        }
    }
    else
    {
        return SAVE_STATUS_EMPTY;
    }
}

static u8 ReadFlashSector(u8 sector, struct SaveSector *section)
{
    ReadFlash(sector, 0, section->data, sizeof(struct SaveSector));
    return 1;
}

static u16 CalculateChecksum(void *data, u16 size)
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
