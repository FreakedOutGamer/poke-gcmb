#include "global.h"

// This file is compiled once per resource so unchanged assets can
// match independently between regional builds.
#define STRINGIFY_(x) #x
#define STRINGIFY(x) STRINGIFY_(x)

#if COLBTL_VERSION == VERSION_COLBTL
#define GRAPHICS_FILE(path) STRINGIFY(graphics/path)
#define TITLE_GFX_FILE GRAPHICS_FILE(title.8bpp.lz)
#define TITLE_PAL_FILE GRAPHICS_FILE(title.gbapal.rl)
#define TITLE_TILEMAP_FILE GRAPHICS_FILE(title.bin.lz)
#define TITLE_EXTRA_PAL_FILE GRAPHICS_FILE(title_extra.gbapal.rl)
#define MON_INFO_ICONS_FILE GRAPHICS_FILE(mon_info_icons.4bpp.lz)
#define PARTY_MENU_FILE GRAPHICS_FILE(party_menu.4bpp.lz)
#define MESSAGE_BOX_FILE GRAPHICS_FILE(message_box.4bpp.lz)
#define TEXT_PAL_FILE GRAPHICS_FILE(text.gbapal.lz)
#define PARTY_MENU_TILEMAP_FILE GRAPHICS_FILE(party_menu.bin.lz)
#define PARTY_SLOTS_TILEMAP_FILE GRAPHICS_FILE(party_slots.bin.lz)
#define SUMMARY_SCREEN_FILE GRAPHICS_FILE(summary_screen.4bpp.lz)
#define SUMMARY_ICONS_FILE GRAPHICS_FILE(summary_icons.4bpp.lz)
#define SUMMARY_PAL_FILE GRAPHICS_FILE(summary.gbapal.lz)
#define SUMMARY_SKILLS_FILE GRAPHICS_FILE(summary_screen_skills.bin.lz)
#define SUMMARY_MOVES_FILE GRAPHICS_FILE(summary_screen_moves.bin.lz)
#define TYPE_STATUS_ICONS_FILE GRAPHICS_FILE(type_status_icons.4bpp.lz)
#define TYPE_STATUS_PAL_FILE GRAPHICS_FILE(type_status.gbapal.lz)
#define BATTLE_SCREEN_FILE GRAPHICS_FILE(battle_screen.4bpp.lz)
#define BATTLE_PAL_FILE GRAPHICS_FILE(battle.gbapal.rl)
#define BATTLE_TILEMAP_FILE GRAPHICS_FILE(battle_screen.bin.lz)
#define BATTLE_WINDOWS_FILE GRAPHICS_FILE(battle_windows.bin.lz)
#else
#if COLBTL_VERSION == VERSION_COLBTL_E
#define GRAPHICS_REGION colbtl_e
#elif COLBTL_VERSION == VERSION_COLBTL_F
#define GRAPHICS_REGION colbtl_f
#elif COLBTL_VERSION == VERSION_COLBTL_G
#define GRAPHICS_REGION colbtl_g
#elif COLBTL_VERSION == VERSION_COLBTL_I
#define GRAPHICS_REGION colbtl_i
#elif COLBTL_VERSION == VERSION_COLBTL_JP
#define GRAPHICS_REGION colbtl_jp
#elif COLBTL_VERSION == VERSION_COLBTL_S
#define GRAPHICS_REGION colbtl_s
#else
#error "unknown COLBTL_VERSION"
#endif
#define GRAPHICS_FILE(path) STRINGIFY(graphics/regions/GRAPHICS_REGION/path)
#define TITLE_GFX_FILE GRAPHICS_FILE(title.8bpp.rl)
#define TITLE_PAL_FILE GRAPHICS_FILE(title.gbapal.rl)
#define TITLE_TILEMAP_FILE GRAPHICS_FILE(title.bin.lz)
#define TITLE_EXTRA_PAL_FILE GRAPHICS_FILE(title_extra.gbapal.rl)
#define MON_INFO_ICONS_FILE GRAPHICS_FILE(mon_info_icons.4bpp.lz)
#define PARTY_MENU_FILE GRAPHICS_FILE(party_menu.4bpp.lz)
#define MESSAGE_BOX_FILE GRAPHICS_FILE(message_box.4bpp.lz)
#define TEXT_PAL_FILE GRAPHICS_FILE(text.gbapal.lz)
#define PARTY_MENU_TILEMAP_FILE GRAPHICS_FILE(party_menu.bin.lz)
#define PARTY_SLOTS_TILEMAP_FILE GRAPHICS_FILE(party_slots.bin.lz)
#define SUMMARY_SCREEN_FILE GRAPHICS_FILE(summary_screen.4bpp.lz)
#define SUMMARY_ICONS_FILE GRAPHICS_FILE(summary_icons.4bpp.lz)
#define SUMMARY_PAL_FILE GRAPHICS_FILE(summary.gbapal.lz)
#define SUMMARY_SKILLS_FILE GRAPHICS_FILE(summary_skills.bin.lz)
#define SUMMARY_MOVES_FILE GRAPHICS_FILE(summary_moves.bin.lz)
#define TYPE_STATUS_ICONS_FILE GRAPHICS_FILE(type_status_icons.4bpp.lz)
#define TYPE_STATUS_PAL_FILE GRAPHICS_FILE(type_status.gbapal.lz)
#define BATTLE_SCREEN_FILE GRAPHICS_FILE(battle_screen.4bpp.lz)
#define BATTLE_PAL_FILE GRAPHICS_FILE(battle.gbapal.rl)
#define BATTLE_TILEMAP_FILE GRAPHICS_FILE(battle_screen.bin.lz)
#define BATTLE_WINDOWS_FILE GRAPHICS_FILE(battle_windows.bin.lz)
#endif

