#include "global.h"
#include "constants/flags.h"
#include "target_game.h"
#include "flag.h"

#define CART_GAME_CODE (*(const u32 *)0x080000AC)
#define CART_GF_ROM_HEADER ((const struct GFRomHeader *)0x08000100)
#define GAME_CODE(a, b, c, d) ((a) | ((b) << 8) | ((c) << 16) | ((d) << 24))
#define GAME_CODE_RUBY_JP GAME_CODE('A', 'X', 'V', 'J')
#define GAME_CODE_SAPPHIRE_JP GAME_CODE('A', 'X', 'P', 'J')

void DetectROM(void)
{
    u32 game = 0;
    u32 gameCode = CART_GAME_CODE;

    if (gameCode == GAME_CODE_SAPPHIRE_JP)
        goto sapphire;
    if (gameCode != GAME_CODE_RUBY_JP)
        goto other;

    game = 2;
    goto selected;

sapphire:
    game = 1;
    goto selected;

other:
    {
        gAgbPmRomParams = CART_GF_ROM_HEADER;
        gTargetIsRubySapphire = FALSE;
        return;
    }

selected:
    if (game == 2)
        gAgbPmRomParams = &gTargetGames[0];
    else
        gAgbPmRomParams = &gTargetGames[1];

    gTargetIsRubySapphire = TRUE;
}

u8 GetPlayerMapType(void)
{
    u8 value;

    if (gTargetIsRubySapphire == 0)
    {
        value = ((u8 *)gSaveBlock2Ptr)[gAgbPmRomParams->warpFlagsOffset];
    }
    else
    {
        value = GetRSPlayerMapType(gSaveBlock1Ptr);
        if (value & 1)
            value |= 0x80;
    }

    return value;
}

void UNUSED TargetGameDummy(void)
{
}

#define VERSION_FIRE_RED 4
#define VERSION_LEAF_GREEN 5

bool8 CheckIfPokedexIsObtained(void)
{
    u32 value;

    if (gTargetIsRubySapphire == 0)
        value = ((u8 *)gSaveBlock2Ptr)[gAgbPmRomParams->gcnLinkFlagsOffset] & 1;
    else
        value = FlagGet(FLAG_SYS_POKEDEX_GET);

    if (value == 0)
        return FALSE;
    else
        return TRUE;
}

u32 IsFRLG(void)
{
    u32 version = gAgbPmRomParams->version;

    if (version == VERSION_FIRE_RED || version == VERSION_LEAF_GREEN)
        return TRUE;
    else
        return FALSE;
}

bool8 CheckGameClear(void)
{
    const struct GFRomHeader *header;
    u32 flagsOffset;
    u8 *flags;
    s32 flagId;

    header = gAgbPmRomParams;
    flagsOffset = header->flagsOffset;
    // BUG: the flags are read relative to the address of gSaveBlock1Ptr
    // itself, not the save block it points to, so this reads unrelated RAM.
    flags = (u8 *)&gSaveBlock1Ptr + flagsOffset;
    flagId = header->gameClearFlag;

    return (flags[flagId / 8] >> (flagId % 8)) & 1;
}

