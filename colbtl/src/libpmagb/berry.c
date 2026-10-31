#include "global.h"
#include "libpmagb.h"
#include "libpmagb/item.h"
#include "constants/berry.h"
#include "constants/items.h"

typedef void (*ItemUseFunc)(u8);

struct Item
{
    u8 name[GAME_LANGUAGE == LANGUAGE_JAPANESE ? 10 : 14];
    u16 itemId;
    u16 price;
    u8 holdEffect;
    u8 holdEffectParam;
    const u8 *description;
    u8 importance;
    u8 exitsBagOnUse; // unused, but items which have this field set to 1 all
    // exit the Bag when they are used.
    u8 pocket;
    u8 type;
    ItemUseFunc fieldUseFunc;
    u8 battleUsage;
    ItemUseFunc battleUseFunc;
    u8 secondaryId;
};

u32 UNUSED GetEnigmaBerryChecksum(struct EnigmaBerry * enigmaBerry)
{
    u32 i, checksum;
    u8 * dest;

    // The description pointers could point anywhere in memory, so they are
    // cleared while the checksum is calculated and restored afterwards.
    const u8 * desc1Bak = gEnigmaBerryPtr->berry.description1;
    const u8 * desc2Bak = gEnigmaBerryPtr->berry.description2;

    gEnigmaBerryPtr->berry.description1 = NULL;
    gEnigmaBerryPtr->berry.description2 = NULL;

    dest = (u8 *)enigmaBerry;
    checksum = 0;
    for (i = 0; i < gAgbPmRomParams->enigmaBerrySize - 4; i++)
    {
        checksum += dest[i];
    }

    gEnigmaBerryPtr->berry.description1 = desc1Bak;
    gEnigmaBerryPtr->berry.description2 = desc2Bak;

    return checksum;
}

// Checks that an e-Reader Enigma Berry has sane growth parameters. The
// checksum test is commented out.
bool32 UNUSED IsEnigmaBerryValid(void)
{
    if (gEnigmaBerryPtr->berry.stageDuration == 0)
        return FALSE;
    if (gEnigmaBerryPtr->berry.maxYield == 0)
        return FALSE;
//    if (GetEnigmaBerryChecksum(gEnigmaBerryPtr) != gEnigmaBerryPtr->checksum)
//        return FALSE;
    return TRUE;
}

extern const struct Berry gBerries[];

const struct Berry * UNUSED GetBerryInfo(u8 berry)
{
    // A valid Enigma Berry's information comes from the save block.
    if (berry == ITEM_TO_BERRY(ITEM_ENIGMA_BERRY) && IsEnigmaBerryValid())
        return &gEnigmaBerryPtr->berry;
    else
    {
        // Invalid berries become Cheri Berries. An invalid Enigma Berry gets
        // the ROM's placeholder Enigma Berry information instead.
        if (berry == BERRY_NONE || berry > ITEM_TO_BERRY(LAST_BERRY_INDEX))
            berry = ITEM_TO_BERRY(FIRST_BERRY_INDEX);
        return &gBerries[berry - 1];
    }
}

static u16 SanitizeItemId(u16 itemId)
{
    if (itemId >= ITEMS_COUNT)
        return 0;
    else
        return itemId;
}

const u8 * UNUSED GetItemName(u16 itemId)
{
    const struct Item * items = gAgbPmRomParams->items;

    return items[SanitizeItemId(itemId)].name;
}

void CopyItemName(u32 itemId, u8 *string, const u8 * berryString)
{
    u16 itemId_ = itemId;

    if (itemId_ == ITEM_ENIGMA_BERRY)
    {
#if COLBTL_VERSION == VERSION_COLBTL_F || COLBTL_VERSION == VERSION_COLBTL_I || COLBTL_VERSION == VERSION_COLBTL_S
        const u8 *name = GetBerryInfo(ITEM_TO_BERRY(ITEM_ENIGMA_BERRY))->name;
        StringCopy(string, berryString);
        StringAppend(string, name);
#else
        StringCopy(string, GetBerryInfo(ITEM_TO_BERRY(ITEM_ENIGMA_BERRY))->name);
        StringAppend(string, berryString);
#endif
    }
    else
    {
        StringCopy(string, GetItemName(itemId_));
    }
}
