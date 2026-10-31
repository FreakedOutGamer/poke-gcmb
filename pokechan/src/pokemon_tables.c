#include "global.h"
#include "constants/pokemon.h"

// Pokémon data tables the shared Pokémon code reads, in their ROM order
// (gBitTable at 0x02010248).

const u32 gBitTable[] =
{
    1 << 0,
    1 << 1,
    1 << 2,
    1 << 3,
    1 << 4,
    1 << 5,
    1 << 6,
    1 << 7,
    1 << 8,
    1 << 9,
    1 << 10,
    1 << 11,
    1 << 12,
    1 << 13,
    1 << 14,
    1 << 15,
    1 << 16,
    1 << 17,
    1 << 18,
    1 << 19,
    1 << 20,
    1 << 21,
    1 << 22,
    1 << 23,
    1 << 24,
    1 << 25,
    1 << 26,
    1 << 27,
    1 << 28,
    1 << 29,
    1 << 30,
    1 << 31,
};

const u8 gTypeToPaletteNumber[] =
{
    13, 13, 14, 14, 13, 13, 15, 14, 13, 15, 13, 14,
    15, 13, 14, 14, 15, 13, 13, 14, 14, 15, 13,
};

// pokeruby's gPPUpReadMasks. Masks the PP Up count of each move in ppBonuses.
const u8 gPPUpGetMask[] = {0x03, 0x0C, 0x30, 0xC0};

// MON_DATA_MARINE_RIBBON through MON_DATA_WORLD_RIBBON.
const u8 gGiftRibbonMonDataIds[] =
{
    MON_DATA_MARINE_RIBBON,
    MON_DATA_LAND_RIBBON,
    MON_DATA_SKY_RIBBON,
    MON_DATA_COUNTRY_RIBBON,
    MON_DATA_NATIONAL_RIBBON,
    MON_DATA_EARTH_RIBBON,
    MON_DATA_WORLD_RIBBON,
};
