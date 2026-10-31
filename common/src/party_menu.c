#include "global.h"
#include "libpmagb/save_block_pointers.h"
#include "pokemon.h"
#include "constants/species.h"
#include "colbtl_version.h"
#include "gflib.h"
#include "colbtl.h"
#include "string_util.h"
#include "constants/items.h"
#include "constants/songs.h"
#include "strings.h"
#include "graphics.h"
#include "battle_menu.h"
#include "fade.h"
#include "link_transfer.h"
#include "party_menu.h"
#include "pokemon_summary_screen.h"
#include "render_text.h"
#include "window.h"

// Localized selection numbers need an extra row in the taller text window.
#define PARTY_SELECTION_TEXT_Y \
    ((GAME_LANGUAGE == LANGUAGE_FRENCH || GAME_LANGUAGE == LANGUAGE_ITALIAN \
      || GAME_LANGUAGE == LANGUAGE_SPANISH) ? 3 : 2)

struct PartySlotCoords
{
    u8 x;
    u8 y;
    u8 tilemapInfoId;
};

struct PartySlotTilemapInfo
{
    u8 width;
    u8 height;
    u8 tileOffset;
};

// The slot renderers in the first English and Japanese revisions use
// four-byte tilemap records; navigation retains its three-byte records.
struct PartySlotDrawTilemapInfo
{
    u8 width;
    u8 height;
    u8 tileOffset;
#if COLBTL_IS(COLBTL) || GAME_LANGUAGE == LANGUAGE_JAPANESE
    u8 unused;
#endif
};

struct PartyMonIcon
{
    u8 monId;
    u8 x;
    u8 y;
    u8 statusPrimary;
    u16 species;
    u16 speciesIcon; // Support for Unown.
    u16 tileOffset;
    struct Sprite *monSprite; // 0xC
    u8 filler10;
    u8 filler11;
    u8 filler12;
    u8 filler13;
    struct Window *nameWindow;
    struct Window *levelWindow;
    struct Window *hpWindow;
    u8 selectionNumberX;
    u8 selectionNumberY;
    u8 selectionOrder;
    struct Window *selectionNumberWindow; // 0x24
    struct Sprite *heldItemSprite;
};

struct PartyMenuState
{
    struct PartyMonIcon mons[PARTY_SIZE];
    struct Window *promptWindow;
    struct Window *actionMenuWindow;
    struct Window *messageWindow;
    struct Sprite *cursorSprite;
    u8 isMultiSelect;
    u8 actionMenuCursor;
    u8 layoutId;
    u8 selectedMon;
    u8 filler11C;
    u8 filler11D;
    u8 previousSelection;
    u8 filler11F;
    u8 lastPartyMon;
    u8 iconBounceOffset;
    u8 opponentMonId;
    u8 numMonsToSelect;
    u8 numMonsLeftToSelect;
    u8 cancelDisabled;
};

struct PartyMenuNavigation
{
    u8 right;
    u8 left;
    u8 previousSelection;
    u8 unused;
};

struct PartySlotPosition
{
    u8 x;
    u8 y;
    u8 tilemapInfoId;
    u8 unused;
};

extern volatile struct MonLinkData gMonLinkData;
extern u16 gNextWindowBaseBlock;
extern const struct Subsprite gSubspritePartyMonIcon[];
extern const struct Subsprite gSubspriteHeldItem[];
extern const struct Subsprite gSubspritesPartyMenuCursor[];
extern const struct PartySlotCoords gPartySlotCoordsDouble[];
extern const struct PartySlotCoords gPartySlotCoordsSingle[];
extern const struct PartySlotTilemapInfo gPartySlotTilemapInfo[];
extern const u16 gInternationalMonNameTilemap[];
extern const u16 gJapaneseMonNameTilemap[];
extern const struct Window gWindowTemplatePartyPrompt;
extern const struct Window gWindowTemplatePartyMessage;
extern const struct Window gWindowTemplatePartyActionMenu;
extern const struct PartyMenuNavigation *gPartyMenuNavigationTables[2];
extern const struct PartySlotPosition *gPartySlotCoordsTables[];

extern struct PartyMenuState gPartyMenuState;

void DrawPartyMonHealthBar(int bgNum, int x, int y, u32 monId)
{
    s32 tileX, barPixels, x2;
    s32 hp, maxHp;
    u16 tile;

    SetBgTilemapBufferTileAt(bgNum, x, y, 0x300A);
    SetBgTilemapBufferTileAt(bgNum, x + 1, y, 0x300B);
    SetBgTilemapBufferTileAt(bgNum, x + 8, y, 0x300C);

    hp = GetMonData(&gPlayerPartyPtr[monId], MON_DATA_HP, NULL);
    maxHp = GetMonData(&gPlayerPartyPtr[monId], MON_DATA_MAX_HP, NULL);
    if (hp > maxHp)
        hp = maxHp;

    if (20 * maxHp > hp * 100)
        tile = 0x6001;
    else if (50 * maxHp > hp * 100)
        tile = 0x5001;
    else
        tile = 0x4001;

    barPixels = Div(hp * 48, maxHp);
    tileX = 0;
    x += 2;
    while (barPixels > 8)
    {
        SetBgTilemapBufferTileAt(bgNum, x + tileX, y, tile + 8);
        tileX++;
        barPixels -= 8;
    }

    if (barPixels != 0)
        SetBgTilemapBufferTileAt(bgNum, x + tileX++, y, barPixels + tile);

    for (; tileX < 6; tileX++)
    {
        SetBgTilemapBufferTileAt(bgNum, x + tileX, y, tile);
    }
}

struct Window *CreatePartyMonHPWindow(u32 left, u32 top, u32 monId)
{
    s32 i;
    u8 text[8];
    s32 hp, maxHp;
    struct Window *win = CreateTextWindow(1, left, top, 7, 1, 8);

    ClearWindowCharBuffer(win, 0);
    hp = GetMonData(&gPlayerPartyPtr[monId], MON_DATA_HP, NULL);
    maxHp = GetMonData(&gPlayerPartyPtr[monId], MON_DATA_MAX_HP, NULL);

    NumToPmString3CustomZeroChar(hp, text, 0x76);
    for (i = 0; text[i] == 0x76; i++)
        text[i] = CHAR_EXTRA_SYMBOL;
    text[3] = 0x73;
    if (hp == 0)
        text[2] = 0x76;

    NumToPmString3CustomZeroChar(maxHp, text + 4, 0x76);
    for (i = 0; text[i + 4] == 0x76; i++)
        text[i + 4] = CHAR_EXTRA_SYMBOL;
    if (maxHp == 0)
        text[6] = 0x76;

#if GAME_LANGUAGE != LANGUAGE_JAPANESE
    TextWindowSetXY(win, 4, 0);
#endif
    RenderText_NoPlaceholders(win, text);
    return win;
}

inline void UNUSED BuildTwoRowMonName(u8 *monName, u8 *dest, u32 nameLen)
{
    u16 tiles;
    s32 i;
#if GAME_LANGUAGE == LANGUAGE_JAPANESE
    for (i = 0; *monName != EOS; monName++, i++)
    {
        tiles = gJapaneseMonNameTilemap[*monName];
        dest[i] = tiles >> 8;
        dest[nameLen + 1 + i] = tiles;
    }
    dest[nameLen] = CHAR_NEWLINE;
    dest[nameLen * 2 + 1] = EOS;
#else
    if (monName[0] != EXT_CTRL_CODE_BEGIN)
    {
        for (i = 0; *monName != EOS; monName++, i++)
        {
            tiles = gInternationalMonNameTilemap[*monName];
            dest[i] = tiles >> 8;
            dest[nameLen + 1 + i] = tiles;
        }
        dest[nameLen] = CHAR_NEWLINE;
        dest[nameLen * 2 + 1] = EOS;
    }
    else
    {
        dest[0] = monName[0];
        monName++;
        dest[1] = monName[0];
        monName++;
        nameLen -= 2;
        for (i = 2; *monName != EOS; monName++, i++)
        {
            tiles = gJapaneseMonNameTilemap[*monName];
            dest[i] = tiles >> 8;
            dest[nameLen + 1 + i] = tiles;
        }
        dest[nameLen + 2] = CHAR_NEWLINE;
        dest[nameLen * 2 + 3] = EOS;
    }
#endif
}

