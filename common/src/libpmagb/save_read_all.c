#include "global.h"
#include "libpmagb/save_sector.h"

bool8 UNUSED ReadAllSaveChunks(const struct SaveBlockChunk *chunks)
{
    s32 i;
    u32 slot = (gSaveCounter & 1) * NUM_SECTORS_PER_SAVE_SLOT;

    for (i = 0; i < NUM_SECTORS_PER_SAVE_SLOT; i++)
        ReadSaveChunk(i + slot, i, chunks);

    return TRUE;
}
