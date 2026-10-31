#ifndef GUARD_POKEMON_DATA_H
#define GUARD_POKEMON_DATA_H

#include "pokemon.h"
#include "constants/pokemon.h"

// pokeruby's Pokémon data definitions, for src/pokemon_tables.c.

#define TARGET_SELECTED_POKEMON 0
#define TARGET_SPECIAL          (1 << 0)
#define TARGET_UNK2             (1 << 1)
#define TARGET_RANDOM           (1 << 2)
#define TARGET_BOTH_ENEMIES     (1 << 3)
#define TARGET_USER             (1 << 4)
#define TARGET_ALL_EXCEPT_USER  (1 << 5)
#define TARGET_ENEMY_SIDE       (1 << 6)

#define F_MAKES_CONTACT           (1 << 0)
#define F_AFFECTED_BY_PROTECT     (1 << 1)
#define F_AFFECTED_BY_MAGIC_COAT  (1 << 2)
#define F_AFFECTED_BY_SNATCH      (1 << 3)
#define F_MIRROR_MOVE_COMPATIBLE  (1 << 4)
#define F_AFFECTED_BY_KINGS_ROCK  (1 << 5)

enum {
    EGG_GROUP_NONE,
    EGG_GROUP_MONSTER,
    EGG_GROUP_WATER_1,
    EGG_GROUP_BUG,
    EGG_GROUP_FLYING,
    EGG_GROUP_FIELD,
    EGG_GROUP_FAIRY,
    EGG_GROUP_GRASS,
    EGG_GROUP_HUMAN_LIKE,
    EGG_GROUP_WATER_3,
    EGG_GROUP_MINERAL,
    EGG_GROUP_AMORPHOUS,
    EGG_GROUP_WATER_2,
    EGG_GROUP_DITTO,
    EGG_GROUP_DRAGON,
    EGG_GROUP_UNDISCOVERED
};

enum {
    GROWTH_MEDIUM_FAST,
    GROWTH_ERRATIC,
    GROWTH_FLUCTUATING,
    GROWTH_MEDIUM_SLOW,
    GROWTH_FAST,
    GROWTH_SLOW
};

enum {
    BODY_COLOR_RED,
    BODY_COLOR_BLUE,
    BODY_COLOR_YELLOW,
    BODY_COLOR_GREEN,
    BODY_COLOR_BLACK,
    BODY_COLOR_BROWN,
    BODY_COLOR_PURPLE,
    BODY_COLOR_GRAY,
    BODY_COLOR_WHITE,
    BODY_COLOR_PINK
};

#define LEVEL_UP_MOVE(lvl, move) ((lvl << 9) | move)

extern const struct SpeciesInfo gBaseStats[];

#endif // GUARD_POKEMON_DATA_H
