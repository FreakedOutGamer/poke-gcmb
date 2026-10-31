#ifndef GUARD_LIBPMAGB_SAVE_SECTOR_H
#define GUARD_LIBPMAGB_SAVE_SECTOR_H

#include "global.h"
#include "libpmagb/save_block_pointers.h"

#define NUM_SECTORS_PER_SAVE_SLOT 14

struct SaveSector
{
    u8 data[0xFF4];
    u16 id;
    u16 checksum;
    u32 signature;
    u32 counter;
}; // size is 0x1000

// Ruby/Sapphire's name for the part of a sector that is programmed first,
// everything up to the signature. Only its size is ever used.
struct UnkSaveSection
{
    u8 data[0xFF4];
    u16 id;
    u16 checksum;
}; // size is 0xFF8

struct SaveBlockChunk
{
    u8 *data;
    u16 size;
};

// Ruby/Sapphire keeps these in IWRAM at 0x03005xxx; this program has no
// IWRAM data section, so they live in EWRAM. Addresses are pinned in
// ld_script.ld.
extern u16 gLastWrittenSector;
extern u32 gLastSaveCounter;
extern u16 gLastKnownGoodSector;
extern u32 gDamagedSaveSectors;
extern u32 gSaveCounter;
extern struct SaveSector *gReadWriteSector;
extern struct SaveSector gSaveReadBuffer[];
extern u16 gIncrementalSectorId; // gUnknown_03005EB4 in RS

#define FILE_SIGNATURE 0x08012025

enum
{
    SECTOR_DAMAGED,
    SECTOR_OK,
    SECTOR_CHECK, // unused
};

#define SAVE_STATUS_EMPTY 0
#define SAVE_STATUS_OK    1
#define SAVE_STATUS_ERROR 0xFF

// Returns u32, not RS's u8; the caller compares the result without
// zero-extending it.
u32 WriteSaveSlot(void);

// Fourteen entries, one per sector of a slot, all pointing at gSaveReadBuffer.
extern const struct SaveBlockChunk gSaveBlockChunkBuffer[];

// The other chunk table, which says where each sector's data lives in the
// connected cartridge's save blocks. Its pointers are filled in at run time.
extern struct SaveBlockChunk gSaveBlockChunks[];


// The arena SaveBlocksInit carves the four save blocks out of, and
// the cursor tracking how much of it is spoken for. Every block is padded
// with 0-60 random bytes after it, so the layout (and, via gSaveRngValue, every
// bit of SaveRandom() after it) differs from one connected cartridge to the
// next.
extern u8 gSaveBlockLoadRegions[];
extern void *gLastSaveBlockPtr;
extern u16 gSaveRngValue;

// SaveBlock1's party, mirrored the way gSaveBlock1BackupPtr mirrors the
// whole block. See the comment on it in target_game.h.

void SaveBlocksInit(void);
void SetSaveSectorPtrs(void);
u16 SaveRandom(void);

// Returns u16, not RS's u8; the caller (ReadFirstSaveSector) narrows the result
// with an explicit 16-bit mask, not an 8-bit one.
u16 GetSaveValidStatus(const struct SaveBlockChunk *chunks);
s32 InitFlash(u32 timerNum, void (**timerIntr)(void));
bool8 WipeFailedSectors(void);

extern u8 gSaveStatusFlags[];

extern u32 gSaveFileStatus;

u8 HandleWriteSector(u16 chunkId, const struct SaveBlockChunk *chunks);
// RS's sub_81255B8, pokeemerald's HandleReplaceSector.
u8 HandleReplaceSector(u16 chunkId, const struct SaveBlockChunk *chunks);
u8 WriteSectorSignatureByte(u16 chunkCount, const struct SaveBlockChunk *chunks);
u8 WriteSectorSignatureByteInverse(u16 chunkCount, const struct SaveBlockChunk *chunks);
void ReadSaveChunk(u32 sectorNum, u32 chunkId, const struct SaveBlockChunk *chunks);
bool8 FullSaveAndLoopOverFailedSectors(void);
u32 ReadSaveBlockChunks(void);
u8 *ReadFirstSaveSector(void);

struct SaveSector * UNUSED GetSaveReadBufferPointer(void);
void Save_ResetSaveCounters(void);
void UNUSED ClearSaveData(void);
bool8 UNUSED ReadAllSaveChunks(const struct SaveBlockChunk *chunks);
u32 UNUSED UnusedReadSaveData(u32 mode, const struct SaveBlockChunk *chunks);
u32 WriteSaveSectorOrSlot(u16 chunkId, const struct SaveBlockChunk *chunks);

u32 UNUSED GetEnigmaBerryChecksum(u8 *enigmaBerry);
bool8 UNUSED IsEnigmaBerryValid(void);

#endif // GUARD_LIBPMAGB_SAVE_SECTOR_H