inline struct Window * UNUSED PutMonLvlOnWindow(u32 x, u32 y, u32 monId)
{
    s32 i;
    u8 text[8];
    struct Window *win = CreateTextWindow(1, x - (GAME_LANGUAGE == LANGUAGE_FRENCH), y, 3, 1, 8);
    u32 lvl = GetMonData(&gPlayerPartyPtr[monId], MON_DATA_LEVEL, NULL);

#if COLBTL_IS(COLBTL) || GAME_LANGUAGE == LANGUAGE_JAPANESE
    SetBgTilemapBufferTileAt(0, x - 1, y, 13);
#else
    text[0] = 0x60;
    text[1] = EOS;
    TextWindowSetXY(win, 0, 0);
    RenderText(win, text);
#endif
    NumToPmString3CustomZeroChar(lvl, text, 0x76);
    for (i = 0; text[i] == 0x76; i++)
        ;
    if (text[i] == EOS)
        i--;
    RenderText_NoPlaceholders(win, &text[i]);

    return win;
}

inline void UNUSED PutMonGenderOnBgTilemap(int x, int y, u32 monId)
{
    u16 tileNum;
    switch (GetBoxMonGender(&gPlayerPartyPtr[monId]))
    {
    default:
        tileNum = 0;
        break;
    case MON_MALE:
        tileNum = 0xF;
        break;
    case MON_FEMALE:
        tileNum = 0x11;
        break;
    }
    SetBgTilemapBufferTileAt(0, x, y, tileNum);
}

inline void UNUSED CopyMonIconToVram(struct PartyMonIcon *icon, u32 frame)
{
    CpuCopy16(&gAgbPmRomParams->monIcons[icon->speciesIcon][frame * 512], (void *)VRAM + 0x10000 + (icon->tileOffset * 32), 512);
}

inline void UNUSED ConfigurePartyMenuTilemap(s32 mode)
{
    switch (mode)
    {
    case 0:
        CopyRectWithinBgTilemapBuffer(1, 0,   0x14, 6, 2, 0x18, 0x12);
        break;
    case 2:
        CopyRectWithinBgTilemapBuffer(1, 0xC, 0x14, 6, 2, 0x18, 0x10);
        CopyRectWithinBgTilemapBuffer(1, 0,   0x14, 6, 2, 0x18, 0x12);
        break;
    case 1:
        CopyRectWithinBgTilemapBuffer(1, 0xC, 0x14, 6, 2, 0x18, 0x12);
        break;
    }
}

inline void UNUSED PrintPartySelectionOrder(struct Window *win, struct PartyMonIcon *icon)
{
    u8 text[4];

    ClearWindowCharBuffer(win, 0);
    if (icon->selectionOrder == 0)
    {
        RenderTextAt(win, 0, 2, gText_Space);
    }
    else
    {
        text[0] = icon->selectionOrder + CHAR_0;
        text[1] = EOS;
        BufferString(0, text);
        TextWindowSetXY(win, 0, PARTY_SELECTION_TEXT_Y);
        RenderText(win, gText_Number);
    }
}

void CreateMonIcon(struct Sprite *sprite)
{
    u32 frame;
    if (sprite->data[1] == 7 || sprite->data[1] == 20)
    {
        struct PartyMonIcon *ptr = (void *)sprite->data[0];
        if (ptr->statusPrimary != STATUS_PRIMARY_FAINTED)
        {
            sprite->data[1] = 0;
            sprite->data[2]++;
        }
        else if (sprite->data[1] == 7)
        {
            sprite->data[1]++;
            return;
        }
        else
        {
            sprite->data[1] = 0;
        }

        frame = sprite->data[2] & 1;
        CopyMonIconToVram(ptr, frame);
        SetSpritePos(sprite, ptr->x, ptr->y);
        if (gPartyMenuState.selectedMon == ptr->monId)
        {
            if (gPartyMenuState.iconBounceOffset == 4)
                AddSpritePos(sprite, 0, -gPartyMenuState.iconBounceOffset);
            else
                SetSpritePos(sprite, ptr->x, ptr->y);
            gPartyMenuState.iconBounceOffset ^= 4;
        }
    }
    else
    {
        sprite->data[1]++;
    }
}

void UndoLastPartySelection(void)
{
    u8 text[4];
    struct Window *win;
    s32 i;

    for (i = 0; i < PARTY_SIZE; i++)
    {
        struct PartyMonIcon *ptr = &gPartyMenuState.mons[i];
        if (ptr->selectionOrder == gPartyMenuState.numMonsLeftToSelect)
        {
            ptr->selectionOrder = 0;
            win = ptr->selectionNumberWindow;
            ClearWindowCharBuffer(win, 0);
            if (ptr->selectionOrder == 0)
            {
                RenderTextAt(win, 0, 2, gText_Space);
            }
            else
            {
                text[0] = ptr->selectionOrder + CHAR_0;
                text[1] = EOS;
                BufferString(0, text);
                TextWindowSetXY(win, 0, PARTY_SELECTION_TEXT_Y);
                RenderText(win, gText_Number);
            }
            gPartyMenuState.numMonsLeftToSelect--;
            gPartyMenuState.selectedMon = i;
            break;
        }
    }
}

u32 CreatePartyMonIconSprite(struct PartyMonIcon *icon, u32 monId, s32 x, s32 y)
{
    u32 personality;
    struct Sprite *sprite;

    icon->species = GetMonData(&gPlayerPartyPtr[monId], MON_DATA_SPECIES_OR_EGG, NULL);
    if (icon->species == SPECIES_NONE)
        return SPECIES_NONE;

    icon->tileOffset = (monId * 16) + 0xC0;
    icon->monId = monId;
    icon->x = x;
    icon->y = y;
    personality = GetMonData(&gPlayerPartyPtr[monId], MON_DATA_PERSONALITY, NULL);
    icon->speciesIcon = GetIconSpecies(icon->species, personality);
    sprite = AddSprite(x, y, gSubspritePartyMonIcon);
    sprite->data[0] = (uintptr_t)(icon);
    sprite->callback = CreateMonIcon;
    SetSpritePaletteNum(sprite, gAgbPmRomParams->monIconPaletteIds[icon->species]);
    SetSpriteTileOffset(sprite, icon->tileOffset);
    CopyMonIconToVram(icon, 1);
    icon->monSprite = sprite;
    return icon->species;
}