// Ruby and Sapphire predate the Game Freak ROM header used by later Gen III
// games, so the utility carries these two Japanese-version descriptors.
#define COMMON_TARGET_FIELDS                                                    \
    .flagsOffset = 0x1220, .varsOffset = 0x1340,                         \
    .pokedexOffset = 0x18, .seen1Offset = 0x938,                      \
    .seen2Offset = 0x3A8C, .pokedexVar = 0x46,                           \
    .pokedexFlag = 0x836, .mysteryEventFlag = 0x84C,                       \
    .pokedexCount = 0x182,                                                      \
    .playerNameLength = 7, .trainerNameLength = 5, .pokemonNameLength1 = 10, .pokemonNameLength2 = 5, .moveNameLength = 7, .itemNameLength = 8, .berryNameLength = 6, .abilityNameLength = 7, .typeNameLength = 4, .mapNameLength1 = 10, .mapNameLength2 = 18, .trainerClassNameLength = 10, .decorationNameLength = 10, .dexCategoryNameLength = 5, .endOfStringLength = 1, .frontierTrainerNameLength = 3,     \
    .easyChatWordLength = 7, .saveBlock2Size = 0x890,                \
    .saveBlock1Size = 0x3AC0, .partyCountOffset = 0x234,                    \
    .partyOffset = 0x238, .warpFlagsOffset = 9,                          \
    .trainerIdOffset = 10, .playerNameOffset = 0,                        \
    .playerGenderOffset = 8, .frontierStatusOffset = 0x556,                 \
    .frontierStatusOffset2 = 0x557, .externalEventFlagsOffset = 0x312F,     \
    .externalEventDataOffset = 0x311B, .gcnLinkFlagsOffset = 0,             \
    .gameClearFlag = 0x804, .ribbonFlag = 0x83B,                            \
    .bagCountItems = 20, .bagCountKeyItems = 20, .bagCountPokeballs = 16, .bagCountTMHMs = 64, .bagCountBerries = 46, .pcItemsCount = 50,        \
    .pcItemsOffset = 0x498, .giftRibbonsOffset = 0x3110,                 \
    .enigmaBerryOffset = 0x3160, .enigmaBerrySize = 0x530

const struct GFRomHeader gTargetGames[2] =
{
    {
        .version = 2, .language = 1, .gameName = "pokemon ruby version",
        .monFrontPics = (const void *)0x081BCB60,
        .monBackPics = (const void *)0x081BE000,
        .monNormalPalettes = (const void *)0x081BEDC0,
        .monShinyPalettes = (const void *)0x081BFB80,
        .monIcons = (const void *)0x08391A98,
        .monIconPaletteIds = (const void *)0x08392178,
        .monIconPalettes = (const void *)0x08392330,
        .monSpeciesNames = (const void *)0x081CA354,
        .moveNames = (const void *)0x081CACFC,
        .decorations = (const void *)0x083C2A44,
        COMMON_TARGET_FIELDS,
        .speciesInfo = (const void *)0x081D09CC,
        .abilityNames = (const void *)0x081CBC44,
        .abilityDescriptions = (const void *)0x081CBEB4,
        .items = (const void *)0x0839A648,
        .moves = (const void *)0x081CCEE0,
        .ballGfx = (const struct CompressedSpriteSheet *)0x081DC630,
        .ballPalettes = (const struct CompressedSpritePalette *)0x081DC690,
        .moveDescriptions = (const void *)0x083923F8,
    },
    {
        .version = 1, .language = 1, .gameName = "pokemon sapphire version",
        .monFrontPics = (const void *)0x081BCAF0,
        .monBackPics = (const void *)0x081BDF90,
        .monNormalPalettes = (const void *)0x081BED50,
        .monShinyPalettes = (const void *)0x081BFB10,
        .monIcons = (const void *)0x08391A7C,
        .monIconPaletteIds = (const void *)0x0839215C,
        .monIconPalettes = (const void *)0x08392314,
        .monSpeciesNames = (const void *)0x081CA2E4,
        .moveNames = (const void *)0x081CAC8C,
        .decorations = (const void *)0x083C2A28,
        COMMON_TARGET_FIELDS,
        .speciesInfo = (const void *)0x081D095C,
        .abilityNames = (const void *)0x081CBBD4,
        .abilityDescriptions = (const void *)0x081CBE44,
        .items = (const void *)0x0839A62C,
        .moves = (const void *)0x081CCE70,
        .ballGfx = (const struct CompressedSpriteSheet *)0x081DC5C0,
        .ballPalettes = (const struct CompressedSpritePalette *)0x081DC620,
        .moveDescriptions = (const void *)0x083923DC,
    },
};
