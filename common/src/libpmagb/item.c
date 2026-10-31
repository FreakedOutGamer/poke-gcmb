#include "global.h"
#include "libpmagb/save_block_pointers.h"
#include "pokemon.h"
#include "string_util.h"
#include "target_game.h"
#include "item.h"
#include "constants/items.h"

// Item and berry names from the connected cartridge's tables.

#define BERRY_RECORD_SIZE 28

extern const u8 gBerries[];

static inline u8 *GetBerryInfoInternal(u8 berryNumber)
{
    u8 berry = berryNumber;

    if (berry == ITEM_TO_BERRY(ITEM_ENIGMA_BERRY)
     && ((u8 *)gEnigmaBerryPtr)[20] != 0
     && -((u8 *)gEnigmaBerryPtr)[10] < 0)
        return (u8 *)gEnigmaBerryPtr;

#ifdef PROGRAM_POKECHAN
    // pokechan has no table of the ordinary berries, so they get NULL too.
    // An out-of-range number goes through the u8 variable on the way, as in
    // the original.
    if ((u8)(berry - 1) >= BERRIES_COUNT)
    {
        berry = 0;
        return (void *)(u32)berry;
    }
    return NULL;
#else
    if ((u8)(berry - 1) >= BERRIES_COUNT)
        berry = 1;

    return (u8 *)gBerries + berry * BERRY_RECORD_SIZE;
#endif
}

u8 * UNUSED GetBerryInfo(u8 berry)
{
    return GetBerryInfoInternal(berry);
}

static inline u16 SanitizeItemId(u16 itemId)
{
    if (itemId >= ITEMS_COUNT)
        return ITEM_NONE;
    return itemId;
}

static inline u8 *GetItemNameInternal(u16 itemId)
{
    u8 *items = (u8 *)gAgbPmRomParams->items;
    // The table is loaded before the index is checked, as in the original.
    return items + SanitizeItemId(itemId) * sizeof(struct Item);
}

u8 * UNUSED GetItemName(u16 itemId)
{
    return GetItemNameInternal(itemId);
}

void UNUSED CopyItemName(u32 itemId, u8 *string, const u8 *berrySuffix)
{
    u16 itemId_ = itemId;

    if (itemId_ == ITEM_ENIGMA_BERRY)
    {
        StringCopy(string, GetBerryInfoInternal(ITEM_TO_BERRY(ITEM_ENIGMA_BERRY)));
        StringAppend(string, berrySuffix);
    }
    else
    {
        StringCopy(string, GetItemNameInternal(itemId_));
    }
}
