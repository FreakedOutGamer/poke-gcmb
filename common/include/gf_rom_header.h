#ifndef GUARD_GF_ROM_HEADER_H
#define GUARD_GF_ROM_HEADER_H

// Game Freak's ROM header, pret's struct GFRomHeader (rom_header_gf.c in
// pokeemerald and pokefirered), with pokeemerald's field names. Emerald,
// FireRed and LeafGreen carry one at 0x08000100 for other programs to read.
// Ruby and Sapphire have none, so each GameCube link program ships its own
// copies for them (gTargetGames), one per version and revision, since the
// pointers are absolute addresses in that cartridge's ROM. Everything else is
// a save offset, count or flag id, identical in every Ruby/Sapphire copy.

struct CompressedSpriteSheet;
struct CompressedSpritePalette;
struct SpritePalette;
struct Decoration;
struct SpeciesInfo;
struct Item;
struct BattleMove;

struct GFRomHeader
{
    /*0x00*/ u32 version;
    /*0x04*/ u32 language;
    /*0x08*/ u8 gameName[32]; // "pokemon ruby version"
    /*0x28*/ const struct CompressedSpriteSheet *monFrontPics;
    /*0x2C*/ const struct CompressedSpriteSheet *monBackPics;
    /*0x30*/ const struct CompressedSpritePalette *monNormalPalettes;
    /*0x34*/ const struct CompressedSpritePalette *monShinyPalettes;
    /*0x38*/ const u8 *const *monIcons;
    /*0x3C*/ const u8 *monIconPaletteIds;
    /*0x40*/ const struct SpritePalette *monIconPalettes;
    // The name tables' entry widths depend on the cartridge's language, so
    // each reader casts them to its own width.
    /*0x44*/ const void *monSpeciesNames;
    /*0x48*/ const void *moveNames;
    /*0x4C*/ const struct Decoration *decorations;
    /*0x50*/ u32 flagsOffset;          // SaveBlock1.flags
    /*0x54*/ u32 varsOffset;           // SaveBlock1.vars
    /*0x58*/ u32 pokedexOffset;        // SaveBlock2.pokedex
    /*0x5C*/ u32 seen1Offset;          // SaveBlock1.dexSeen2
    /*0x60*/ u32 seen2Offset;          // SaveBlock1.dexSeen3
    /*0x64*/ u32 pokedexVar;           // VAR_NATIONAL_DEX - VARS_START (0x46)
    /*0x68*/ u32 pokedexFlag;          // FLAG_SYS_NATIONAL_DEX
    /*0x6C*/ u32 mysteryEventFlag;     // FLAG_SYS_EXDATA_ENABLE
    /*0x70*/ u32 pokedexCount;         // NATIONAL_DEX_COUNT (386)
    /*0x74*/ u8 playerNameLength;
    /*0x75*/ u8 trainerNameLength;
    /*0x76*/ u8 pokemonNameLength1;
    /*0x77*/ u8 pokemonNameLength2;
    /*0x78*/ u8 moveNameLength;
    /*0x79*/ u8 itemNameLength;
    /*0x7A*/ u8 berryNameLength;
    /*0x7B*/ u8 abilityNameLength;
    /*0x7C*/ u8 typeNameLength;
    /*0x7D*/ u8 mapNameLength1;
    /*0x7E*/ u8 mapNameLength2;
    /*0x7F*/ u8 trainerClassNameLength;
    /*0x80*/ u8 decorationNameLength;
    /*0x81*/ u8 dexCategoryNameLength;
    /*0x82*/ u8 endOfStringLength;
    /*0x83*/ u8 frontierTrainerNameLength;
    /*0x84*/ u8 easyChatWordLength;
    /*0x88*/ u32 saveBlock2Size;       // sizeof(struct SaveBlock2)
    /*0x8C*/ u32 saveBlock1Size;       // sizeof(struct SaveBlock1)
    /*0x90*/ u32 partyCountOffset;     // SaveBlock1.playerPartyCount
    /*0x94*/ u32 partyOffset;          // SaveBlock1.playerParty
    /*0x98*/ u32 warpFlagsOffset;      // SaveBlock2.specialSaveWarp
    /*0x9C*/ u32 trainerIdOffset;      // SaveBlock2.playerTrainerId
    /*0xA0*/ u32 playerNameOffset;     // SaveBlock2.playerName
    /*0xA4*/ u32 playerGenderOffset;   // SaveBlock2.playerGender
    /*0xA8*/ u32 frontierStatusOffset;  // 0x556 in the RS copies
    /*0xAC*/ u32 frontierStatusOffset2; // 0x557 in the RS copies
    /*0xB0*/ u32 externalEventFlagsOffset; // SaveBlock1.externalEventFlags
    /*0xB4*/ u32 externalEventDataOffset;  // SaveBlock1.externalEventData
    // Link restrictions, zero in every Ruby/Sapphire copy. SetPlayerLinkInfo
    // tests bit 1 and pokechan's JoyBusHandleCmdAA bit 2, each on its own.
    /*0xB8*/ u32 blockLinkBoxRS:1;
             u32 blockLinkColoXD:1;
             u32 blockJirachiDistribution:1;
             u32 blockLinkUnused:29;
    /*0xBC*/ const struct SpeciesInfo *speciesInfo;
    /*0xC0*/ const void *abilityNames;
    /*0xC4*/ const void *abilityDescriptions; // Japanese: 19-byte strings; otherwise pointers
    /*0xC8*/ const struct Item *items;
    /*0xCC*/ const struct BattleMove *moves;
    /*0xD0*/ const struct CompressedSpriteSheet *ballGfx;
    /*0xD4*/ const struct CompressedSpritePalette *ballPalettes;
    /*0xD8*/ u32 gcnLinkFlagsOffset;   // SaveBlock2.gcnLinkFlags; 0 in the RS copies
    /*0xDC*/ s32 gameClearFlag;        // FLAG_SYS_GAME_CLEAR
    /*0xE0*/ u32 ribbonFlag;           // FLAG_SYS_RIBBON_GET
    /*0xE4*/ u8 bagCountItems;
    /*0xE5*/ u8 bagCountKeyItems;
    /*0xE6*/ u8 bagCountPokeballs;
    /*0xE7*/ u8 bagCountTMHMs;
    /*0xE8*/ u8 bagCountBerries;
    /*0xE9*/ u8 pcItemsCount;
    /*0xEC*/ u32 pcItemsOffset;        // SaveBlock1.pcItems
    /*0xF0*/ u32 giftRibbonsOffset;    // SaveBlock1.giftRibbons
    /*0xF4*/ u32 enigmaBerryOffset;    // SaveBlock1.enigmaBerry
    /*0xF8*/ u32 enigmaBerrySize;      // sizeof(struct EnigmaBerry)
    /*0xFC*/ const u8 *moveDescriptions;
}; /*size = 0x100*/

