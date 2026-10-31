#include "global.h"
#include "libpmagb/save_sector.h"
#include "target_game.h"

// Where each sector of a save slot lives in RAM: SaveBlock2, SaveBlock1 in
// four sectors, then PokemonStorage in nine. SetSaveSectorPtrs replaces
// the pointers at run time; the initial ones are leftovers that point at the
// pointer variables themselves plus each sector's offset.
struct SaveBlockChunk gSaveBlockChunks[] =
{
    {(u8 *)&gSaveBlock2Ptr, 4},
    {(u8 *)&gSaveBlock1Ptr, 0xF80},
    {(u8 *)&gSaveBlock1Ptr + 0xF80, 0xF80},
    {(u8 *)&gSaveBlock1Ptr + 0xF80 * 2, 0xF80},
    {(u8 *)&gSaveBlock1Ptr + 0xF80 * 3, 0xC40},
    {(u8 *)&gPokemonStoragePtr, 0xF80},
    {(u8 *)&gPokemonStoragePtr + 0xF80, 0xF80},
    {(u8 *)&gPokemonStoragePtr + 0xF80 * 2, 0xF80},
    {(u8 *)&gPokemonStoragePtr + 0xF80 * 3, 0xF80},
    {(u8 *)&gPokemonStoragePtr + 0xF80 * 4, 0xF80},
    {(u8 *)&gPokemonStoragePtr + 0xF80 * 5, 0xF80},
    {(u8 *)&gPokemonStoragePtr + 0xF80 * 6, 0xF80},
    {(u8 *)&gPokemonStoragePtr + 0xF80 * 7, 0xF80},
    {(u8 *)&gPokemonStoragePtr + 0xF80 * 8, 4},
};

// Same fourteen sectors, all read and written through gSaveReadBuffer.
const struct SaveBlockChunk gSaveBlockChunkBuffer[] =
{
    {(u8 *)gSaveReadBuffer, 0xF80},
    {(u8 *)gSaveReadBuffer, 0xF80},
    {(u8 *)gSaveReadBuffer, 0xF80},
    {(u8 *)gSaveReadBuffer, 0xF80},
    {(u8 *)gSaveReadBuffer, 0xF80},
    {(u8 *)gSaveReadBuffer, 0xF80},
    {(u8 *)gSaveReadBuffer, 0xF80},
    {(u8 *)gSaveReadBuffer, 0xF80},
    {(u8 *)gSaveReadBuffer, 0xF80},
    {(u8 *)gSaveReadBuffer, 0xF80},
    {(u8 *)gSaveReadBuffer, 0xF80},
    {(u8 *)gSaveReadBuffer, 0xF80},
    {(u8 *)gSaveReadBuffer, 0xF80},
    {(u8 *)gSaveReadBuffer, 0xF80},
};