void DrawPartyMonSlotLeft(struct PartyMenuState *menu, const struct PartySlotCoords *coords, u32 monId)
{
    s32 i;
    u8 monName[24];
    u8 nameBuffer[44];
    const struct PartySlotDrawTilemapInfo *tilemapInfo = &((const struct PartySlotDrawTilemapInfo *)gPartySlotTilemapInfo)[coords->tilemapInfoId];
    struct PartyMonIcon *icon = &menu->mons[monId];
    u32 species = CreatePartyMonIconSprite(icon, monId, (coords->x + (GAME_LANGUAGE == LANGUAGE_JAPANESE)) * 8, (coords->y * 8) + 2);

    if (species == SPECIES_NONE)
    {
        tilemapInfo = &((const struct PartySlotDrawTilemapInfo *)gPartySlotTilemapInfo)[2];
        CopyToBgTilemapBufferRect(3, coords->x, coords->y, tilemapInfo->width, tilemapInfo->height, (u16 *) (VRAM + 0x14000) + tilemapInfo->tileOffset);
    }
    else
    {
        icon->statusPrimary = GetMonAilment(&gPlayerPartyPtr[monId]);
        CopyToBgTilemapBufferRect(3, coords->x, coords->y, tilemapInfo->width, tilemapInfo->height, (u16 *) (VRAM + 0x14000) + tilemapInfo->tileOffset);
        if (icon->statusPrimary != STATUS_PRIMARY_FAINTED)
        {
            if (menu->selectedMon == monId)
                SetBgTilemapBufferPaletteRect(3, coords->x, coords->y, tilemapInfo->width, tilemapInfo->height, 7);
        }
        else
        {
            if (menu->selectedMon == monId)
                SetBgTilemapBufferPaletteRect(3, coords->x, coords->y, tilemapInfo->width, tilemapInfo->height, 9);
            else
                SetBgTilemapBufferPaletteRect(3, coords->x, coords->y, tilemapInfo->width, tilemapInfo->height, 5);
        }

        icon->nameWindow = CreateTextWindow(1, coords->x + 4 + (GAME_LANGUAGE == LANGUAGE_JAPANESE), coords->y + 1, gAgbPmRomParams->pokemonNameLength2, 2, 8);
        GetMonData(&gPlayerPartyPtr[monId], MON_DATA_NICKNAME, monName);
        BuildTwoRowMonName(monName, nameBuffer, StringLength(monName));
#if !COLBTL_IS(COLBTL) && GAME_LANGUAGE != LANGUAGE_JAPANESE
        if (GetMonData(&gPlayerPartyPtr[monId], MON_DATA_LANGUAGE, monName) != LANGUAGE_JAPANESE)
            icon->nameWindow->startX = 1;
        else
            icon->nameWindow->startX = 2;
        TextWindowSetXY(icon->nameWindow, icon->nameWindow->startX, 0);
#endif
        RenderText_NoPlaceholders(icon->nameWindow, nameBuffer);

        if (icon->statusPrimary == STATUS_PRIMARY_NONE || icon->statusPrimary == STATUS_PRIMARY_POKERUS)
        {
            icon->levelWindow = PutMonLvlOnWindow(coords->x + 6, coords->y + 3, monId);
        }
        else
        {
            for (i = 0; i < 4; i++)
            {
                u32 tileNum = i + 0xB05C + (icon->statusPrimary * 4);
                SetBgTilemapBufferTileAt(0, coords->x + 5 + i, coords->y + 3, tileNum);
            }
            gBgTilemapBufferTransferScheduled[0] = 1;
        }

        if (species != SPECIES_NIDORAN_M && species != SPECIES_NIDORAN_F)
        {
            PutMonGenderOnBgTilemap(coords->x + 9, coords->y + 3, monId);
        }
        else
        {
            GetSpeciesName(nameBuffer, species);
            if (StringCompare(monName, nameBuffer) != 0)
            {
                PutMonGenderOnBgTilemap(coords->x + 9, coords->y + 3, monId);
            }
        }

        if (gPartyMenuState.isMultiSelect == 0)
        {
            icon->hpWindow = CreatePartyMonHPWindow(coords->x + (GAME_LANGUAGE == LANGUAGE_JAPANESE ? 3 : 5), coords->y + 5, monId);
            DrawPartyMonHealthBar(0, coords->x + 2, coords->y + 4, monId);
            gPartyMenuState.mons[monId].selectionNumberX = coords->x + 2;
            gPartyMenuState.mons[monId].selectionNumberY = coords->y + 4;
        }
        else
        {
            icon->selectionNumberX = coords->x + 3;
            icon->selectionNumberY = coords->y + 4;
            icon->selectionNumberWindow = CreateTextWindow(monId + 5, icon->selectionNumberX, icon->selectionNumberY, 6, PARTY_SELECTION_TEXT_Y, 0x10);
            PrintPartySelectionOrder(icon->selectionNumberWindow, icon);
        }

        if (GetMonData(&gPlayerPartyPtr[monId], MON_DATA_HELD_ITEM, NULL) != ITEM_NONE)
        {
            icon->heldItemSprite = AddSprite((coords->x * 8) + 24, (coords->y * 8) + 21, gSubspriteHeldItem);
            SetSpritePaletteNum(icon->heldItemSprite, 12);
        }
        gPartyMenuState.lastPartyMon = monId;
    }
}

void DrawPartyMonSlotRight(struct PartyMenuState *menu, const struct PartySlotCoords *coords, u32 monId)
{
    s32 i;
    u8 monName[24];
    u8 nameBuffer[44];
    const struct PartySlotDrawTilemapInfo *tilemapInfo = &((const struct PartySlotDrawTilemapInfo *)gPartySlotTilemapInfo)[coords->tilemapInfoId];
    struct PartyMonIcon *icon = &menu->mons[monId];
    u32 species = CreatePartyMonIconSprite(icon, monId, ((coords->x + (GAME_LANGUAGE == LANGUAGE_JAPANESE)) * 8) - (!COLBTL_IS(COLBTL) && GAME_LANGUAGE != LANGUAGE_JAPANESE), ((coords->y - 1) * 8) + 2);

    if (species == SPECIES_NONE)
    {
        tilemapInfo = &((const struct PartySlotDrawTilemapInfo *)gPartySlotTilemapInfo)[2];
        CopyToBgTilemapBufferRect(3, coords->x, coords->y, tilemapInfo->width, tilemapInfo->height, (u16 *) (VRAM + 0x14000) + tilemapInfo->tileOffset);
    }
    else
    {
        icon->statusPrimary = GetMonAilment(&gPlayerPartyPtr[monId]);
        CopyToBgTilemapBufferRect(3, coords->x, coords->y, tilemapInfo->width, tilemapInfo->height, (u16 *) (VRAM + 0x14000) + tilemapInfo->tileOffset);
        if (icon->statusPrimary != STATUS_PRIMARY_FAINTED)
        {
            if (menu->selectedMon == monId)
                SetBgTilemapBufferPaletteRect(3, coords->x, coords->y, tilemapInfo->width, tilemapInfo->height, 7);
        }
        else
        {
            if (menu->selectedMon == monId)
                SetBgTilemapBufferPaletteRect(3, coords->x, coords->y, tilemapInfo->width, tilemapInfo->height, 9);
            else
                SetBgTilemapBufferPaletteRect(3, coords->x, coords->y, tilemapInfo->width, tilemapInfo->height, 5);
        }

        icon->nameWindow = CreateTextWindow(1, coords->x + (GAME_LANGUAGE == LANGUAGE_JAPANESE ? 5 : 3), coords->y, gAgbPmRomParams->pokemonNameLength2, 2, 8);
        GetMonData(&gPlayerPartyPtr[monId], MON_DATA_NICKNAME, monName);
        BuildTwoRowMonName(monName, nameBuffer, StringLength(monName));
#if !COLBTL_IS(COLBTL) && GAME_LANGUAGE != LANGUAGE_JAPANESE
        if (GetMonData(&gPlayerPartyPtr[monId], MON_DATA_LANGUAGE, monName) != LANGUAGE_JAPANESE)
            icon->nameWindow->startX = 6;
        else
            icon->nameWindow->startX = 8;
#elif COLBTL_IS(COLBTL)
        icon->nameWindow->startX = 7;
#endif
#if GAME_LANGUAGE != LANGUAGE_JAPANESE
        TextWindowSetXY(icon->nameWindow, icon->nameWindow->startX, 0);
#endif
        RenderText_NoPlaceholders(icon->nameWindow, nameBuffer);

        if (icon->statusPrimary == STATUS_PRIMARY_NONE || icon->statusPrimary == STATUS_PRIMARY_POKERUS)
        {
            icon->levelWindow = PutMonLvlOnWindow(coords->x + 6, coords->y + 2, monId);
        }
        else
        {
            for (i = 0; i < 4; i++)
            {
                u32 tileNum = i + 0xB05C + (icon->statusPrimary * 4);
                SetBgTilemapBufferTileAt(0, coords->x + 5 + i, coords->y + 2, tileNum);
            }
        }

        if (species != SPECIES_NIDORAN_M && species != SPECIES_NIDORAN_F)
        {
            PutMonGenderOnBgTilemap(coords->x + 9, coords->y + 2, monId);
        }
        else
        {
            GetSpeciesName(nameBuffer, species);
            if (StringCompare(monName, nameBuffer) != 0)
            {
                PutMonGenderOnBgTilemap(coords->x + 9, coords->y + 2, monId);
            }
        }

        if (gPartyMenuState.isMultiSelect == 0)
        {
            icon->hpWindow = CreatePartyMonHPWindow(coords->x + (GAME_LANGUAGE == LANGUAGE_JAPANESE ? 11 : 13), coords->y + 2, monId);
            DrawPartyMonHealthBar(0, coords->x + 10, coords->y + 1, monId);
            gPartyMenuState.mons[monId].selectionNumberX = coords->x + 10;
            gPartyMenuState.mons[monId].selectionNumberY = coords->y + 1;
        }
        else
        {
            icon->selectionNumberX = coords->x + 11;
            icon->selectionNumberY = coords->y;
            icon->selectionNumberWindow = CreateTextWindow(monId + 5, icon->selectionNumberX, icon->selectionNumberY, 6, PARTY_SELECTION_TEXT_Y, 0x10);
            PrintPartySelectionOrder(icon->selectionNumberWindow, icon);
        }

        if (GetMonData(&gPlayerPartyPtr[monId], MON_DATA_HELD_ITEM, NULL) != ITEM_NONE)
        {
            icon->heldItemSprite = AddSprite((coords->x * 8) + 24, (coords->y * 8) + 15, gSubspriteHeldItem);
            SetSpritePaletteNum(icon->heldItemSprite, 12);
        }
        gPartyMenuState.lastPartyMon = monId;
        gBgTilemapBufferTransferScheduled[0] = TRUE;
    }
}

