#include "global.h"
#include "constants/flags.h"
#include "save.h"
#include "target_game.h"


// Only the first of pokefirered's three checks: SaveBlock2's
// pokedex.nationalMagic, which FireRed/LeafGreen set to 0xB9.
bool32 IsNationalPokedexEnabled(void)
{
    if (((struct Pokedex *)&gSaveBlock2Buffer[gPokedexOffset])->nationalMagic != 0xB9)
        return FALSE;
    return TRUE;
}

// Returns whether the Hall of Fame sector is valid and holds at least one entry.
// Like every special sector, it stores its checksum in the id field.
bool32 HasHallOfFameRecord(void)
{
    ReadFlashSector(SECTOR_ID_HOF_1, &gSaveSectorBuffer);
    if (gSaveSectorBuffer.signature != FILE_SIGNATURE
     || CalculateChecksum(gSaveSectorBuffer.data, sizeof(gSaveSectorBuffer.data)) != gSaveSectorBuffer.id
     || (*(u16 *)&gSaveSectorBuffer.data[8] & 0x1FF) == 0) // first mon's species
        return FALSE;
    return TRUE;
}

// Checks FLAG_SYS_POKEDEX_GET (0x801).
// NOTE: This hard-codes Ruby/Sapphire's event flag offset (0x1220) whatever
// the connected game, unlike the offsets InitSaveBlockChunks reads from a
// GFRomHeader.
bool32 HasPokedex(void)
{
    if (gSaveBlock1Buffer[0x1220 + 0x801 / 8] & (1 << (0x801 % 8)))
        return TRUE;
    return FALSE;
}

s32 GetPartyCount(void)
{
    return gSaveBlock1Buffer[gPartyCountOffset];
}
