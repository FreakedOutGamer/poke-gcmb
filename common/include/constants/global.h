#ifndef GUARD_CONSTANTS_GLOBAL_H
#define GUARD_CONSTANTS_GLOBAL_H

// pokeruby's game version and language IDs.
#define VERSION_SAPPHIRE 1
#define VERSION_RUBY 2
#define VERSION_EMERALD 3
#define VERSION_FIRE_RED 4
#define VERSION_LEAF_GREEN 5
#define VERSION_HEART_GOLD 7
#define VERSION_SOUL_SILVER 8
#define VERSION_DIAMOND 10
#define VERSION_PEARL 11
#define VERSION_PLATINUM 12
#define VERSION_GAMECUBE 15

#define LANGUAGE_JAPANESE 1
#define LANGUAGE_ENGLISH  2
#define LANGUAGE_FRENCH   3
#define LANGUAGE_ITALIAN  4
#define LANGUAGE_GERMAN   5
#define LANGUAGE_KOREAN   6 // Unused, presumably meant for Korean
#define LANGUAGE_SPANISH  7

#define MALE 0
#define FEMALE 1

// Text lengths and counts (pokeemerald's constants/global.h).
#define MOVE_NAME_LENGTH 12
#define POKEMON_NAME_LENGTH 10
#define OT_NAME_LENGTH 7
#define PLAYER_NAME_LENGTH 7
#define ABILITY_NAME_LENGTH 12
#define MAX_MON_MOVES 4
#define TRAINER_ID_LENGTH 4
#define GIFT_RIBBONS_COUNT 11

// capacities of various saveblock objects
#define DAYCARE_MON_COUNT   2
#define POKEBLOCKS_COUNT    40
#define PARTY_SIZE          6
#define EVENT_OBJECTS_COUNT 16
#define BERRY_TREES_COUNT   128
#define FLAGS_COUNT         288
#define VARS_COUNT          256
#define MAIL_COUNT          16
#define SECRET_BASES_COUNT  20
#define TV_SHOWS_COUNT      25
#define POKE_NEWS_COUNT     16
#define PC_ITEMS_COUNT      50
#define BAG_ITEMS_COUNT     20
#define BAG_KEYITEMS_COUNT  20
#define BAG_POKEBALLS_COUNT 16
#define BAG_TMHM_COUNT      64
#define BAG_BERRIES_COUNT   46
#define DECOR_MAX_SECRET_BASE 16
#define SAVED_TRENDS_COUNT 5

#define ITEM_NAME_LENGTH 14 // Includes the terminator in struct Item.

#define CONTEST_CATEGORY_COOL     0
#define CONTEST_CATEGORY_BEAUTY   1
#define CONTEST_CATEGORY_CUTE     2
#define CONTEST_CATEGORY_SMART    3
#define CONTEST_CATEGORY_TOUGH    4
#define CONTEST_CATEGORIES_COUNT  5

#define GENDER_MALE     0
#define GENDER_FEMALE   1

// SaveBlock2's specialSaveWarp flags.
#define POKECENTER_SAVEWARP          (1 << 1)
#define LOBBY_SAVEWARP               (1 << 2)
#define CHAMPION_SAVEWARP            (1 << 7)

#endif // GUARD_CONSTANTS_GLOBAL_H
