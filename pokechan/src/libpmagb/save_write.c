#include "global.h"
#include "gba/gba.h"
#include "gba/flash_internal.h"
#include "libpmagb/save_sector.h"

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

// Same as Ruby/Sapphire's, minus the gLastWrittenSector and gSaveCounter restore on
// failure. Reprograms the first signature byte of the last sector written
// with its own value, which only proves the sector still takes a write.
// chunks is unused, matching Ruby/Sapphire's own signature, because callers
// in src/libpmagb/save_link.c need it there.
u8 WriteSectorSignatureByte(u16 chunkCount, const struct SaveBlockChunk *chunks)
{
    u16 sector;

    sector = chunkCount + gLastWrittenSector - 1;
    sector %= NUM_SECTORS_PER_SAVE_SLOT;
    sector += NUM_SECTORS_PER_SAVE_SLOT * (gSaveCounter % 2);

    if (ProgramFlashByte(sector, sizeof(struct UnkSaveSection),
                         ((u8 *)gReadWriteSector)[sizeof(struct UnkSaveSection)]))
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

// Same as WriteSectorSignatureByte, except that it programs the complement
// of the signature byte.
// Flash programming only ever clears bits, so this leaves the last written
// sector with a signature that can no longer match and the slot is retired.
// chunks is unused, matching Ruby/Sapphire's own signature.
u8 WriteSectorSignatureByteInverse(u16 chunkCount, const struct SaveBlockChunk *chunks)
{
    u16 sector;

    sector = chunkCount + gLastWrittenSector - 1;
    sector %= NUM_SECTORS_PER_SAVE_SLOT;
    sector += NUM_SECTORS_PER_SAVE_SLOT * (gSaveCounter % 2);

    if (ProgramFlashByte(sector, sizeof(struct UnkSaveSection),
                         ~((u8 *)gReadWriteSector)[sizeof(struct UnkSaveSection)]))
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

static inline u8 UNUSED HandleWriteIncrementalSector(u16 chunkCount, const struct SaveBlockChunk *chunks)
{
    u8 retVal;

    if (gIncrementalSectorId < chunkCount - 1)
    {
        HandleWriteSector(gIncrementalSectorId, chunks);

        if (gDamagedSaveSectors)
        {
            retVal = SAVE_STATUS_ERROR;
        }
        else
        {
            retVal = SAVE_STATUS_OK;
            gIncrementalSectorId++;
        }
    }
    else
    {
        retVal = SAVE_STATUS_ERROR;
    }

    return retVal;
}

bool8 FullSaveAndLoopOverFailedSectors(void)
{
    u8 retVal;

retry:
    retVal = HandleWriteIncrementalSector(NUM_SECTORS_PER_SAVE_SLOT, gSaveBlockChunks);

    if (gDamagedSaveSectors)
    {
        if (!WipeFailedSectors())
            goto retry;

        gSaveStatusFlags[4] |= 1;
    }

    if (retVal == SAVE_STATUS_ERROR)
        return TRUE;
    else
        return FALSE;
}