// The header of the connected cartridge's game. This is the cartridge's own
// at 0x08000100, or the program's copy for Ruby/Sapphire.
extern const struct GFRomHeader *gAgbPmRomParams;

// The connected cartridge's header. AGB game codes are four characters,
// read as a little-endian word.
#define CART_ROM ((u8 *)0x08000000)
#define GAME_CODE(a, b, c, d) \
    ((a) | ((b) << 8) | ((c) << 16) | ((d) << 24))

#define GAME_CODE_RUBY_EN     GAME_CODE('A', 'X', 'V', 'E')
#define GAME_CODE_RUBY_JP     GAME_CODE('A', 'X', 'V', 'J')
#define GAME_CODE_RUBY_DE     GAME_CODE('A', 'X', 'V', 'D')
#define GAME_CODE_RUBY_FR     GAME_CODE('A', 'X', 'V', 'F')
#define GAME_CODE_RUBY_IT     GAME_CODE('A', 'X', 'V', 'I')
#define GAME_CODE_RUBY_ES     GAME_CODE('A', 'X', 'V', 'S')
#define GAME_CODE_SAPPHIRE_EN GAME_CODE('A', 'X', 'P', 'E')
#define GAME_CODE_SAPPHIRE_JP GAME_CODE('A', 'X', 'P', 'J')
#define GAME_CODE_SAPPHIRE_DE GAME_CODE('A', 'X', 'P', 'D')
#define GAME_CODE_SAPPHIRE_FR GAME_CODE('A', 'X', 'P', 'F')
#define GAME_CODE_SAPPHIRE_IT GAME_CODE('A', 'X', 'P', 'I')
#define GAME_CODE_SAPPHIRE_ES GAME_CODE('A', 'X', 'P', 'S')

// Where Emerald, FireRed and LeafGreen keep their Game Freak ROM header.
// DetectROM starts from it and only swaps in a built-in copy for
// Japanese and English Ruby/Sapphire, so any other game code reads its
// descriptor straight out of the cartridge. European Ruby/Sapphire keep it
// too, but their ROMs have no header there.
#define CART_GF_ROM_HEADER ((const struct GFRomHeader *)0x08000100)


// Ruby/Sapphire's game-clear flag, which CheckGameClear uses for exactly
// the cartridges it is wrong for. Flag 0x804 is FLAG_SYS_GAME_CLEAR only in
// Ruby/Sapphire. In Emerald it is the trainer flag for TRAINER_ROXANNE_4,
// Roxanne's third rematch, and in FireRed/LeafGreen it is
// FLAG_SYS_BLACK_FLUTE_ACTIVE. Their real game-clear flags are 0x864 and
// 0x82C.
#define FLAG_SYS_GAME_CLEAR_RS 0x804

#endif // GUARD_GF_ROM_HEADER_H
