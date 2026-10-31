#ifndef GUARD_ITEM_H
#define GUARD_ITEM_H

#include "global.h"

typedef void (*ItemUseFunc)(u8);

// Cartridge item-table layout from pret/pokeruby's src/item.c. Japanese
// cartridges have shorter names, so 40-byte records rather than 44.
struct Item
{
    u8 name[GAME_LANGUAGE == LANGUAGE_JAPANESE ? 10 : ITEM_NAME_LENGTH];
    u16 itemId;
    u16 price;
    u8 holdEffect;
    u8 holdEffectParam;
    const u8 *description;
    u8 importance;
    u8 exitsBagOnUse;
    u8 pocket;
    u8 type;
    ItemUseFunc fieldUseFunc;
    u8 battleUsage;
    ItemUseFunc battleUseFunc;
    u8 secondaryId;
};

#endif // GUARD_ITEM_H
