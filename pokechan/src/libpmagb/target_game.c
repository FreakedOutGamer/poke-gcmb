#include "global.h"
#include "constants/flags.h"
#include "target_game.h"
#include "flag.h"

// Works out which game is in the slot and where its descriptor is. The
// descriptor is Game Freak's ROM header layout (see include/target_game.h).
// Emerald, FireRed and LeafGreen carry one at 0x08000100, so
// DetectROM starts there and only swaps in a built-in copy for Ruby
// and Sapphire, which have none.
//  - Game code is read from the cartridge header at 0x080000AC.
//  - English RS carts use the software version at 0x080000BC to tell rev0
//    from later revisions. Rev1 and rev2 share a copy because their data
//    tables are at the same addresses.
//  - Japanese RS carts get a copy but return FALSE.
//  - German, French, Italian and Spanish RS carts count as Ruby/Sapphire but
//    keep 0x08000100, where their ROMs have no header.
//  - Any other game code clears gTargetIsRubySapphire and uses its own
//    header. That is how Emerald, FireRed and LeafGreen work.
//
// GetPlayerMapType and CheckIfPokedexIsObtained take save offsets from the header for
// the later games and use RS-specific logic for Ruby and Sapphire.
// CheckGameClear gets the same choice backwards.

// Built-in descriptors for Ruby and Sapphire, which have no Game Freak ROM
// header of their own. Pointers are addresses in the target cartridge;
// the names beside them come from a pokeruby build of that version and
// revision (pokeruby does not build the Japanese ROMs).
const struct GFRomHeader gTargetGames[6] =
{
    // Ruby (Japanese)
    {
        .version = 2,
        .language = 1,
        .gameName = "pokemon ruby version",
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
        .flagsOffset = 0x1220, // SaveBlock1.flags
        .varsOffset = 0x1340, // SaveBlock1.vars
        .pokedexOffset = 0x18, // SaveBlock2.pokedex
        .seen1Offset = 0x938, // SaveBlock1.dexSeen2
        .seen2Offset = 0x3A8C, // SaveBlock1.dexSeen3
        .pokedexVar = 0x46,
        .pokedexFlag = 0x836, // FLAG_SYS_NATIONAL_DEX
        .mysteryEventFlag = 0x84C, // FLAG_SYS_EXDATA_ENABLE
        .pokedexCount = 0x182,
        .playerNameLength = 7, .trainerNameLength = 10, .pokemonNameLength1 = 10, .pokemonNameLength2 = 10, .moveNameLength = 12, .itemNameLength = 12, .berryNameLength = 6, .abilityNameLength = 12, .typeNameLength = 6, .mapNameLength1 = 16, .mapNameLength2 = 18, .trainerClassNameLength = 12, .decorationNameLength = 15, .dexCategoryNameLength = 11, .endOfStringLength = 1, .frontierTrainerNameLength = 8,
        .easyChatWordLength = 0xC,
        .saveBlock2Size = 0x890, // sizeof(SaveBlock2)
        .saveBlock1Size = 0x3AC0, // sizeof(SaveBlock1)
        .partyCountOffset = 0x234, // SaveBlock1.playerPartyCount
        .partyOffset = 0x238, // SaveBlock1.playerParty
        .warpFlagsOffset = 0x9, // SaveBlock2.specialSaveWarp
        .trainerIdOffset = 0xA, // SaveBlock2.playerTrainerId
        .playerNameOffset = 0x0, // SaveBlock2.playerName
        .playerGenderOffset = 0x8, // SaveBlock2.playerGender
        .frontierStatusOffset = 0x556,
        .frontierStatusOffset2 = 0x557,
        .externalEventFlagsOffset = 0x312F, // SaveBlock1.externalEventFlags
        .externalEventDataOffset = 0x311B, // SaveBlock1.externalEventData
        .speciesInfo = (const void *)0x081D09CC,
        .abilityNames = (const void *)0x081CBC44,
        .abilityDescriptions = (const void *)0x081CBEB4,
        .items = (const void *)0x0839A648,
        .moves = (const void *)0x081CCEE0,
        .ballGfx = (const struct CompressedSpriteSheet *)0x081DC630,
        .ballPalettes = (const struct CompressedSpritePalette *)0x081DC690,
        .gcnLinkFlagsOffset = 0x0,
        .gameClearFlag = 0x804, // FLAG_SYS_GAME_CLEAR
        .ribbonFlag = 0x83B, // FLAG_SYS_RIBBON_GET
        .bagCountItems = 20, .bagCountKeyItems = 20, .bagCountPokeballs = 16, .bagCountTMHMs = 64,
        .bagCountBerries = 46,
        .pcItemsCount = 50,
        .pcItemsOffset = 0x498, // SaveBlock1.pcItems
        .giftRibbonsOffset = 0x3110, // SaveBlock1.giftRibbons
        .enigmaBerryOffset = 0x3160, // SaveBlock1.enigmaBerry
        .enigmaBerrySize = 0x530,
        .moveDescriptions = (const void *)0x083923F8,
    },
    // Sapphire (Japanese)
    {
        .version = 1,
        .language = 1,
        .gameName = "pokemon sapphire version",
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
        .flagsOffset = 0x1220, // SaveBlock1.flags
        .varsOffset = 0x1340, // SaveBlock1.vars
        .pokedexOffset = 0x18, // SaveBlock2.pokedex
        .seen1Offset = 0x938, // SaveBlock1.dexSeen2
        .seen2Offset = 0x3A8C, // SaveBlock1.dexSeen3
        .pokedexVar = 0x46,
        .pokedexFlag = 0x836, // FLAG_SYS_NATIONAL_DEX
        .mysteryEventFlag = 0x84C, // FLAG_SYS_EXDATA_ENABLE
        .pokedexCount = 0x182,
        .playerNameLength = 7, .trainerNameLength = 10, .pokemonNameLength1 = 10, .pokemonNameLength2 = 10, .moveNameLength = 12, .itemNameLength = 12, .berryNameLength = 6, .abilityNameLength = 12, .typeNameLength = 6, .mapNameLength1 = 16, .mapNameLength2 = 18, .trainerClassNameLength = 12, .decorationNameLength = 15, .dexCategoryNameLength = 11, .endOfStringLength = 1, .frontierTrainerNameLength = 8,
        .easyChatWordLength = 0xC,
        .saveBlock2Size = 0x890, // sizeof(SaveBlock2)
        .saveBlock1Size = 0x3AC0, // sizeof(SaveBlock1)
        .partyCountOffset = 0x234, // SaveBlock1.playerPartyCount
        .partyOffset = 0x238, // SaveBlock1.playerParty
        .warpFlagsOffset = 0x9, // SaveBlock2.specialSaveWarp
        .trainerIdOffset = 0xA, // SaveBlock2.playerTrainerId
        .playerNameOffset = 0x0, // SaveBlock2.playerName
        .playerGenderOffset = 0x8, // SaveBlock2.playerGender
        .frontierStatusOffset = 0x556,
        .frontierStatusOffset2 = 0x557,
        .externalEventFlagsOffset = 0x312F, // SaveBlock1.externalEventFlags
        .externalEventDataOffset = 0x311B, // SaveBlock1.externalEventData
        .speciesInfo = (const void *)0x081D095C,
        .abilityNames = (const void *)0x081CBBD4,
        .abilityDescriptions = (const void *)0x081CBE44,
        .items = (const void *)0x0839A62C,
        .moves = (const void *)0x081CCE70,
        .ballGfx = (const struct CompressedSpriteSheet *)0x081DC5C0,
        .ballPalettes = (const struct CompressedSpritePalette *)0x081DC620,
        .gcnLinkFlagsOffset = 0x0,
        .gameClearFlag = 0x804, // FLAG_SYS_GAME_CLEAR
        .ribbonFlag = 0x83B, // FLAG_SYS_RIBBON_GET
        .bagCountItems = 20, .bagCountKeyItems = 20, .bagCountPokeballs = 16, .bagCountTMHMs = 64,
        .bagCountBerries = 46,
        .pcItemsCount = 50,
        .pcItemsOffset = 0x498, // SaveBlock1.pcItems
        .giftRibbonsOffset = 0x3110, // SaveBlock1.giftRibbons
        .enigmaBerryOffset = 0x3160, // SaveBlock1.enigmaBerry
        .enigmaBerrySize = 0x530,
        .moveDescriptions = (const void *)0x083923DC,
    },
    // Ruby (English, Rev 0)
    {
        .version = 2,
        .language = 2,
        .gameName = "pokemon ruby version",
        .monFrontPics = (const void *)0x081E8354, // gMonFrontPicTable
        .monBackPics = (const void *)0x081E97F4, // gMonBackPicTable
        .monNormalPalettes = (const void *)0x081EA5B4, // gMonPaletteTable
        .monShinyPalettes = (const void *)0x081EB374, // gMonShinyPaletteTable
        .monIcons = (const void *)0x083BBD20, // gMonIconTable
        .monIconPaletteIds = (const void *)0x083BC400, // gMonIconPaletteIndices
        .monIconPalettes = (const void *)0x083BC5B8, // gMonIconPaletteTable
        .monSpeciesNames = (const void *)0x081F716C, // gSpeciesNames
        .moveNames = (const void *)0x081F8320, // gMoveNames
        .decorations = (const void *)0x083EB6C4, // gDecorations
        .flagsOffset = 0x1220, // SaveBlock1.flags
        .varsOffset = 0x1340, // SaveBlock1.vars
        .pokedexOffset = 0x18, // SaveBlock2.pokedex
        .seen1Offset = 0x938, // SaveBlock1.dexSeen2
        .seen2Offset = 0x3A8C, // SaveBlock1.dexSeen3
        .pokedexVar = 0x46,
        .pokedexFlag = 0x836, // FLAG_SYS_NATIONAL_DEX
        .mysteryEventFlag = 0x84C, // FLAG_SYS_EXDATA_ENABLE
        .pokedexCount = 0x182,
        .playerNameLength = 7, .trainerNameLength = 10, .pokemonNameLength1 = 10, .pokemonNameLength2 = 10, .moveNameLength = 12, .itemNameLength = 12, .berryNameLength = 6, .abilityNameLength = 12, .typeNameLength = 6, .mapNameLength1 = 16, .mapNameLength2 = 18, .trainerClassNameLength = 12, .decorationNameLength = 15, .dexCategoryNameLength = 11, .endOfStringLength = 1, .frontierTrainerNameLength = 8,
        .easyChatWordLength = 0xC,
        .saveBlock2Size = 0x890, // sizeof(SaveBlock2)
        .saveBlock1Size = 0x3AC0, // sizeof(SaveBlock1)
        .partyCountOffset = 0x234, // SaveBlock1.playerPartyCount
        .partyOffset = 0x238, // SaveBlock1.playerParty
        .warpFlagsOffset = 0x9, // SaveBlock2.specialSaveWarp
        .trainerIdOffset = 0xA, // SaveBlock2.playerTrainerId
        .playerNameOffset = 0x0, // SaveBlock2.playerName
        .playerGenderOffset = 0x8, // SaveBlock2.playerGender
        .frontierStatusOffset = 0x556,
        .frontierStatusOffset2 = 0x557,
        .externalEventFlagsOffset = 0x312F, // SaveBlock1.externalEventFlags
        .externalEventDataOffset = 0x311B, // SaveBlock1.externalEventData
        .speciesInfo = (const void *)0x081FEC18, // gBaseStats
        .abilityNames = (const void *)0x081FA248, // gAbilityNames
        .abilityDescriptions = (const void *)0x081FA110, // gAbilityDescriptions
        .items = (const void *)0x083C5564, // gItems
        .moves = (const void *)0x081FB12C, // gBattleMoves
        .ballGfx = (const struct CompressedSpriteSheet *)0x0820A92C, // gUnknown_0820A904 + 0x28
        .ballPalettes = (const struct CompressedSpritePalette *)0x0820A98C, // gUnknown_0820A904 + 0x88
        .gcnLinkFlagsOffset = 0x0,
        .gameClearFlag = 0x804, // FLAG_SYS_GAME_CLEAR
        .ribbonFlag = 0x83B, // FLAG_SYS_RIBBON_GET
        .bagCountItems = 20, .bagCountKeyItems = 20, .bagCountPokeballs = 16, .bagCountTMHMs = 64,
        .bagCountBerries = 46,
        .pcItemsCount = 50,
        .pcItemsOffset = 0x498, // SaveBlock1.pcItems
        .giftRibbonsOffset = 0x3110, // SaveBlock1.giftRibbons
        .enigmaBerryOffset = 0x3160, // SaveBlock1.enigmaBerry
        .enigmaBerrySize = 0x530,
        .moveDescriptions = (const void *)0x083C09D8, // gMoveDescriptions
    },
    // Ruby (English, Rev 2)
    {
        .version = 2,
        .language = 2,
        .gameName = "pokemon ruby version",
        .monFrontPics = (const void *)0x081E836C, // gMonFrontPicTable
        .monBackPics = (const void *)0x081E980C, // gMonBackPicTable
        .monNormalPalettes = (const void *)0x081EA5CC, // gMonPaletteTable
        .monShinyPalettes = (const void *)0x081EB38C, // gMonShinyPaletteTable
        .monIcons = (const void *)0x083BBD3C, // gMonIconTable
        .monIconPaletteIds = (const void *)0x083BC41C, // gMonIconPaletteIndices
        .monIconPalettes = (const void *)0x083BC5D4, // gMonIconPaletteTable
        .monSpeciesNames = (const void *)0x081F7184, // gSpeciesNames
        .moveNames = (const void *)0x081F8338, // gMoveNames
        .decorations = (const void *)0x083EB6E0, // gDecorations
        .flagsOffset = 0x1220, // SaveBlock1.flags
        .varsOffset = 0x1340, // SaveBlock1.vars
        .pokedexOffset = 0x18, // SaveBlock2.pokedex
        .seen1Offset = 0x938, // SaveBlock1.dexSeen2
        .seen2Offset = 0x3A8C, // SaveBlock1.dexSeen3
        .pokedexVar = 0x46,
        .pokedexFlag = 0x836, // FLAG_SYS_NATIONAL_DEX
        .mysteryEventFlag = 0x84C, // FLAG_SYS_EXDATA_ENABLE
        .pokedexCount = 0x182,
        .playerNameLength = 7, .trainerNameLength = 10, .pokemonNameLength1 = 10, .pokemonNameLength2 = 10, .moveNameLength = 12, .itemNameLength = 12, .berryNameLength = 6, .abilityNameLength = 12, .typeNameLength = 6, .mapNameLength1 = 16, .mapNameLength2 = 18, .trainerClassNameLength = 12, .decorationNameLength = 15, .dexCategoryNameLength = 11, .endOfStringLength = 1, .frontierTrainerNameLength = 8,
        .easyChatWordLength = 0xC,
        .saveBlock2Size = 0x890, // sizeof(SaveBlock2)
        .saveBlock1Size = 0x3AC0, // sizeof(SaveBlock1)
        .partyCountOffset = 0x234, // SaveBlock1.playerPartyCount
        .partyOffset = 0x238, // SaveBlock1.playerParty
        .warpFlagsOffset = 0x9, // SaveBlock2.specialSaveWarp
        .trainerIdOffset = 0xA, // SaveBlock2.playerTrainerId
        .playerNameOffset = 0x0, // SaveBlock2.playerName
        .playerGenderOffset = 0x8, // SaveBlock2.playerGender
        .frontierStatusOffset = 0x556,
        .frontierStatusOffset2 = 0x557,
        .externalEventFlagsOffset = 0x312F, // SaveBlock1.externalEventFlags
        .externalEventDataOffset = 0x311B, // SaveBlock1.externalEventData
        .speciesInfo = (const void *)0x081FEC30, // gBaseStats
        .abilityNames = (const void *)0x081FA260, // gAbilityNames
        .abilityDescriptions = (const void *)0x081FA128, // gAbilityDescriptions
        .items = (const void *)0x083C5580, // gItems
        .moves = (const void *)0x081FB144, // gBattleMoves
        .ballGfx = (const struct CompressedSpriteSheet *)0x0820A944, // gUnknown_0820A904 + 0x28
        .ballPalettes = (const struct CompressedSpritePalette *)0x0820A9A4, // gUnknown_0820A904 + 0x88
        .gcnLinkFlagsOffset = 0x0,
        .gameClearFlag = 0x804, // FLAG_SYS_GAME_CLEAR
        .ribbonFlag = 0x83B, // FLAG_SYS_RIBBON_GET
        .bagCountItems = 20, .bagCountKeyItems = 20, .bagCountPokeballs = 16, .bagCountTMHMs = 64,
        .bagCountBerries = 46,
        .pcItemsCount = 50,
        .pcItemsOffset = 0x498, // SaveBlock1.pcItems
        .giftRibbonsOffset = 0x3110, // SaveBlock1.giftRibbons
        .enigmaBerryOffset = 0x3160, // SaveBlock1.enigmaBerry
        .enigmaBerrySize = 0x530,
        .moveDescriptions = (const void *)0x083C09F4, // gMoveDescriptions
    },
    // Sapphire (English, Rev 0)
    {
        .version = 1,
        .language = 2,
        .gameName = "pokemon sapphire version",
        .monFrontPics = (const void *)0x081E82E4, // gMonFrontPicTable
        .monBackPics = (const void *)0x081E9784, // gMonBackPicTable
        .monNormalPalettes = (const void *)0x081EA544, // gMonPaletteTable
        .monShinyPalettes = (const void *)0x081EB304, // gMonShinyPaletteTable
        .monIcons = (const void *)0x083BBD78, // gMonIconTable
        .monIconPaletteIds = (const void *)0x083BC458, // gMonIconPaletteIndices
        .monIconPalettes = (const void *)0x083BC610, // gMonIconPaletteTable
        .monSpeciesNames = (const void *)0x081F70FC, // gSpeciesNames
        .moveNames = (const void *)0x081F82B0, // gMoveNames
        .decorations = (const void *)0x083EB71C, // gDecorations
        .flagsOffset = 0x1220, // SaveBlock1.flags
        .varsOffset = 0x1340, // SaveBlock1.vars
        .pokedexOffset = 0x18, // SaveBlock2.pokedex
        .seen1Offset = 0x938, // SaveBlock1.dexSeen2
        .seen2Offset = 0x3A8C, // SaveBlock1.dexSeen3
        .pokedexVar = 0x46,
        .pokedexFlag = 0x836, // FLAG_SYS_NATIONAL_DEX
        .mysteryEventFlag = 0x84C, // FLAG_SYS_EXDATA_ENABLE
        .pokedexCount = 0x182,
        .playerNameLength = 7, .trainerNameLength = 10, .pokemonNameLength1 = 10, .pokemonNameLength2 = 10, .moveNameLength = 12, .itemNameLength = 12, .berryNameLength = 6, .abilityNameLength = 12, .typeNameLength = 6, .mapNameLength1 = 16, .mapNameLength2 = 18, .trainerClassNameLength = 12, .decorationNameLength = 15, .dexCategoryNameLength = 11, .endOfStringLength = 1, .frontierTrainerNameLength = 8,
        .easyChatWordLength = 0xC,
        .saveBlock2Size = 0x890, // sizeof(SaveBlock2)
        .saveBlock1Size = 0x3AC0, // sizeof(SaveBlock1)
        .partyCountOffset = 0x234, // SaveBlock1.playerPartyCount
        .partyOffset = 0x238, // SaveBlock1.playerParty
        .warpFlagsOffset = 0x9, // SaveBlock2.specialSaveWarp
        .trainerIdOffset = 0xA, // SaveBlock2.playerTrainerId
        .playerNameOffset = 0x0, // SaveBlock2.playerName
        .playerGenderOffset = 0x8, // SaveBlock2.playerGender
        .frontierStatusOffset = 0x556,
        .frontierStatusOffset2 = 0x557,
        .externalEventFlagsOffset = 0x312F, // SaveBlock1.externalEventFlags
        .externalEventDataOffset = 0x311B, // SaveBlock1.externalEventData
        .speciesInfo = (const void *)0x081FEBA8, // gBaseStats
        .abilityNames = (const void *)0x081FA1D8, // gAbilityNames
        .abilityDescriptions = (const void *)0x081FA0A0, // gAbilityDescriptions
        .items = (const void *)0x083C55BC, // gItems
        .moves = (const void *)0x081FB0BC, // gBattleMoves
        .ballGfx = (const struct CompressedSpriteSheet *)0x0820A8BC, // gUnknown_0820A904 + 0x28
        .ballPalettes = (const struct CompressedSpritePalette *)0x0820A91C, // gUnknown_0820A904 + 0x88
        .gcnLinkFlagsOffset = 0x0,
        .gameClearFlag = 0x804, // FLAG_SYS_GAME_CLEAR
        .ribbonFlag = 0x83B, // FLAG_SYS_RIBBON_GET
        .bagCountItems = 20, .bagCountKeyItems = 20, .bagCountPokeballs = 16, .bagCountTMHMs = 64,
        .bagCountBerries = 46,
        .pcItemsCount = 50,
        .pcItemsOffset = 0x498, // SaveBlock1.pcItems
        .giftRibbonsOffset = 0x3110, // SaveBlock1.giftRibbons
        .enigmaBerryOffset = 0x3160, // SaveBlock1.enigmaBerry
        .enigmaBerrySize = 0x530,
        .moveDescriptions = (const void *)0x083C0A30, // gMoveDescriptions
    },
    // Sapphire (English, Rev 2)
    {
        .version = 1,
        .language = 2,
        .gameName = "pokemon sapphire version",
        .monFrontPics = (const void *)0x081E82FC, // gMonFrontPicTable
        .monBackPics = (const void *)0x081E979C, // gMonBackPicTable
        .monNormalPalettes = (const void *)0x081EA55C, // gMonPaletteTable
        .monShinyPalettes = (const void *)0x081EB31C, // gMonShinyPaletteTable
        .monIcons = (const void *)0x083BBD98, // gMonIconTable
        .monIconPaletteIds = (const void *)0x083BC478, // gMonIconPaletteIndices
        .monIconPalettes = (const void *)0x083BC630, // gMonIconPaletteTable
        .monSpeciesNames = (const void *)0x081F7114, // gSpeciesNames
        .moveNames = (const void *)0x081F82C8, // gMoveNames
        .decorations = (const void *)0x083EB73C, // gDecorations
        .flagsOffset = 0x1220, // SaveBlock1.flags
        .varsOffset = 0x1340, // SaveBlock1.vars
        .pokedexOffset = 0x18, // SaveBlock2.pokedex
        .seen1Offset = 0x938, // SaveBlock1.dexSeen2
        .seen2Offset = 0x3A8C, // SaveBlock1.dexSeen3
        .pokedexVar = 0x46,
        .pokedexFlag = 0x836, // FLAG_SYS_NATIONAL_DEX
        .mysteryEventFlag = 0x84C, // FLAG_SYS_EXDATA_ENABLE
        .pokedexCount = 0x182,
        .playerNameLength = 7, .trainerNameLength = 10, .pokemonNameLength1 = 10, .pokemonNameLength2 = 10, .moveNameLength = 12, .itemNameLength = 12, .berryNameLength = 6, .abilityNameLength = 12, .typeNameLength = 6, .mapNameLength1 = 16, .mapNameLength2 = 18, .trainerClassNameLength = 12, .decorationNameLength = 15, .dexCategoryNameLength = 11, .endOfStringLength = 1, .frontierTrainerNameLength = 8,
        .easyChatWordLength = 0xC,
        .saveBlock2Size = 0x890, // sizeof(SaveBlock2)
        .saveBlock1Size = 0x3AC0, // sizeof(SaveBlock1)
        .partyCountOffset = 0x234, // SaveBlock1.playerPartyCount
        .partyOffset = 0x238, // SaveBlock1.playerParty
        .warpFlagsOffset = 0x9, // SaveBlock2.specialSaveWarp
        .trainerIdOffset = 0xA, // SaveBlock2.playerTrainerId
        .playerNameOffset = 0x0, // SaveBlock2.playerName
        .playerGenderOffset = 0x8, // SaveBlock2.playerGender
        .frontierStatusOffset = 0x556,
        .frontierStatusOffset2 = 0x557,
        .externalEventFlagsOffset = 0x312F, // SaveBlock1.externalEventFlags
        .externalEventDataOffset = 0x311B, // SaveBlock1.externalEventData
        .speciesInfo = (const void *)0x081FEBC0, // gBaseStats
        .abilityNames = (const void *)0x081FA1F0, // gAbilityNames
        .abilityDescriptions = (const void *)0x081FA0B8, // gAbilityDescriptions
        .items = (const void *)0x083C55DC, // gItems
        .moves = (const void *)0x081FB0D4, // gBattleMoves
        .ballGfx = (const struct CompressedSpriteSheet *)0x0820A8D4, // gUnknown_0820A904 + 0x28
        .ballPalettes = (const struct CompressedSpritePalette *)0x0820A934, // gUnknown_0820A904 + 0x88
        .gcnLinkFlagsOffset = 0x0,
        .gameClearFlag = 0x804, // FLAG_SYS_GAME_CLEAR
        .ribbonFlag = 0x83B, // FLAG_SYS_RIBBON_GET
        .bagCountItems = 20, .bagCountKeyItems = 20, .bagCountPokeballs = 16, .bagCountTMHMs = 64,
        .bagCountBerries = 46,
        .pcItemsCount = 50,
        .pcItemsOffset = 0x498, // SaveBlock1.pcItems
        .giftRibbonsOffset = 0x3110, // SaveBlock1.giftRibbons
        .enigmaBerryOffset = 0x3160, // SaveBlock1.enigmaBerry
        .enigmaBerrySize = 0x530,
        .moveDescriptions = (const void *)0x083C0A50, // gMoveDescriptions
    },
};