void PrintPartySelectionPrompt(u32 selectMonsTextId)
{
    u8 text[8];
    DrawTextWindowBorder(0, 0x10, 0x18, 4, 0xE001);
    gBgTilemapBufferTransferScheduled[2] = TRUE;
    gPartyMenuState.promptWindow = AddWindow(0, &gWindowTemplatePartyPrompt);
    SetTextColor(gPartyMenuState.promptWindow, 1, 8);
    ClearWindowCharBuffer(gPartyMenuState.promptWindow, 0xFFFF);
    if (gPartyMenuState.numMonsToSelect != 0)
    {
        switch (selectMonsTextId)
        {
        case 0:
            text[0] = gPartyMenuState.numMonsToSelect + CHAR_0;
            text[1] = EOS;
            BufferString(0, text);
            RenderText(gPartyMenuState.promptWindow, gText_SelectNumberPokemon);
            break;
        case 1:
            RenderText(gPartyMenuState.promptWindow, gText_SelectAdditionalPokemon);
            break;
        }
    }
    else
    {
        RenderText(gPartyMenuState.promptWindow, gText_SelectPokemon);
    }
}

void InitPartyMenu(struct PartyMenuState *menu)
{
    s32 i;
    const struct PartySlotCoords *coords;
    const struct SpritePalette *monIconPalettes;

    gNextWindowBaseBlock = 0x80;
    ClearVram();
    REG_DISPCNT = DISPCNT_BG_ALL_ON | DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP;
    REG_BG0CNT = BGCNT_PRIORITY(2) | BGCNT_CHARBASE(1) | BGCNT_SCREENBASE(28);
    REG_BG1CNT = BGCNT_PRIORITY(3) | BGCNT_CHARBASE(0) | BGCNT_SCREENBASE(29);
    REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHARBASE(2) | BGCNT_SCREENBASE(30);
    REG_BG3CNT = BGCNT_PRIORITY(2) | BGCNT_CHARBASE(0) | BGCNT_SCREENBASE(31);
    SetBgPos(3, 0, -1);
    AutoUnCompVram(gTextPal, (void *) PLTT);
    AutoUnCompVram(gMonInfoIcons_Gfx, (void *) VRAM + 0x4000);
    AutoUnCompVram(gPartyMenu_Gfx, (void *) VRAM);
    AutoUnCompVram(gMessageBox_Gfx, (void *) VRAM + 0x8000);
    AutoUnCompVram(gPartyMenu_Tilemap, BG_TILEMAP_BUFFER(1));
    AutoUnCompVram(gTypeStatusIcons_Pal, (void *) PLTT + 0x200);
    AutoUnCompVram(gTypeStatusIcons_Gfx, (void *) VRAM + 0x10000);
    ConfigurePartyMenuTilemap(gPartyMenuState.isMultiSelect);
    gBgTilemapBufferTransferScheduled[1] = TRUE;
    monIconPalettes = gAgbPmRomParams->monIconPalettes;
    DmaCopy16(3, monIconPalettes[0].data, (void *) PLTT + 0x200, 0x60);
    AutoUnCompVram(gPartySlots_Tilemap, (void *) VRAM + 0x14000);
    if (menu->layoutId == 0)
        coords = gPartySlotCoordsSingle;
    else
        coords = gPartySlotCoordsDouble;
    for (i = 0; i < PARTY_SIZE; i++, coords++)
    {
        if (coords->tilemapInfoId == 0)
            DrawPartyMonSlotLeft(menu, coords, i);
        else
            DrawPartyMonSlotRight(menu, coords, i);
    }
    gBgTilemapBufferTransferScheduled[0] = TRUE;
    gBgTilemapBufferTransferScheduled[3] = TRUE;
    PrintPartySelectionPrompt(0);
}

void PrintPartySelectionError(u32 monId, u32 stringId)
{
    u8 name[24];

    if (gPartyMenuState.cursorSprite != NULL)
    {
        MoveSpriteToHead(gPartyMenuState.cursorSprite);
        gPartyMenuState.cursorSprite = NULL;
    }
    FillBgTilemapBufferRect(2, 0, 12, 30, 8, 0);
    DrawTextWindowBorder(3, 14, 24, 6, 0xE001);
    gPartyMenuState.messageWindow = AddWindow(0, &gWindowTemplatePartyMessage);
    SetTextColor(gPartyMenuState.messageWindow, 1, 8);
    ClearWindowCharBuffer(gPartyMenuState.messageWindow, 0xFFFF);
    gBgTilemapBufferTransferScheduled[2] = TRUE;
    GetMonData(&gPlayerPartyPtr[monId], MON_DATA_NICKNAME, name);
    BufferString(4, name);
    PlaySE(SONG_SE_FAILURE);
    RenderText(gPartyMenuState.messageWindow, gBattleStringsTable[stringId]);
    FillBgTilemapBufferRect(2, 3, 14, 24, 6, 0);
}

