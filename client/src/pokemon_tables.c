#include "global.h"
#include "pokemon_data.h"
#include "constants/abilities.h"
#include "constants/battle_move_effects.h"
#include "constants/global.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/species.h"

// pokeruby's Pokémon data, cut down to what the gift needs, but still with
// the full move, experience and Pokédex number tables.

const s8 gNatureStatTable[][5] =
{
    // Atk Def Spd Sp.Atk Sp.Def
    {    0,  0,  0,     0,     0}, // Hardy
    {   +1, -1,  0,     0,     0}, // Lonely
    {   +1,  0, -1,     0,     0}, // Brave
    {   +1,  0,  0,    -1,     0}, // Adamant
    {   +1,  0,  0,     0,    -1}, // Naughty
    {   -1, +1,  0,     0,     0}, // Bold
    {    0,  0,  0,     0,     0}, // Docile
    {    0, +1, -1,     0,     0}, // Relaxed
    {    0, +1,  0,    -1,     0}, // Impish
    {    0, +1,  0,     0,    -1}, // Lax
    {   -1,  0, +1,     0,     0}, // Timid
    {    0, -1, +1,     0,     0}, // Hasty
    {    0,  0,  0,     0,     0}, // Serious
    {    0,  0, +1,    -1,     0}, // Jolly
    {    0,  0, +1,     0,    -1}, // Naive
    {   -1,  0,  0,    +1,     0}, // Modest
    {    0, -1,  0,    +1,     0}, // Mild
    {    0,  0, -1,    +1,     0}, // Quiet
    {    0,  0,  0,     0,     0}, // Bashful
    {    0,  0,  0,    +1,    -1}, // Rash
    {   -1,  0,  0,     0,    +1}, // Calm
    {    0, -1,  0,     0,    +1}, // Gentle
    {    0,  0, -1,     0,    +1}, // Sassy
    {    0,  0,  0,    -1,    +1}, // Careful
    {    0,  0,  0,     0,     0} // Quirky
};

#include "data/pokemon/battle_moves.h"
#include "data/pokemon/experience_tables.h"
#include "data/pokemon/species_to_pokedex_num.h"

// Only Jirachi's entry of gBaseStats.
const struct SpeciesInfo gJirachiBaseStats =
{
    .baseHP = 100,
    .baseAttack = 100,
    .baseDefense = 100,
    .baseSpeed = 100,
    .baseSpAttack = 100,
    .baseSpDefense = 100,
    .types = {TYPE_STEEL, TYPE_PSYCHIC},
    .catchRate = 3,
    .expYield = 215,
    .evYield_HP = 3,
    .evYield_Attack = 0,
    .evYield_Defense = 0,
    .evYield_Speed = 0,
    .evYield_SpAttack = 0,
    .evYield_SpDefense = 0,
    .itemCommon = ITEM_STAR_PIECE,
    .itemRare = ITEM_STAR_PIECE,
    .genderRatio = MON_GENDERLESS,
    .eggCycles = 120,
    .friendship = 100,
    .growthRate = GROWTH_SLOW,
    .eggGroups = {EGG_GROUP_UNDISCOVERED, EGG_GROUP_UNDISCOVERED},
    .abilities = {ABILITY_SERENE_GRACE, ABILITY_NONE},
    .safariZoneFleeRate = 0,
    .bodyColor = BODY_COLOR_YELLOW,
    .noFlip = FALSE,
};

const u16 gJirachiLevelUpLearnset[] =
{
    LEVEL_UP_MOVE( 1, MOVE_WISH),
    LEVEL_UP_MOVE( 1, MOVE_CONFUSION),
    LEVEL_UP_MOVE( 5, MOVE_REST),
    LEVEL_UP_MOVE(10, MOVE_SWIFT),
    LEVEL_UP_MOVE(15, MOVE_HELPING_HAND),
    LEVEL_UP_MOVE(20, MOVE_PSYCHIC),
    LEVEL_UP_MOVE(25, MOVE_REFRESH),
    LEVEL_UP_MOVE(30, MOVE_REST),
    LEVEL_UP_MOVE(35, MOVE_DOUBLE_EDGE),
    LEVEL_UP_MOVE(40, MOVE_FUTURE_SIGHT),
    LEVEL_UP_MOVE(45, MOVE_COSMIC_POWER),
    LEVEL_UP_MOVE(50, MOVE_DOOM_DESIRE),
    LEVEL_UP_END
};

#if REVISION == 1
const u8 gGameVersion = VERSION_RUBY;
#endif
const u8 gGameLanguage = LANGUAGE_ENGLISH;

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

// Masks for getting PP Up count, also PP Max values
const u8 gPPUpReadMasks[] = {0x03, 0x0c, 0x30, 0xc0};

// Masks for setting PP Up count
const u8 gPPUpWriteMasks[] = {0xFC, 0xF3, 0xCF, 0x3F};

// Values added to PP Up count
const u8 gPPUpValues[] = {0x01, 0x04, 0x10, 0x40};
