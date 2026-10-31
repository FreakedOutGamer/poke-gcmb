#include "global.h"
#include "pokemon_data.h"
#include "constants/abilities.h"
#include "constants/battle_move_effects.h"
#include "constants/global.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/species.h"

// pokeruby's Pokémon data, as in its pokemon_1.c but without the Hoenn
// order, Spinda spot, TM/HM, trainer class, cry and evolution tables.

#define min(a, b) ((a) < (b) ? (a) : (b))

#include "data/pokemon/battle_moves.h"
#include "data/pokemon/species_to_pokedex_num.h"
#include "data/pokemon/item_effects.h"

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
    {    0,  0,  0,     0,     0}  // Quirky
};

#include "data/pokemon/experience_tables.h"
#include "data/pokemon/base_stats.h"
#include "data/pokemon/level_up_learnsets.h"
#include "data/pokemon/level_up_learnset_pointers.h"

const u8 gGameVersion = VERSION_RUBY;
const u8 gGameLanguage = LANGUAGE_JAPANESE;

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