s32 HandlePartyMonActionMenu(u32 monId)
{
    s32 restriction;
    u32 keys;
    u8 monName[16];
    u32 i = 0;
    bool32 keepGoing = TRUE;

    while (TRUE)
    {
        if (!keepGoing)
            break;
        switch (i)
        {
        case 0:
            ClearWindowCharBuffer(gPartyMenuState.promptWindow, 0xFFFF);
            TextWindowFillTileBufferForText(gPartyMenuState.promptWindow);
            DrawTextWindowBorder(0, 16, 20, 4, 0xE001);
            DrawTextWindowBorder(20, 12, 10, 8, 0xE001);
            GetMonData(&gPlayerPartyPtr[monId], MON_DATA_NICKNAME, monName);
            BufferString(4, monName);
            TextWindowSetXY(gPartyMenuState.promptWindow, 0, 0);
            RenderText(gPartyMenuState.promptWindow, gText_DoWhatWithMon);
            gPartyMenuState.actionMenuWindow = AddWindow(2, &gWindowTemplatePartyActionMenu);
            SetTextColor(gPartyMenuState.actionMenuWindow, 1, 8);
            TextWindowFillTileBufferForText(gPartyMenuState.actionMenuWindow);
            ClearWindowCharBuffer(gPartyMenuState.actionMenuWindow, 0xFFFF);
            if (gPartyMenuState.cancelDisabled == 0)
                RenderTextAt(gPartyMenuState.actionMenuWindow, GAME_LANGUAGE == LANGUAGE_JAPANESE ? 8 : 0, 0, gText_Switch);
            else
                RenderTextAt(gPartyMenuState.actionMenuWindow, GAME_LANGUAGE == LANGUAGE_JAPANESE ? 8 : 0, 0, gText_SendOut);
            RenderTextAt(gPartyMenuState.actionMenuWindow, GAME_LANGUAGE == LANGUAGE_JAPANESE ? 8 : 0, 16, gText_Summary);
            RenderTextAt(gPartyMenuState.actionMenuWindow, GAME_LANGUAGE == LANGUAGE_JAPANESE ? 8 : 0, 32, gText_Cancel);
            gPartyMenuState.cursorSprite = AddSprite(0, 0, gSubspritesPartyMenuCursor);
            SetSpritePaletteNum(gPartyMenuState.cursorSprite, 5);
            gPartyMenuState.actionMenuCursor = 0;
            SetSpritePos(gPartyMenuState.cursorSprite, 168, 104);
            gBgTilemapBufferTransferScheduled[2] = TRUE;
            if (IsScreenFadedOut())
                FadeIn();
            i = 1;
            break;
        case 1:
            DelayFrames(1);
            keys = gNewKeys;
            if (keys & B_BUTTON)
                i = 2;
            if (keys & A_BUTTON)
            {
                if (gPartyMenuState.actionMenuCursor == 0)
                    i = 3;
                else if (gPartyMenuState.actionMenuCursor == 1)
                    i = 4;
                else
                    i = 2;
            }
            else
            {
                keys = gNewAndRepeatedKeys;
                if (keys & DPAD_DOWN)
                {
                    if (gPartyMenuState.actionMenuCursor != 2)
                        gPartyMenuState.actionMenuCursor++;
                    else
                        gPartyMenuState.actionMenuCursor = 0;
                    PlaySE(SONG_SE_SELECT);
                }
                if (keys & DPAD_UP)
                {
                    if (gPartyMenuState.actionMenuCursor != 0)
                        gPartyMenuState.actionMenuCursor--;
                    else
                        gPartyMenuState.actionMenuCursor = 2;
                    PlaySE(SONG_SE_SELECT);
                }
                SetSpritePos(gPartyMenuState.cursorSprite, 168, 104 + (gPartyMenuState.actionMenuCursor * 16));
            }
            break;
        case 2:
            PlaySE(SONG_SE_SELECT);
            if (gPartyMenuState.cursorSprite != NULL)
            {
                MoveSpriteToHead(gPartyMenuState.cursorSprite);
                gPartyMenuState.cursorSprite = NULL;
            }
            FillBgTilemapBufferRect(2, 20, 12, 10, 8, 0);
            PrintPartySelectionPrompt(0);
            return 0;
        case 3:
            restriction = GetSwitchOutRestriction();
            if ((restriction & 0xFFFF) == 1)
            {
                PlaySE(SONG_SE_FAILURE);
                PrintPartySelectionError(gPartyMenuState.opponentMonId, 0);
                i = 2;
            }
            else if ((restriction & 0xFFFF) == 2)
            {
                PlaySE(SONG_SE_FAILURE);
                BufferString(0, (void *) gMonLinkData.monName[restriction >> 16].str);
                PrintPartySelectionError(gPartyMenuState.opponentMonId, 1);
                i = 2;
            }
            else if ((restriction & 0xFFFF) == 4)
            {
                PlaySE(SONG_SE_FAILURE);
                BufferString(0, (void *) gMonLinkData.monName[restriction >> 16].str);
                PrintPartySelectionError(gPartyMenuState.opponentMonId, 2);
                i = 2;
            }
            else if ((restriction & 0xFFFF) == 3)
            {
                PlaySE(SONG_SE_FAILURE);
                BufferString(0, (void *) gMonLinkData.monName[restriction >> 16].str);
                PrintPartySelectionError(gPartyMenuState.opponentMonId, 3);
                i = 2;
            }
            else
            {
                if (IsMonFainted(gPartyMenuState.selectedMon) == TRUE)
                {
                    PlaySE(SONG_SE_FAILURE);
                    PrintPartySelectionError(gPartyMenuState.selectedMon, 10);
                    i = 2;
                }
                else if (gPartyMenuState.opponentMonId == gPartyMenuState.selectedMon)
                {
                    PlaySE(SONG_SE_FAILURE);
                    PrintPartySelectionError(gPartyMenuState.selectedMon, 11);
                    i = 2;
                }
                else
                {
                    switch ((s32)GetNibbleByIndex(gMonLinkData.monSwitchStatus, gPartyMenuState.selectedMon))
                    {
                    case 0:
                        PlaySE(SONG_SE_SELECT);
                        gMonLinkData.partyActionMonId = gPartyMenuState.selectedMon;
                        gMonLinkData.battleAction = 2;
                        return -1;
                    case 1:
                        PlaySE(SONG_SE_FAILURE);
                        PrintPartySelectionError(gPartyMenuState.selectedMon, 13);
                        break;
                    case 2:
                        PlaySE(SONG_SE_FAILURE);
                        PrintPartySelectionError(gPartyMenuState.selectedMon, 11);
                        break;
                    case 3:
                        PlaySE(SONG_SE_FAILURE);
                        PrintPartySelectionError(gPartyMenuState.selectedMon, 12);
                        break;
                    }
                    i = 2;
                }
            }
            break;
        case 4:
            PlaySE(SONG_SE_SELECT);
            FadeOut();
            gPartyMenuState.selectedMon = ShowPokemonSummaryScreen(gPartyMenuState.selectedMon);
            monId = gPartyMenuState.selectedMon;
            if (gPartyMenuState.layoutId == 1)
            {
                if (gPartyMenuState.selectedMon == 0)
                    gPartyMenuState.previousSelection = 2;
                else if (gPartyMenuState.selectedMon == 1)
                    gPartyMenuState.previousSelection = 4;
            }
            InitPartyMenu(&gPartyMenuState);
            i = 0;
            break;
        }
    }
    return 0;
}
s32 HandlePartyMonSelectionMenu(s32 ret)
{
    u32 keys;
    s32 j;
    u8 monName[16];
    struct PartyMonIcon *icon = &gPartyMenuState.mons[ret];
    u32 i = 0;
    bool32 keepGoing = TRUE;

    while (TRUE)
    {
        if (!keepGoing)
            break;
        switch (i)
        {
        case 0:
            ClearWindowCharBuffer(gPartyMenuState.promptWindow, 0xFFFF);
            TextWindowFillTileBufferForText(gPartyMenuState.promptWindow);
            DrawTextWindowBorder(0, 16, 20, 4, 0xE001);
            DrawTextWindowBorder(20, 12, 10, 8, 0xE001);
            GetMonData(&gPlayerPartyPtr[ret], MON_DATA_NICKNAME, monName);
            BufferString(4, monName);
            TextWindowSetXY(gPartyMenuState.promptWindow, 0, 0);
            RenderText(gPartyMenuState.promptWindow, gText_DoWhatWithMon);
            gPartyMenuState.actionMenuWindow = AddWindow(2, &gWindowTemplatePartyActionMenu);
            SetTextColor(gPartyMenuState.actionMenuWindow, 1, 8);
            TextWindowFillTileBufferForText(gPartyMenuState.actionMenuWindow);
            ClearWindowCharBuffer(gPartyMenuState.actionMenuWindow, 0xFFFF);
            if (icon->selectionOrder == 0)
                RenderTextAt(gPartyMenuState.actionMenuWindow, GAME_LANGUAGE == LANGUAGE_JAPANESE ? 8 : 0, 0, gText_Select);
            else
                RenderTextAt(gPartyMenuState.actionMenuWindow, GAME_LANGUAGE == LANGUAGE_JAPANESE ? 8 : 0, 0, gText_Deselect);
            RenderTextAt(gPartyMenuState.actionMenuWindow, GAME_LANGUAGE == LANGUAGE_JAPANESE ? 8 : 0, 16, gText_Summary);
            RenderTextAt(gPartyMenuState.actionMenuWindow, GAME_LANGUAGE == LANGUAGE_JAPANESE ? 8 : 0, 32, gText_Cancel);
            gPartyMenuState.cursorSprite = AddSprite(0, 0, gSubspritesPartyMenuCursor);
            SetSpritePaletteNum(gPartyMenuState.cursorSprite, 5);
            gPartyMenuState.actionMenuCursor = 0;
            SetSpritePos(gPartyMenuState.cursorSprite, 168, 104);
            gBgTilemapBufferTransferScheduled[2] = TRUE;
            if (IsScreenFadedOut())
                FadeIn();
            else
                PlaySE(SONG_SE_SELECT);
            i = 1;
            break;
        case 1:
            DelayFrames(1);
            keys = gNewAndRepeatedKeys;
            if (keys & B_BUTTON)
                i = 2;
            if (keys & A_BUTTON)
            {
                if (gPartyMenuState.actionMenuCursor == 0)
                    i = 3;
                else if (gPartyMenuState.actionMenuCursor == 1)
                    i = 4;
                else
                    i = 2;
            }
            else
            {
                if (keys & DPAD_DOWN)
                {
                    if (gPartyMenuState.actionMenuCursor != 2)
                        gPartyMenuState.actionMenuCursor++;
                    else
                        gPartyMenuState.actionMenuCursor = 0;
                    PlaySE(SONG_SE_SELECT);
                }
                else if (keys & DPAD_UP)
                {
                    if (gPartyMenuState.actionMenuCursor != 0)
                        gPartyMenuState.actionMenuCursor--;
                    else
                        gPartyMenuState.actionMenuCursor = 2;
                    PlaySE(SONG_SE_SELECT);
                }
                else
                {
                    SetSpritePos(gPartyMenuState.cursorSprite, 168, 104 + (gPartyMenuState.actionMenuCursor * 16));
                }
            }
            break;
        case 2:
            PlaySE(SONG_SE_SELECT);
            if (gPartyMenuState.cursorSprite != NULL)
            {
                MoveSpriteToHead(gPartyMenuState.cursorSprite);
                gPartyMenuState.cursorSprite = NULL;
            }
            FillBgTilemapBufferRect(2, 20, 12, 10, 8, 0);
            PrintPartySelectionPrompt(0);
            // The original routine leaves the return register unspecified.
            return;
        case 3:
            if (icon->selectionOrder == 0)
            {
                if (gPartyMenuState.numMonsLeftToSelect == gPartyMenuState.numMonsToSelect)
                {
                    PlaySE(SONG_SE_FAILURE);
                    i = 1;
                }
                else
                {
                    PlaySE(SONG_SE_SELECT);
                    icon->selectionOrder = ++gPartyMenuState.numMonsLeftToSelect;
                    PrintPartySelectionOrder(icon->selectionNumberWindow, icon);
                    REG_JOY_TRANS = 0x1000000;
                    if (gPartyMenuState.numMonsLeftToSelect == gPartyMenuState.numMonsToSelect)
                        gPartyMenuState.selectedMon = 6;
                    i = 2;
                }
            }
            else
            {
                for (j = 0; j < 6; j++)
                {
                    if (gPartyMenuState.mons[j].selectionOrder > icon->selectionOrder)
                    {
                        gPartyMenuState.mons[j].selectionOrder--;
                        PrintPartySelectionOrder(gPartyMenuState.mons[j].selectionNumberWindow, &gPartyMenuState.mons[j]);
                    }
                }
                icon->selectionOrder = 0;
                PrintPartySelectionOrder(icon->selectionNumberWindow, icon);
                REG_JOY_TRANS = 0xFF000000;
                gPartyMenuState.numMonsLeftToSelect--;
                i = 2;
            }
            break;
        case 4:
            PlaySE(SONG_SE_SELECT);
            FadeOut();
            gPartyMenuState.selectedMon = ShowPokemonSummaryScreen(gPartyMenuState.selectedMon);
            if (gPartyMenuState.layoutId == 1)
            {
                if (gPartyMenuState.selectedMon == 0)
                    gPartyMenuState.previousSelection = 2;
                else if (gPartyMenuState.selectedMon == 1)
                    gPartyMenuState.previousSelection = 4;
            }
            InitPartyMenu(&gPartyMenuState);
#if !COLBTL_IS(COLBTL) && GAME_LANGUAGE != LANGUAGE_JAPANESE
            ret = gPartyMenuState.selectedMon;
            icon = &gPartyMenuState.mons[ret];
#endif
            i = 0;
            break;
        }
    }

    // The original loop has no fallthrough return value.
}