#if GRAPHICS_PART == 1
const u32 gTitle_Gfx[] = INCBIN_U32(TITLE_GFX_FILE);
#elif GRAPHICS_PART == 2
const u32 gTitle_Pal[] = INCBIN_U32(TITLE_PAL_FILE);
#elif GRAPHICS_PART == 3
const u32 gTitle_Tilemap[] = INCBIN_U32(TITLE_TILEMAP_FILE);
#elif GRAPHICS_PART == 4
const u32 gTitleExtraPal[] = INCBIN_U32(TITLE_EXTRA_PAL_FILE);
#elif GRAPHICS_PART == 5
const u32 gMonInfoIcons_Gfx[] = INCBIN_U32(MON_INFO_ICONS_FILE);
#elif GRAPHICS_PART == 6
const u32 gPartyMenu_Gfx[] = INCBIN_U32(PARTY_MENU_FILE);
#elif GRAPHICS_PART == 7
const u32 gMessageBox_Gfx[] = INCBIN_U32(MESSAGE_BOX_FILE);
#elif GRAPHICS_PART == 8
const u32 gTextPal[] = INCBIN_U32(TEXT_PAL_FILE);
#elif GRAPHICS_PART == 9
const u32 gPartyMenu_Tilemap[] = INCBIN_U32(PARTY_MENU_TILEMAP_FILE);
#elif GRAPHICS_PART == 10
const u32 gPartySlots_Tilemap[] = INCBIN_U32(PARTY_SLOTS_TILEMAP_FILE);
#elif GRAPHICS_PART == 11
const u32 gSummaryScreen_Gfx[] = INCBIN_U32(SUMMARY_SCREEN_FILE);
#elif GRAPHICS_PART == 12
const u32 gSummaryIcons_Gfx[] = INCBIN_U32(SUMMARY_ICONS_FILE);
#elif GRAPHICS_PART == 13
const u32 gSummaryScreen_Pal[] = INCBIN_U32(SUMMARY_PAL_FILE);
#elif GRAPHICS_PART == 14
const u32 gSummaryScreen_Skills_Tilemap[] = INCBIN_U32(SUMMARY_SKILLS_FILE);
#elif GRAPHICS_PART == 15
const u32 gSummaryScreen_Moves_Tilemap[] = INCBIN_U32(SUMMARY_MOVES_FILE);
#elif GRAPHICS_PART == 16
const u32 gTypeStatusIcons_Gfx[] = INCBIN_U32(TYPE_STATUS_ICONS_FILE);
#elif GRAPHICS_PART == 17
const u32 gTypeStatusIcons_Pal[] = INCBIN_U32(TYPE_STATUS_PAL_FILE);
#elif GRAPHICS_PART == 18
const u32 gBattleScreen_Gfx[] = INCBIN_U32(BATTLE_SCREEN_FILE);
#elif GRAPHICS_PART == 19
const u32 gBattleScreen_Pal[] = INCBIN_U32(BATTLE_PAL_FILE);
#elif GRAPHICS_PART == 20
const u32 gBattleScreen_Tilemap[] = INCBIN_U32(BATTLE_TILEMAP_FILE);
#elif GRAPHICS_PART == 21
const u32 gBattleWindows_Tilemap[] = INCBIN_U32(BATTLE_WINDOWS_FILE);
#else
#error "unknown GRAPHICS_PART"
#endif
