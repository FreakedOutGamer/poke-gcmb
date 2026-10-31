#include "global.h"
#include "gba/flash_internal.h"
#include "libpmagb/save_sector.h"
#include "target_game.h"

#define SECTOR_DATA_SIZE 0xF80

static inline void Save_ResetSaveCountersInline(void)
{
    gSaveCounter = 0;
    gLastWrittenSector = 0;
    gDamagedSaveSectors = 0;
}

// SaveBlock2 fits in sector 0 and SaveBlock1's tail spills into sector 4,
// so only those two sizes come from the descriptor. Sector 13, the last of
// the Pokémon storage, is always 2000 bytes.
s32 InitFlash(u32 timerNum, void (**timerIntr)(void))
{
    gSaveBlockChunks[0].size = gAgbPmRomParams->saveBlock2Size;
    gSaveBlockChunks[4].size = gAgbPmRomParams->saveBlock1Size % SECTOR_DATA_SIZE;
    gSaveBlockChunks[13].size = 2000;

    if (IdentifyFlash() == 0 && SetFlashTimerIntr(timerNum, timerIntr) == 0)
    {
        Save_ResetSaveCountersInline();
        gSaveFileStatus = 0x80;
        return 0;
    }

    return -1;
}

void SetSaveSectorPtrs(void)
{
    gSaveBlockChunks[0].data = gSaveBlock2Ptr;

    gSaveBlockChunks[1].data = gSaveBlock1Ptr;
    gSaveBlockChunks[2].data = gSaveBlock1Ptr + SECTOR_DATA_SIZE;
    gSaveBlockChunks[3].data = gSaveBlock1Ptr + SECTOR_DATA_SIZE * 2;
    gSaveBlockChunks[4].data = gSaveBlock1Ptr + SECTOR_DATA_SIZE * 3;

    gSaveBlockChunks[5].data = (u8 *)gPokemonStoragePtr;
    gSaveBlockChunks[6].data = (u8 *)gPokemonStoragePtr + SECTOR_DATA_SIZE;
    gSaveBlockChunks[7].data = (u8 *)gPokemonStoragePtr + SECTOR_DATA_SIZE * 2;
    gSaveBlockChunks[8].data = (u8 *)gPokemonStoragePtr + SECTOR_DATA_SIZE * 3;
    gSaveBlockChunks[9].data = (u8 *)gPokemonStoragePtr + SECTOR_DATA_SIZE * 4;
    gSaveBlockChunks[10].data = (u8 *)gPokemonStoragePtr + SECTOR_DATA_SIZE * 5;
    gSaveBlockChunks[11].data = (u8 *)gPokemonStoragePtr + SECTOR_DATA_SIZE * 6;
    gSaveBlockChunks[12].data = (u8 *)gPokemonStoragePtr + SECTOR_DATA_SIZE * 7;
    gSaveBlockChunks[13].data = (u8 *)gPokemonStoragePtr + SECTOR_DATA_SIZE * 8;
}