s32 HandleSinglePartySelection(void)
{
    u16 keys;
    u32 monId;
    const struct PartyMenuNavigation *navigation = gPartyMenuNavigationTables[gPartyMenuState.layoutId];
    const struct PartySlotPosition *slotPositions = gPartySlotCoordsTables[gPartyMenuState.layoutId];
    const struct PartySlotPosition *slotPosition;
    const struct PartySlotTilemapInfo *tilemapInfo;

    while (TRUE)
    {
        DelayFrames(1);
        keys = gNewAndRepeatedKeys;
        if (keys == 0)
            continue;

        if (keys & B_BUTTON)
        {
            if (gPartyMenuState.cancelDisabled == 0)
            {
                PlaySE(SONG_SE_SELECT);
                return -1;
            }
            else
            {
                PlaySE(SONG_SE_FAILURE);
                continue;
            }
        }
        if (keys & A_BUTTON)
        {
            if (gPartyMenuState.selectedMon == PARTY_SIZE)
            {
                if (gPartyMenuState.cancelDisabled == 0)
                {
                    PlaySE(SONG_SE_SELECT);
                    return -1;
                }
                else
                {
                    PlaySE(SONG_SE_FAILURE);
                    continue;
                }
            }
            else
            {
                s32 choice = 0;

                PlaySE(SONG_SE_SELECT);
                choice = HandlePartyMonActionMenu(gPartyMenuState.selectedMon);
                if (choice == -1)
                    return -1;
            }
        }

        monId = gPartyMenuState.selectedMon;
        if (keys & DPAD_UP)
        {
            PlaySE(SONG_SE_SELECT);
            if (gPartyMenuState.selectedMon == 0)
                gPartyMenuState.selectedMon = PARTY_SIZE;
            else if (gPartyMenuState.selectedMon == PARTY_SIZE)
                gPartyMenuState.selectedMon = gPartyMenuState.lastPartyMon;
            else
                gPartyMenuState.selectedMon--;
            gPartyMenuState.previousSelection = navigation[gPartyMenuState.selectedMon].previousSelection;
        }
        if (keys & DPAD_DOWN)
        {
            PlaySE(SONG_SE_SELECT);
            if (gPartyMenuState.selectedMon == gPartyMenuState.lastPartyMon)
                gPartyMenuState.selectedMon = PARTY_SIZE;
            else if (gPartyMenuState.selectedMon == PARTY_SIZE)
                gPartyMenuState.selectedMon = 0;
            else
                gPartyMenuState.selectedMon++;
            gPartyMenuState.previousSelection = navigation[gPartyMenuState.selectedMon].previousSelection;
        }
        if (keys & DPAD_LEFT)
        {
            PlaySE(SONG_SE_SELECT);
            if (navigation[gPartyMenuState.selectedMon].left != 0xFF)
            {
                gPartyMenuState.previousSelection = gPartyMenuState.selectedMon;
                gPartyMenuState.selectedMon = navigation[gPartyMenuState.selectedMon].left;
            }
        }
        if (keys & DPAD_RIGHT)
        {
            PlaySE(SONG_SE_SELECT);
            switch (navigation[gPartyMenuState.selectedMon].right)
            {
            case 0xFF:
                break;
            case 7:
                gPartyMenuState.selectedMon = gPartyMenuState.previousSelection;
                if (gPartyMenuState.selectedMon > gPartyMenuState.lastPartyMon)
                {
                    gPartyMenuState.selectedMon = gPartyMenuState.lastPartyMon;
                    if (gPartyMenuState.layoutId == 1 && gPartyMenuState.selectedMon == 1 && monId == 0)
                        gPartyMenuState.selectedMon = monId;
                }
                break;
            default:
                gPartyMenuState.selectedMon = navigation[gPartyMenuState.selectedMon].right;
                break;
            }
        }

        if (monId == gPartyMenuState.selectedMon)
            continue;

        if (monId != PARTY_SIZE)
        {
            slotPosition = &slotPositions[monId];
            tilemapInfo = &gPartySlotTilemapInfo[slotPosition->tilemapInfoId];
            if (GetMonAilment(&gPlayerPartyPtr[monId]) != STATUS_PRIMARY_FAINTED)
                SetBgTilemapBufferPaletteRect(3, slotPosition->x, slotPosition->y, tilemapInfo->width, tilemapInfo->height, 3);
            else
                SetBgTilemapBufferPaletteRect(3, slotPosition->x, slotPosition->y, tilemapInfo->width, tilemapInfo->height, 5);
        }
        else
        {
            SetBgTilemapBufferPaletteRect(1, 24, 18, 6, 2, 1);
        }

        if (gPartyMenuState.selectedMon != PARTY_SIZE)
        {
            slotPosition = &slotPositions[gPartyMenuState.selectedMon];
            tilemapInfo = &gPartySlotTilemapInfo[slotPosition->tilemapInfoId];
            if (GetMonAilment(&gPlayerPartyPtr[gPartyMenuState.selectedMon]) != STATUS_PRIMARY_FAINTED)
                SetBgTilemapBufferPaletteRect(3, slotPosition->x, slotPosition->y, tilemapInfo->width, tilemapInfo->height, 7);
            else
                SetBgTilemapBufferPaletteRect(3, slotPosition->x, slotPosition->y, tilemapInfo->width, tilemapInfo->height, 9);
        }
        else
        {
            SetBgTilemapBufferPaletteRect(1, 24, 18, 6, 2, 2);
            if (gPartyMenuState.layoutId == 0)
                gPartyMenuState.previousSelection = 1;
            else
                gPartyMenuState.previousSelection = 2;
        }

        gBgTilemapBufferTransferScheduled[3] = TRUE;
        gBgTilemapBufferTransferScheduled[1] = TRUE;
    }
    return 1;
}

inline s32 UNUSED FindMonBySelectionOrder(u32 val)
{
    s32 i;
    for (i = 0; i < PARTY_SIZE; i++)
    {
        if (gPartyMenuState.mons[i].selectionOrder == val)
            return i;
    }
    return -1;
}

inline void UNUSED SendPartySelectionOrder(void)
{
    s32 i;
    u32 bits = 0;
    u32 shift = 0;

    for (i = 1; i < gPartyMenuState.numMonsLeftToSelect + 1; i++)
    {
        s32 ret = FindMonBySelectionOrder(i);
        if (ret != -1)
            bits |= (ret << shift);
        shift += 4;
    }
    gMonLinkData.response = bits;
    gMonLinkData.responsePending = 1;
}

