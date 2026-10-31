#ifndef GUARD_GIFT_H
#define GUARD_GIFT_H

// The Pokémon the delivery gives, in src/delivery_data.c.

struct GiftOT
{
    const u8 *name;
    u32 id;
    u8 gender;
};

struct GiftMon
{
    u8 level;
    u16 species;
    u16 moves[4];
    u16 heldItem;
    const u32 *ribbons;
    bool32 shiny;
};

struct EggTemplate
{
    u8 level;
    u16 weight;
    u16 species;
    u16 moves[4];
    u16 heldItem;
    bool32 shiny;
};

extern const struct GiftOT gGiftOT;
extern const struct EggTemplate gGiftEggs[];
extern const u16 gGiftSpecies[];

#endif // GUARD_GIFT_H