bool8 DetectROM(void)
{
    u8 *rom = CART_ROM;
    bool8 result = TRUE;

    gTargetIsRubySapphire = TRUE;
    gAgbPmRomParams = CART_GF_ROM_HEADER;

    switch (*(u32 *)(rom + 0xAC))
    {
    case GAME_CODE_RUBY_JP:
        gAgbPmRomParams = &gTargetGames[0];
        result = FALSE;
        break;
    case GAME_CODE_SAPPHIRE_JP:
        gAgbPmRomParams = &gTargetGames[1];
        result = FALSE;
        break;
    case GAME_CODE_RUBY_EN:
        if (*(u8 *)(rom + 0xBC) == 0)
            gAgbPmRomParams = &gTargetGames[2];
        else
            gAgbPmRomParams = &gTargetGames[3];
        break;
    case GAME_CODE_SAPPHIRE_EN:
        if (*(u8 *)(rom + 0xBC) == 0)
            gAgbPmRomParams = &gTargetGames[4];
        else
            gAgbPmRomParams = &gTargetGames[5];
        break;
    case GAME_CODE_RUBY_DE:
    case GAME_CODE_RUBY_FR:
    case GAME_CODE_RUBY_IT:
    case GAME_CODE_RUBY_ES:
    case GAME_CODE_SAPPHIRE_DE:
    case GAME_CODE_SAPPHIRE_FR:
    case GAME_CODE_SAPPHIRE_IT:
    case GAME_CODE_SAPPHIRE_ES:
        break;
    default:
        gTargetIsRubySapphire = FALSE;
        break;
    }

    return result;
}


