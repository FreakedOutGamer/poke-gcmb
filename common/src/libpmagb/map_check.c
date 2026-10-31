#include "global.h"
#include "target_game.h"
#include "constants/maps.h"

// Checks whether the player is in a Pokémon Center or the Battle Tower lobby
// on a Ruby/Sapphire cartridge, which is where the GameCube may link.

#ifdef PROGRAM_COLBTL
#include "data.h"
#else
// Ruby/Sapphire's SaveBlock1 up to the player's location; the rest of the
// save is read through gAgbPmRomParams's offsets.
struct SaveBlock1
{
    struct { s16 x, y; } pos;
    struct
    {
        s8 mapGroup;
        s8 mapNum;
        s8 warpId;
        s16 x, y;
    } location;
};
#endif

// colbtl and coupon keep the tables with their other data (src/maps.c,
// src/pokemon_tables.c).
#ifdef PROGRAM_POKECHAN
// Maps GetRSPlayerMapType treats as Pokémon Centers.
const u16 gRSPokemonCenterMaps[] =
{
    MAP_OLDALE_TOWN_POKEMON_CENTER_1F,
    MAP_OLDALE_TOWN_POKEMON_CENTER_2F,
    MAP_DEWFORD_TOWN_POKEMON_CENTER_1F,
    MAP_DEWFORD_TOWN_POKEMON_CENTER_2F,
    MAP_LAVARIDGE_TOWN_POKEMON_CENTER_1F,
    MAP_LAVARIDGE_TOWN_POKEMON_CENTER_2F,
    MAP_FALLARBOR_TOWN_POKEMON_CENTER_1F,
    MAP_FALLARBOR_TOWN_POKEMON_CENTER_2F,
    MAP_VERDANTURF_TOWN_POKEMON_CENTER_1F,
    MAP_VERDANTURF_TOWN_POKEMON_CENTER_2F,
    MAP_PACIFIDLOG_TOWN_POKEMON_CENTER_1F,
    MAP_PACIFIDLOG_TOWN_POKEMON_CENTER_2F,
    MAP_PETALBURG_CITY_POKEMON_CENTER_1F,
    MAP_PETALBURG_CITY_POKEMON_CENTER_2F,
    MAP_SLATEPORT_CITY_POKEMON_CENTER_1F,
    MAP_SLATEPORT_CITY_POKEMON_CENTER_2F,
    MAP_MAUVILLE_CITY_POKEMON_CENTER_1F,
    MAP_MAUVILLE_CITY_POKEMON_CENTER_2F,
    MAP_RUSTBORO_CITY_POKEMON_CENTER_1F,
    MAP_RUSTBORO_CITY_POKEMON_CENTER_2F,
    MAP_FORTREE_CITY_POKEMON_CENTER_1F,
    MAP_FORTREE_CITY_POKEMON_CENTER_2F,
    MAP_LILYCOVE_CITY_POKEMON_CENTER_1F,
    MAP_LILYCOVE_CITY_POKEMON_CENTER_2F,
    MAP_MOSSDEEP_CITY_POKEMON_CENTER_1F,
    MAP_MOSSDEEP_CITY_POKEMON_CENTER_2F,
    MAP_SOOTOPOLIS_CITY_POKEMON_CENTER_1F,
    MAP_SOOTOPOLIS_CITY_POKEMON_CENTER_2F,
    MAP_EVER_GRANDE_CITY_POKEMON_CENTER_1F,
    MAP_EVER_GRANDE_CITY_POKEMON_CENTER_2F,
    MAP_EVER_GRANDE_CITY_POKEMON_LEAGUE,
    MAP_SINGLE_BATTLE_COLOSSEUM,
    MAP_TRADE_CENTER,
    MAP_RECORD_CORNER,
    MAP_DOUBLE_BATTLE_COLOSSEUM,
    MAP_UNDEFINED,
};

// Maps GetRSPlayerMapType treats as the Battle Tower.
const u16 gRSLobbyMaps[] =
{
    MAP_BATTLE_TOWER_LOBBY,
    MAP_UNDEFINED,
};
#else
extern const u16 gRSPokemonCenterMaps[];
extern const u16 gRSLobbyMaps[];
#endif

// colbtl's callers take the result as a u8, coupon's and pokechan's as a u32.
#ifdef PROGRAM_COLBTL
u8 GetRSPlayerMapType(struct SaveBlock1 *sav1)
#else
u32 GetRSPlayerMapType(struct SaveBlock1 *sav1)
#endif
{
    s32 i;
    u16 mapGroup = sav1->location.mapGroup;
    u16 map = (mapGroup << 8) + sav1->location.mapNum;

    for (i = 0; gRSPokemonCenterMaps[i] != MAP_UNDEFINED; i++)
    {
        if (gRSPokemonCenterMaps[i] == map)
            return POKECENTER_SAVEWARP;
    }

    for (i = 0; gRSLobbyMaps[i] != MAP_UNDEFINED; i++)
    {
        if (gRSLobbyMaps[i] == map)
            return LOBBY_SAVEWARP;
    }

    return 0;
}