s32 HandleMultiPartySelection(void)
{
    u16 keys;
    u32 monId;
    const struct PartyMenuNavigation *navigation = gPartyMenuNavigationTables[gPartyMenuState.layoutId];
    const struct PartySlotPosition *slotPositions = gPartySlotCoordsTables[gPartyMenuState.layoutId];
    const struct PartySlotPosition *slotPosition;
    const struct PartySlotTilemapInfo *tilemapInfo;

    while (TRUE)
    {
        DelayFrames(1);
        keys = gNewAndRepeatedKeys;
        if (keys == 0)
            continue;

        monId = gPartyMenuState.selectedMon;
        if (keys & B_BUTTON)
        {
            if (gPartyMenuState.numMonsLeftToSelect != 0)
            {
                UndoLastPartySelection();
                PlaySE(SONG_SE_SELECT);
                REG_JOY_TRANS = 0xFF000000;
            }
            else
            {
                PrintPartySelectionPrompt(1);
                PlaySE(SONG_SE_FAILURE);
            }
        }
        if (keys & A_BUTTON)
        {
            if (gPartyMenuState.selectedMon == PARTY_SIZE)
            {
                if (gPartyMenuState.numMonsLeftToSelect != gPartyMenuState.numMonsToSelect)
                {
                    PlaySE(SONG_SE_FAILURE);
                    PrintPartySelectionPrompt(1);
                    continue;
                }
                else
                {
                    SendPartySelectionOrder();
                    return -1;
                }
            }
            else
            {
                s32 choice = HandlePartyMonSelectionMenu(gPartyMenuState.selectedMon);
                if (choice == -1)
                    return -1;
            }
        }

        if (keys & DPAD_UP)
        {
            PlaySE(SONG_SE_SELECT);
            if (gPartyMenuState.selectedMon == 0)
                gPartyMenuState.selectedMon = PARTY_SIZE;
            else if (gPartyMenuState.selectedMon == PARTY_SIZE)
                gPartyMenuState.selectedMon = gPartyMenuState.lastPartyMon;
            else
                gPartyMenuState.selectedMon--;
            gPartyMenuState.previousSelection = navigation[gPartyMenuState.selectedMon].previousSelection;
        }
        if (keys & DPAD_DOWN)
        {
            PlaySE(SONG_SE_SELECT);
            if (gPartyMenuState.selectedMon == gPartyMenuState.lastPartyMon)
                gPartyMenuState.selectedMon = PARTY_SIZE;
            else if (gPartyMenuState.selectedMon == PARTY_SIZE)
                gPartyMenuState.selectedMon = 0;
            else
                gPartyMenuState.selectedMon++;
            gPartyMenuState.previousSelection = navigation[gPartyMenuState.selectedMon].previousSelection;
        }
        if (keys & DPAD_LEFT)
        {
            PlaySE(SONG_SE_SELECT);
            if (navigation[gPartyMenuState.selectedMon].left != 0xFF)
            {
                gPartyMenuState.previousSelection = gPartyMenuState.selectedMon;
                gPartyMenuState.selectedMon = navigation[gPartyMenuState.selectedMon].left;
            }
        }
        if (keys & DPAD_RIGHT)
        {
            PlaySE(SONG_SE_SELECT);
            switch (navigation[gPartyMenuState.selectedMon].right)
            {
            case 0xFF:
                break;
            case 7:
                gPartyMenuState.selectedMon = gPartyMenuState.previousSelection;
                if (gPartyMenuState.selectedMon > gPartyMenuState.lastPartyMon)
                {
                    gPartyMenuState.selectedMon = gPartyMenuState.lastPartyMon;
                    if (gPartyMenuState.layoutId == 1 && gPartyMenuState.selectedMon == 1 && monId == 0)
                        gPartyMenuState.selectedMon = monId;
                }
                break;
            default:
                gPartyMenuState.selectedMon = navigation[gPartyMenuState.selectedMon].right;
                break;
            }
        }

        if (monId == gPartyMenuState.selectedMon)
            continue;

        if (monId != PARTY_SIZE)
        {
            slotPosition = &slotPositions[monId];
            tilemapInfo = &gPartySlotTilemapInfo[slotPosition->tilemapInfoId];
            if (GetMonAilment(&gPlayerPartyPtr[monId]) != STATUS_PRIMARY_FAINTED)
                SetBgTilemapBufferPaletteRect(3, slotPosition->x, slotPosition->y, tilemapInfo->width, tilemapInfo->height, 3);
            else
                SetBgTilemapBufferPaletteRect(3, slotPosition->x, slotPosition->y, tilemapInfo->width, tilemapInfo->height, 5);
        }
        else
        {
            SetBgTilemapBufferPaletteRect(1, 24, 18, 6, 2, 1);
        }

        if (gPartyMenuState.selectedMon != PARTY_SIZE)
        {
            slotPosition = &slotPositions[gPartyMenuState.selectedMon];
            tilemapInfo = &gPartySlotTilemapInfo[slotPosition->tilemapInfoId];
            if (GetMonAilment(&gPlayerPartyPtr[gPartyMenuState.selectedMon]) != STATUS_PRIMARY_FAINTED)
                SetBgTilemapBufferPaletteRect(3, slotPosition->x, slotPosition->y, tilemapInfo->width, tilemapInfo->height, 7);
            else
                SetBgTilemapBufferPaletteRect(3, slotPosition->x, slotPosition->y, tilemapInfo->width, tilemapInfo->height, 9);
        }
        else
        {
            SetBgTilemapBufferPaletteRect(1, 24, 18, 6, 2, 2);
            if (gPartyMenuState.layoutId == 0)
                gPartyMenuState.previousSelection = 1;
            else
                gPartyMenuState.previousSelection = 2;
        }

        gBgTilemapBufferTransferScheduled[3] = TRUE;
        gBgTilemapBufferTransferScheduled[1] = TRUE;
    }
    return 1;
}

