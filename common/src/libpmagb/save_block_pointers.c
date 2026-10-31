#include "global.h"
#include "libpmagb/save_block_pointers.h"
#include "pokemon.h"
#include "libpmagb/save.h"
















// A payload that places this bss itself (SDK_EXTERN_BSS) only refers to it.
#ifdef SDK_EXTERN_BSS
extern u16 gSaveRngValue;
extern void *gLastSaveBlockPtr;
extern u8 gSaveBlockLoadRegions[];
#else
const struct GFRomHeader *gAgbPmRomParams;
u16 gSaveRngValue;
void * gSaveBlock1Ptr;
void * gSaveBlock1BackupPtr;
void * gSaveBlock2Ptr;
struct PokemonStorage * gPokemonStoragePtr;
struct Pokemon * gPlayerPartyPtr;
struct Pokemon * gPlayerPartyBackupPtr;
u8 * gPlayerPartyCountPtr;
struct Pokedex * gPokedexPtr;
u8 * gDexSeen2Ptr;
u8 * gDexSeen3Ptr;
struct ItemSlot * gPcItemsPtr;
u8 * gFlagsPtr;
u16 * gVarsPtr;
u8 * gGiftRibbonsPtr;
struct EnigmaBerry * gEnigmaBerryPtr;
#endif

static u16 SaveRandom(void);

#ifndef SDK_EXTERN_BSS
void * gLastSaveBlockPtr;
#endif

#if GAME_LANGUAGE == LANGUAGE_JAPANESE
void *GetLastSaveBlockPtr(void)
{
    return gLastSaveBlockPtr;
}
#endif
#ifndef SDK_EXTERN_BSS
u8 gSaveBlockLoadRegions[0xE000];
#endif

static inline void SeedSaveRng(void)
{
    u8 * firstSector;
    s32 sum;
    s32 i;

    firstSector = ReadFirstSaveSector();

    for (sum = 0, i = 0; i < 0x1000; i++)
    {
        sum += firstSector[i];
    }

    gLastSaveBlockPtr = gSaveBlockLoadRegions + 4 * (sum & 0xFF);
    gSaveRngValue = sum;

    // coupon.bin draws once per unit of the whole checksum, not its low
    // byte, so up to 0xFF000 draws instead of 0xFF.
#ifdef PROGRAM_COUPON
    for (i = 0; i < sum; i++)
#else
    for (i = 0; i < (sum & 0xFF); i++)
#endif
    {
        SaveRandom();
    }
}

static inline void * UpdateSaveBlockPtrInline(u32 size)
{
    void * ret = gLastSaveBlockPtr;

    gLastSaveBlockPtr = (gLastSaveBlockPtr + size) + ((SaveRandom() & 15) * 4);

    return ret;
}

static inline void SetSaveBlockPtr(void ** dest, u32 size)
{
    *dest = UpdateSaveBlockPtrInline(size);
}

static inline void SetSaveBlock1Ptr(void)
{
    void ** ptr = (void**)&gSaveBlock1Ptr;
    u32 size = gAgbPmRomParams->saveBlock1Size;

    SetSaveBlockPtr(ptr, size);
}

static inline void SetSaveBlock1BakPtr(void)
{
    void ** ptr = (void**)&gSaveBlock1BackupPtr;
    u32 size = gAgbPmRomParams->saveBlock1Size;

    SetSaveBlockPtr(ptr, size);
}

static inline void SetSaveBlock2Ptr(void)
{
    void ** ptr = (void**)&gSaveBlock2Ptr;
    u32 size = gAgbPmRomParams->saveBlock2Size;

    SetSaveBlockPtr(ptr, size);
}

static inline void SetPokemonStoragePtr(void)
{
    void ** ptr = (void**)&gPokemonStoragePtr;
    u32 size = sizeof(struct PokemonStorage);

    SetSaveBlockPtr(ptr, size);
}

void SaveBlocksInit(void)
{
    SeedSaveRng();

    switch (SaveRandom() & 3)
    {
    case 0:
        SetSaveBlock1Ptr();
        SetSaveBlock2Ptr();
        SetPokemonStoragePtr();
        SetSaveBlock1BakPtr();
        break;
    case 1:
        SetPokemonStoragePtr();
        SetSaveBlock1BakPtr();
        SetSaveBlock1Ptr();
        SetSaveBlock2Ptr();
        break;
    case 2:
        SetSaveBlock2Ptr();
        SetSaveBlock1Ptr();
        SetSaveBlock1BakPtr();
        SetPokemonStoragePtr();
        break;
    case 3:
        SetSaveBlock1BakPtr();
        SetPokemonStoragePtr();
        SetSaveBlock2Ptr();
        SetSaveBlock1Ptr();
        break;
    }

    gPlayerPartyPtr = gSaveBlock1Ptr + gAgbPmRomParams->partyOffset;
    gPlayerPartyBackupPtr = gSaveBlock1BackupPtr + gAgbPmRomParams->partyOffset;
    gPlayerPartyCountPtr = gSaveBlock1Ptr + gAgbPmRomParams->partyCountOffset;
    gPokedexPtr = gSaveBlock2Ptr + gAgbPmRomParams->pokedexOffset;
    gDexSeen2Ptr = gSaveBlock1Ptr + gAgbPmRomParams->seen1Offset;
    gDexSeen3Ptr = gSaveBlock1Ptr + gAgbPmRomParams->seen2Offset;
    gFlagsPtr = gSaveBlock1Ptr + gAgbPmRomParams->flagsOffset;
    if (GAME_LANGUAGE != LANGUAGE_JAPANESE)
        gVarsPtr = gSaveBlock1Ptr + gAgbPmRomParams->varsOffset;
    gGiftRibbonsPtr = gSaveBlock1Ptr + gAgbPmRomParams->giftRibbonsOffset;
    gEnigmaBerryPtr = gSaveBlock1Ptr + gAgbPmRomParams->enigmaBerryOffset;
    gPcItemsPtr = gSaveBlock1Ptr + gAgbPmRomParams->pcItemsOffset;
}

static u16 SaveRandom(void)
{
    gSaveRngValue = gSaveRngValue * 2061 + 7;

    return gSaveRngValue;
}

void * UNUSED UpdateSaveBlockPtr(u32 size)
{
    return UpdateSaveBlockPtrInline(size);
}

#ifdef PROGRAM_POKECHAN
u32 UNUSED GetIwramWorkAreaAddress(void)
{
    return 0x03001000;
}
#endif
