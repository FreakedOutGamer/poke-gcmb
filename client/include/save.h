#ifndef GUARD_SAVE_H
#define GUARD_SAVE_H

#include "global.h"

struct SaveSector
{
    u8 data[0xFF4];
    u16 id;
    u16 checksum;
    u32 signature;
    u32 counter;
}; // size is 0x1000

// This program keeps only the size of each chunk (pokeruby's
// SaveBlockChunk also has a data pointer, before the size).
struct SaveBlockChunk
{
    u32 size;
};

#define NUM_SECTORS_PER_SAVE_SLOT 14
#define SECTOR_DATA_SIZE 3968

#define SECTOR_ID_HOF_1 28

#define FILE_SIGNATURE 0x08012025

enum
{
    SECTOR_DAMAGED,
    SECTOR_OK,
    SECTOR_CHECK, // unused
};

#define SAVE_STATUS_EMPTY 0
#define SAVE_STATUS_OK    1
#define SAVE_STATUS_CORRUPT  2
#define SAVE_STATUS_NO_FLASH 4 // never returned here
#define SAVE_STATUS_ERROR 0xFF

extern u16 gLastWrittenSector;
extern u32 gDamagedSaveSectors;
extern u32 gSaveCounter;

// The sector every read goes through.
extern struct SaveSector gSaveSectorBuffer;
// Filled in by GetSaveValidStatus with each slot's valid-sector bits, which
// slot holds the newest save, and the sector holding that slot's first chunk.
extern u32 gSlot1ValidSectors;
extern u32 gSlot2ValidSectors;
extern u8 gCurrentSaveSlot;
extern u8 gSaveSlotFirstSector;
extern u16 gFirstSectorChecksum;
// This program splits pokeruby's SaveBlockChunk table in two. The sizes live
// in IWRAM (gSaveBlockChunks) and the data pointers in gSaveBlockChunkData. A NULL
// data pointer means the chunk is carried over unchanged when saving.
extern struct SaveBlockChunk gSaveBlockChunks[NUM_SECTORS_PER_SAVE_SLOT];
extern u8 *gSaveBlockChunkData[NUM_SECTORS_PER_SAVE_SLOT];

// pokefirered's layout, where nationalMagic is 0xB9 once the national
// dex is enabled.
struct Pokedex
{
    /*0x00*/ u8 order;
    /*0x01*/ u8 mode;
    /*0x02*/ u8 unused;
    /*0x03*/ u8 nationalMagic;
    /*0x04*/ u32 unownPersonality;
    /*0x08*/ u32 spindaPersonality;
    /*0x0C*/ u32 unknown3;
    /*0x10*/ u8 owned[52];
    /*0x44*/ u8 seen[52];
};

// pokeemerald's SaveBlock1.externalEventFlags.
struct ExternalEventFlags
{
    u8 usedBoxRS:1;
    u8 boxRSEggsUnlocked:2;
    u8 unknownFlag1;
    u8 receivedGCNJirachi:1; // shared by the Colosseum Bonus Disc and Pokémon Channel
};

// The connected game's save blocks, as loaded by CopySaveSlotData
// (gSaveBlockChunkData points into these).
extern u8 gSaveBlock2Buffer[SECTOR_DATA_SIZE];
extern u8 gSaveBlock1Buffer[SECTOR_DATA_SIZE * 4];

// gBerryFixStatus (later revision only).
#define BERRY_FIX_NOT_NEEDED 0
#define BERRY_FIX_UPDATED    1
#define BERRY_FIX_FAILED     2
#define BERRY_FIX_SAVED      3 // updated, and the save must be rewritten
#define BERRY_FIX_NOT_RUN    0xFF

// save.c
u8 ReadFlashSector(u8 sector, struct SaveSector *section);
u16 CalculateChecksum(void *data, u16 size);
bool32 CheckForFlashMemory(void);
u8 WriteSaveSectorOrSlot(const struct SaveBlockChunk *chunks);
s32 LoadGameSave(void);

// libpmagb/save_data.c
bool32 IsNationalPokedexEnabled(void);
bool32 HasHallOfFameRecord(void);
bool32 HasPokedex(void);
s32 GetPartyCount(void);

#endif // GUARD_SAVE_H