#ifdef NONMATCHING
// Differs only in register allocation at the start. The original keeps the 0
// it stores in r5, live past the first block, which leaves &layoutId in r2
// for the layout check and gMonLinkData in r7. A literal 0 is a block-local
// constant (r0) and i is r6. Storing i gets everything after the layout
// check right.
void ShowPartyMenu(s32 mode, u32 opponentMonId, u32 cancelDisabled)
{
    s32 i, j;
    u16 maxHp;
    u16 pp;
    u32 status;

    gPartyMenuState.opponentMonId = opponentMonId;
    i = 0;
    gPartyMenuState.selectedMon = i;
    gPartyMenuState.cancelDisabled = cancelDisabled;
    gPartyMenuState.layoutId = i;

    if (gMonLinkData.partyMenuLayout == 1)
    {
        gPartyMenuState.previousSelection = 1;
    }
    else if (gMonLinkData.partyMenuLayout == 2)
    {
        gPartyMenuState.layoutId = 1;
        gPartyMenuState.previousSelection = 2;
    }

    switch (mode)
    {
    case 0:
        gPartyMenuState.isMultiSelect = 0;
        gPartyMenuState.numMonsToSelect = 0;
        InitPartyMenu(&gPartyMenuState);
        FadeIn();
        HandleSinglePartySelection();
        FadeOut();
        break;
    case 1:
        gPartyMenuState.isMultiSelect = 1;
        gPartyMenuState.numMonsToSelect = gMonLinkData.numMonsToSelect;
        if (gPartyMenuState.numMonsToSelect == 0 || gPartyMenuState.numMonsToSelect > *gPlayerPartyCountPtr)
            gPartyMenuState.numMonsToSelect = *gPlayerPartyCountPtr;
        gPartyMenuState.numMonsLeftToSelect = 0;
        for (i = 0; i < PARTY_SIZE; i++)
        {
            gPartyMenuState.mons[i].selectionOrder = 0;
            maxHp = GetMonData(&gPlayerPartyPtr[i], MON_DATA_MAX_HP, NULL);
            SetMonData(&gPlayerPartyPtr[i], MON_DATA_HP, &maxHp);
            status = 0;
            SetMonData(&gPlayerPartyPtr[i], MON_DATA_STATUS, &status);
            for (j = 0; j < 4; j++)
            {
                u16 move = GetBoxMonMoveBySlot(&gPlayerPartyPtr[i].box, j);
                u32 ppBonus = GetMonData(&gPlayerPartyPtr[i], MON_DATA_PP_BONUSES, NULL);
                pp = CalculatePPWithBonus(move, ppBonus, j);
                SetMonData(&gPlayerPartyPtr[i], MON_DATA_PP1 + j, &pp);
            }
        }
        InitPartyMenu(&gPartyMenuState);
        FadeIn();
        HandleMultiPartySelection();
        FadeOut();
        break;
    }
}
#else
void NAKED ShowPartyMenu(s32 mode, u32 opponentMonId, u32 cancelDisabled)
{
    asm_unified("	push {r4, r5, r6, r7, lr}\t\n\
	mov r7, sl\t\n\
	mov r6, sb\t\n\
	mov r5, r8\t\n\
	push {r5, r6, r7}\t\n\
	sub sp, #0xc\t\n\
	adds r4, r0, #0\t\n\
	ldr r3, =gPartyMenuState\t\n\
	movs r5, #0x91\t\n\
	lsls r5, r5, #1\t\n\
	adds r0, r3, r5\t\n\
	movs r5, #0\t\n\
	strb r1, [r0]\t\n\
	ldr r1, =0x0000011B\t\n\
	adds r0, r3, r1\t\n\
	strb r5, [r0]\t\n\
	adds r1, #0xa\t\n\
	adds r0, r3, r1\t\n\
	strb r2, [r0]\t\n\
	movs r0, #0x8d\t\n\
	lsls r0, r0, #1\t\n\
	adds r2, r3, r0\t\n\
	strb r5, [r2]\t\n\
	ldr r1, =gMonLinkData\t\n\
	ldr r0, =0x00000858\t\n\
	adds r6, r1, r0\t\n\
	ldrb r0, [r6]\t\n\
	adds r7, r1, #0\t\n\
	cmp r0, #1\t\n\
	bne _02002AF4\t\n\
	movs r1, #0x8f\t\n\
	lsls r1, r1, #1\t\n\
	adds r2, r3, r1\t\n\
	strb r0, [r2]\t\n\
	b _02002B08\t\n\
	.align 2, 0\t\n\
	.pool\t\n\
_02002AF4:\t\n\
	ldrb r0, [r6]\t\n\
	adds r1, r0, #0\t\n\
	cmp r1, #2\t\n\
	bne _02002B08\t\n\
	movs r0, #1\t\n\
	strb r0, [r2]\t\n\
	movs r2, #0x8f\t\n\
	lsls r2, r2, #1\t\n\
	adds r0, r3, r2\t\n\
	strb r1, [r0]\t\n\
_02002B08:\t\n\
	cmp r4, #0\t\n\
	beq _02002B12\t\n\
	cmp r4, #1\t\n\
	beq _02002B38\t\n\
	b _02002C1E\t\n\
_02002B12:\t\n\
	movs r5, #0x8c\t\n\
	lsls r5, r5, #1\t\n\
	adds r0, r3, r5\t\n\
	strb r4, [r0]\t\n\
	ldr r1, =0x00000123\t\n\
	adds r0, r3, r1\t\n\
	strb r4, [r0]\t\n\
	adds r0, r3, #0\t\n\
	bl InitPartyMenu\t\n\
	bl FadeIn\t\n\
	bl HandleSinglePartySelection\t\n\
	bl FadeOut\t\n\
	b _02002C1E\t\n\
	.align 2, 0\t\n\
	.pool\t\n\
_02002B38:\t\n\
	movs r2, #0x8c\t\n\
	lsls r2, r2, #1\t\n\
	adds r0, r3, r2\t\n\
	strb r4, [r0]\t\n\
	ldr r5, =0x00000859\t\n\
	adds r0, r7, r5\t\n\
	ldrb r0, [r0]\t\n\
	ldr r1, =0x00000123\t\n\
	adds r4, r3, r1\t\n\
	strb r0, [r4]\t\n\
	lsls r0, r0, #0x18\t\n\
	lsrs r1, r0, #0x18\t\n\
	ldr r2, =gPlayerPartyCountPtr\t\n\
	cmp r1, #0\t\n\
	beq _02002B5E\t\n\
	ldr r0, [r2]\t\n\
	ldrb r0, [r0]\t\n\
	cmp r1, r0\t\n\
	bls _02002B64\t\n\
_02002B5E:\t\n\
	ldr r0, [r2]\t\n\
	ldrb r0, [r0]\t\n\
	strb r0, [r4]\t\n\
_02002B64:\t\n\
	movs r2, #0x92\t\n\
	lsls r2, r2, #1\t\n\
	adds r0, r3, r2\t\n\
	movs r1, #0\t\n\
	strb r1, [r0]\t\n\
	movs r6, #0\t\n\
	mov r5, sp\t\n\
	adds r5, #4\t\n\
	str r5, [sp, #8]\t\n\
	mov r0, sp\t\n\
	adds r0, #2\t\n\
	mov sl, r6\t\n\
	ldr r7, =gPlayerPartyPtr\t\n\
	mov sb, r0\t\n\
_02002B80:\t\n\
	movs r0, #0x2c\t\n\
	muls r0, r6, r0\t\n\
	adds r0, r0, r3\t\n\
	adds r0, #0x22\t\n\
	mov r1, sl\t\n\
	strb r1, [r0]\t\n\
	movs r2, #0x64\t\n\
	adds r4, r6, #0\t\n\
	muls r4, r2, r4\t\n\
	ldr r0, [r7]\t\n\
	adds r0, r0, r4\t\n\
	movs r1, #0x3a\t\n\
	movs r2, #0\t\n\
	bl GetMonData\t\n\
	mov r1, sp\t\n\
	strh r0, [r1]\t\n\
	ldr r0, [r7]\t\n\
	adds r0, r0, r4\t\n\
	movs r1, #0x39\t\n\
	mov r2, sp\t\n\
	bl SetMonData\t\n\
	mov r5, sl\t\n\
	str r5, [sp, #4]\t\n\
	ldr r0, [r7]\t\n\
	adds r0, r0, r4\t\n\
	movs r1, #0x37\t\n\
	ldr r2, [sp, #8]\t\n\
	bl SetMonData\t\n\
	movs r5, #0\t\n\
	adds r6, #1\t\n\
	mov r8, r6\t\n\
	adds r6, r4, #0\t\n\
_02002BC6:\t\n\
	ldr r0, [r7]\t\n\
	adds r0, r0, r6\t\n\
	adds r1, r5, #0\t\n\
	bl GetBoxMonMoveBySlot\t\n\
	adds r4, r0, #0\t\n\
	lsls r4, r4, #0x10\t\n\
	lsrs r4, r4, #0x10\t\n\
	ldr r0, [r7]\t\n\
	adds r0, r0, r6\t\n\
	movs r1, #0x15\t\n\
	movs r2, #0\t\n\
	bl GetMonData\t\n\
	adds r1, r0, #0\t\n\
	adds r0, r4, #0\t\n\
	adds r2, r5, #0\t\n\
	bl CalculatePPWithBonus\t\n\
	mov r1, sb\t\n\
	strh r0, [r1]\t\n\
	ldr r0, [r7]\t\n\
	adds r0, r0, r6\t\n\
	adds r1, r5, #0\t\n\
	adds r1, #0x11\t\n\
	mov r2, sb\t\n\
	bl SetMonData\t\n\
	adds r5, #1\t\n\
	cmp r5, #3\t\n\
	ble _02002BC6\t\n\
	mov r6, r8\t\n\
	ldr r3, =gPartyMenuState\t\n\
	cmp r6, #5\t\n\
	ble _02002B80\t\n\
	adds r0, r3, #0\t\n\
	bl InitPartyMenu\t\n\
	bl FadeIn\t\n\
	bl HandleMultiPartySelection\t\n\
	bl FadeOut\t\n\
_02002C1E:\t\n\
	add sp, #0xc\t\n\
	pop {r3, r4, r5}\t\n\
	mov r8, r3\t\n\
	mov sb, r4\t\n\
	mov sl, r5\t\n\
	pop {r4, r5, r6, r7}\t\n\
	pop {r0}\t\n\
	bx r0\t\n\
	.align 2, 0\t\n\
	.pool");
}
#endif

void ResetPartyMenuState(void)
{
    CpuFill16(0, &gPartyMenuState, sizeof(gPartyMenuState));
    gPartyMenuState.layoutId = 0;
    gPartyMenuState.selectedMon = 0;
    gPartyMenuState.previousSelection = 1;
}

void ResetPartyMenuCursor(void)
{
    gPartyMenuState.selectedMon = 0;
    if (gPartyMenuState.layoutId == 0)
        gPartyMenuState.previousSelection = 1;
    else
        gPartyMenuState.previousSelection = 2;
}