// Returns flags for the player's current location. For the later games this
// is the save's specialSaveWarpFlags byte, and for Ruby/Sapphire a map lookup.
//  - POKECENTER_SAVEWARP
//  - LOBBY_SAVEWARP
//  - bit 7
// SetPlayerLinkInfo calls this twice and extracts bits 1 and 7 separately.
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

// Empty in this build (just "bx lr"). Unused.
void UNUSED TargetGameDummy(void)
{
}

// Returns whether the player has the Pokédex.
// SetPlayerLinkInfo stores the result in bit 0 of its status byte.
// Also called by JoyBusHandleCmdAA.
// For the later games it reads bit 0 of SaveBlock2.gcnLinkFlags through the
// header; the Ruby/Sapphire copies have no such offset, so RS uses
// FLAG_SYS_POKEDEX_GET.
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

// Returns TRUE for a FireRed or LeafGreen cartridge, whose own header gives
// version 4 or 5. Unreachable in practice only because its one caller,
// SetPlayerLinkInfo, has no caller itself.
u32 IsFRLG(void)
{
    u32 version = gAgbPmRomParams->version;

    if (version == VERSION_FIRE_RED || version == VERSION_LEAF_GREEN)
        return TRUE;
    else
        return FALSE;
}

// Returns whether the connected game has been beaten, with the test backwards. When
// gAgbPmRomParams is the cartridge's own ROM header, which names the right
// flag, it uses Ruby/Sapphire's 0x804; it only takes flagGameClear from the
// built-in copies, which say 0x804 anyway. Right for Ruby and Sapphire, and
// an unrelated flag for Emerald and FireRed/LeafGreen. See
// FLAG_SYS_GAME_CLEAR_RS.
bool8 CheckGameClear(void)
{
    s32 flagId;
    u8 *flags = gSaveBlock1Ptr + gAgbPmRomParams->flagsOffset;

    if (gAgbPmRomParams == CART_GF_ROM_HEADER)
        flagId = FLAG_SYS_GAME_CLEAR_RS;
    else
        flagId = gAgbPmRomParams->gameClearFlag;

    return (flags[flagId / 8] >> (flagId % 8)) & 1;
}

