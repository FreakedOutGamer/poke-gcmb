#include "global.h"
#include "pokedex.h"
#include "colbtl_version.h"
#include "gflib.h"
#include "colbtl.h"
#include "strings.h"
#include "graphics.h"
#include "string_util.h"
#include "libpmagb/pic_uncomp.h"
#include "constants/moves.h"
#include "constants/songs.h"
#include "party_menu.h"
#include "render_text.h"
#include "window.h"
#include "battle_menu.h"
#include "fade.h"
#include "link_transfer.h"

extern const u8 gText_BattleType[];

extern const struct Window gWindowTemplateBattleMonInfo;
extern const struct Window gWindowTemplateBattleMessage;

extern const struct Subsprite *gBattleMonSubspriteTables[];
extern const struct Subsprite gSubspriteBattleStatus[];

struct BattleMenuState
{
    struct Sprite *monSprite;
    struct Sprite *statusSprite;
    struct Sprite *cursorSprite;
    u16 species;
    u16 displaySpecies;
    u8 selectedOption;
    u8 fill11[3];
    struct Window *infoWindow;
    struct Window *messageWindow;
};

extern struct BattleMenuState gBattleMenuState;

struct StrCoords
{
    u8 x;
    u8 y;
    u8 result;
    const u8 *str;
    void (*callback)(s32 id);
};

extern const struct Subsprite gSubspritesBattleMenuCursor[];
extern const struct Subsprite gSubspritesMoveMenuCursor[];
extern const struct Subsprite gSubspritesTargetMenuCursor[];
extern const struct Subsprite gSubspritesGiveInMenuCursor[];

extern const struct StrCoords gText_BattleOptions[];
extern const struct StrCoords gMoveMenuOptions[];
extern const struct StrCoords gTargetMenuOptions[];
extern const struct StrCoords gGiveInMenuOptions[];

extern u32 gBattleMenuMonId;

extern u8 gLinkMonNameBuffer[];
extern u8 gStringBuffers[][32];

struct BattleMenuInputState
{
    u8 state;
    u8 unused;
    u8 moveSlot;
};

extern struct BattleMenuInputState gBattleMenuInputState;

void InitBattleMenu(void)
{
#if COLBTL_IS(COLBTL_F) || COLBTL_IS(COLBTL_G) || COLBTL_IS(COLBTL_I) || COLBTL_IS(COLBTL_S)
    u16 *tilemap;
    s32 count;
#endif
    ClearVram();
    REG_DISPCNT = DISPCNT_BG_ALL_ON | DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP;
    REG_BG0CNT = BGCNT_PRIORITY(0) | BGCNT_CHARBASE(0) | BGCNT_SCREENBASE(27);
    REG_BG1CNT = BGCNT_PRIORITY(1) | BGCNT_CHARBASE(0) | BGCNT_SCREENBASE(15);
    REG_BG2CNT = BGCNT_PRIORITY(2) | BGCNT_CHARBASE(2) | BGCNT_SCREENBASE(28) | BGCNT_TXT512x256;
    REG_BG3CNT = BGCNT_PRIORITY(3) | BGCNT_CHARBASE(2) | BGCNT_SCREENBASE(30);
    AutoUnCompVram(gTextPal, (void *) PLTT);
    AutoUnCompVram(gMonInfoIcons_Gfx, (void *) VRAM);
    AutoUnCompVram(gBattleScreen_Gfx, (void *) VRAM + 0x8000);
    AutoUnCompVram(gBattleScreen_Pal, (void *) PLTT + 0x140);

    AutoUnCompVram(gBattleScreen_Tilemap, BG_TILEMAP_BUFFER(3));
    AutoUnCompVram(gBattleWindows_Tilemap, BG_TILEMAP_BUFFER(2) + BG_SCREEN_SIZE);

#if COLBTL_IS(COLBTL_F) || COLBTL_IS(COLBTL_G) || COLBTL_IS(COLBTL_I) || COLBTL_IS(COLBTL_S)
    // These releases share the Japanese tilemaps, so move the battle screen
    // to palette 10 and shift the window tiles to this language's graphics.
    tilemap = (u16 *)BG_TILEMAP_BUFFER(3);
    for (count = 0; count < 0x400; count++, tilemap++)
        *tilemap += 0xA000;
    tilemap = (u16 *)(BG_TILEMAP_BUFFER(2) + BG_SCREEN_SIZE);
    for (count = 0; count < 0x400; count++, tilemap++)
        if (*tilemap & 0x3FF)
            *tilemap += GAME_LANGUAGE == LANGUAGE_GERMAN ? -1 : GAME_LANGUAGE == LANGUAGE_ITALIAN ? 5 : GAME_LANGUAGE == LANGUAGE_SPANISH ? 10 : 8;
#endif

    gBgTilemapBufferTransferScheduled[2] = TRUE;
    gBgTilemapBufferTransferScheduled[3] = TRUE;

    AutoUnCompVram(gTypeStatusIcons_Pal, (void *) PLTT + 0x200);
    AutoUnCompVram(gTypeStatusIcons_Gfx, (void *) VRAM + 0x10000);

    *(u16 *)(PLTT) = 0x5108;

    CpuFill16(0, &gBattleMenuState, sizeof(gBattleMenuState));

    gBattleMenuState.infoWindow = AddWindow(0, &gWindowTemplateBattleMonInfo);
    gBattleMenuState.messageWindow = AddWindow(1, &gWindowTemplateBattleMessage);

    SetTextColor(gBattleMenuState.infoWindow, 1, 8);
    SetTextColor(gBattleMenuState.messageWindow, 1, 8);
}

void CreateBattleMonSprite(struct BattleMenuState *menu, u32 monId, s32 x, s32 y)
{
    void *buffer;
    u32 personality;
    const struct CompressedSpriteSheet *monFrontSheet;
    const struct SpeciesInfo *speciesInfo = gAgbPmRomParams->speciesInfo;

    menu->species = gMonLinkData.species;
    if (menu->species == SPECIES_NONE)
        return;

    personality = gMonLinkData.personality;
    menu->displaySpecies = GetIconSpecies(menu->species, personality);
    if (menu->monSprite != NULL)
        MoveSpriteToHead(menu->monSprite);

    menu->monSprite = AddSprite(x, y, gBattleMonSubspriteTables[speciesInfo[menu->species].noFlip]);
    SetSpritePaletteNum(menu->monSprite, 4);
    monFrontSheet = &gAgbPmRomParams->monFrontPics[menu->displaySpecies];
    buffer = GetPicUncompPtr();
    LZ77UnCompVram(monFrontSheet->data, buffer);
    if (menu->species == SPECIES_CASTFORM)
        buffer += (gMonLinkData.castformForm * 0x800);
    DrawSpindaSpots(menu->species, personality, buffer);
    CpuCopy16(buffer, (void *)VRAM + 0x12000, 0x800);
    LZ77UnCompWram(GetMonSpritePalStructFromOtIdPersonality(menu->species, gMonLinkData.otId, personality)->data, buffer);
    if (menu->species == SPECIES_CASTFORM)
        buffer += (gMonLinkData.castformForm * 32);
    CpuCopy16(buffer, (void *)PLTT + 0x280, 0x20);
}

void PrintBattleMonGender(struct Pokemon *mon)
{
    u8 text[16];
    u32 gender = GetBoxMonGender(mon);
    if (gender == MON_MALE || gender == MON_FEMALE)
    {
        text[1] = EOS;
        TextWindowSetXY(gBattleMenuState.infoWindow, GAME_LANGUAGE == LANGUAGE_JAPANESE ? 48 : 72, 16);
        if (gender == MON_MALE)
        {
            text[0] = CHAR_MALE;
            SetTextColor(gBattleMenuState.infoWindow, 4, 5);
        }
        else
        {
            text[0] = CHAR_FEMALE;
            SetTextColor(gBattleMenuState.infoWindow, 7, 8);
        }
        RenderText(gBattleMenuState.infoWindow, text);
        SetBgTilemapBufferPaletteRect(0, GAME_LANGUAGE == LANGUAGE_JAPANESE ? 17 : 20, 8, 1, 2, 0);
        SetTextColor(gBattleMenuState.infoWindow, 1, 8);
    }
}

void DrawBattleMonInfo(u32 monId)
{
    s32 i;
    u8 text[32];
    u32 level, species;
    s32 value;
    struct Pokemon *mon = &gPlayerPartyPtr[monId];

    CreateBattleMonSprite(&gBattleMenuState, monId, 8, 32);
#if GAME_LANGUAGE == LANGUAGE_JAPANESE || COLBTL_IS(COLBTL)
    SetBgTilemapBufferTileAt(0, 12, 6, 0xF014);
    SetBgTilemapBufferTileAt(0, 12, 7, 0xF013);
#else
    text[0] = 0x34;
    text[1] = EOS;
    TextWindowSetXY(gBattleMenuState.infoWindow, 8, 0);
    RenderText(gBattleMenuState.infoWindow, text);
#endif

    level = GetMonData(mon, MON_DATA_LEVEL, NULL);
    NumToPmString3CustomZeroChar(level, text, CHAR_0);
    for (i = 0; text[i] == CHAR_0; i++)
        ;
    if (text[i] == EOS)
        i--;
    TextWindowSetXY(gBattleMenuState.infoWindow, (COLBTL_IS(COLBTL) || GAME_LANGUAGE == LANGUAGE_JAPANESE) ? 16 : 17, 0);
    RenderText(gBattleMenuState.infoWindow, text + i);

    GetMonData(mon, MON_DATA_NICKNAME, text);
    BufferString(4, text);
    TextWindowSetXY(gBattleMenuState.infoWindow, 44, 0);
    RenderText(gBattleMenuState.infoWindow, text);

    species = GetMonData(mon, MON_DATA_SPECIES, NULL);
    if (species != SPECIES_NIDORAN_M && species != SPECIES_NIDORAN_F)
        PrintBattleMonGender(mon);

    GetSpeciesName(text, species);
    TextWindowSetXY(gBattleMenuState.infoWindow, 8, 16);
    RenderText(gBattleMenuState.infoWindow, text);

    value = GetMonAilment(mon);
    if (value != STATUS_PRIMARY_NONE)
    {
        if (gBattleMenuState.statusSprite == NULL)
            gBattleMenuState.statusSprite = AddSprite(GAME_LANGUAGE == LANGUAGE_JAPANESE ? 150 : 166, 68, gSubspriteBattleStatus);
        SetSpriteInvisible(gBattleMenuState.statusSprite, FALSE);
        SetSpriteTileOffset(gBattleMenuState.statusSprite, (value - 1) * 4);
    }

    DrawPartyMonHealthBar(1, 12, 10, monId);
    SetBgPos(1, 0, -4);
    gBgTilemapBufferTransferScheduled[1] = TRUE;

    value = GetMonData(mon, MON_DATA_HP, NULL);
    NumToPmString3RightAlign(text, value);
    text[3] = CHAR_SLASH;
    value = GetMonData(mon, MON_DATA_MAX_HP, NULL);
    NumToPmString3RightAlign(text + 4, value);
    TextWindowSetXY(gBattleMenuState.infoWindow, 80, 32);
#if GAME_LANGUAGE != LANGUAGE_JAPANESE
        gBattleMenuState.infoWindow->glyphWidth = 6;
#endif
    RenderText(gBattleMenuState.infoWindow, text);
    gBgTilemapBufferTransferScheduled[0] = TRUE;
}

static inline void PrintMenuOptionsInline(struct Window *window, const struct StrCoords *strInfo, s32 count)
{
    s32 i;

    for (i = 0; i < count; i++, strInfo++)
    {
        s32 x = strInfo->x - window->left * 8;
        s32 y = strInfo->y - window->top * 8;
        RenderTextAt(window, x + (GAME_LANGUAGE == LANGUAGE_JAPANESE ? 8 : 0), y, strInfo->str);
    }
}

#if GAME_LANGUAGE == LANGUAGE_JAPANESE
static inline
#endif
void PrintMenuOptions(struct Window *window, const struct StrCoords *strInfo, s32 count)
{
    PrintMenuOptionsInline(window, strInfo, count);

#if GAME_LANGUAGE != LANGUAGE_JAPANESE
    if (gBattleMenuState.cursorSprite != NULL)
    {
        MoveSpriteToHead(gBattleMenuState.cursorSprite);
        gBattleMenuState.cursorSprite = NULL;
    }

#else
    if (gBattleMenuState.cursorSprite == NULL)
    {
#endif
    if (strInfo == gText_BattleOptions)
        gBattleMenuState.cursorSprite = AddSprite(0, 0, gSubspritesBattleMenuCursor);
    else if (strInfo == gMoveMenuOptions)
        gBattleMenuState.cursorSprite = AddSprite(0, 0, gSubspritesMoveMenuCursor);
    else if (strInfo == gTargetMenuOptions)
        gBattleMenuState.cursorSprite = AddSprite(0, 0, gSubspritesTargetMenuCursor);
    else if (strInfo == gGiveInMenuOptions)
        gBattleMenuState.cursorSprite = AddSprite(0, 0, gSubspritesGiveInMenuCursor);

    SetSpritePaletteNum(gBattleMenuState.cursorSprite, 5);
#if GAME_LANGUAGE == LANGUAGE_JAPANESE
    }
#endif
    SetSpritePos(gBattleMenuState.cursorSprite, strInfo[0].x, strInfo[0].y);
}

static inline void PrintJapaneseMenuOptions(struct Window *window, const struct StrCoords *strInfo, s32 count, u32 cursorKind)
{
    PrintMenuOptionsInline(window, strInfo, count);
    if (gBattleMenuState.cursorSprite == NULL)
    {
        gBattleMenuState.cursorSprite = AddSprite(0, 0,
            cursorKind == 0 ? gSubspritesBattleMenuCursor :
            cursorKind == 1 ? gSubspritesMoveMenuCursor :
            cursorKind == 2 ? gSubspritesTargetMenuCursor : gSubspritesGiveInMenuCursor);
        SetSpritePaletteNum(gBattleMenuState.cursorSprite, 5);
    }
    SetSpritePos(gBattleMenuState.cursorSprite, strInfo[0].x, strInfo[0].y);
}

#if GAME_LANGUAGE == LANGUAGE_JAPANESE
void UNUSED PrintBattleMenuOptions(struct Window *window, const struct StrCoords *strInfo, s32 count)
{
    PrintJapaneseMenuOptions(window, strInfo, count, 0);
}
#endif

s32 HandleMenuInput(const struct StrCoords *strInfo, s32 count)
{
    u32 keys;
    const struct StrCoords *curr;
    s32 i, previousId;

    for (i = 0; strInfo[i].str[0] == EOS; i++)
        ;

    if (GAME_LANGUAGE != LANGUAGE_JAPANESE && strInfo == gTargetMenuOptions && i == 1 && strInfo[2].str[0] != EOS)
        i = 2;

    if (i < count)
    {
        SetSpritePos(gBattleMenuState.cursorSprite, strInfo[i].x, strInfo[i].y);
        curr = &strInfo[i];
        while (1)
        {
            DelayFrames(1);
            keys = gNewKeys;
            previousId = i;
            if (keys & DPAD_RIGHT)
            {
                if (i == 0)
                {
                    if (strInfo[1].str[0] != EOS && (COLBTL_IS(COLBTL) || GAME_LANGUAGE == LANGUAGE_JAPANESE || strInfo[1].str[0] != CHAR_HYPHEN))
                    {
                        curr = &strInfo[1];
                        i = 1;
                    }
                    else if (strInfo[3].str[0] != EOS && (COLBTL_IS(COLBTL) || GAME_LANGUAGE == LANGUAGE_JAPANESE || strInfo[3].str[0] != CHAR_HYPHEN))
                    {
                        curr = &strInfo[3];
                        i = 3;
                    }
                }
                if (i == 2)
                {
                    if (strInfo[3].str[0] != EOS && (COLBTL_IS(COLBTL) || GAME_LANGUAGE == LANGUAGE_JAPANESE || strInfo[3].str[0] != CHAR_HYPHEN))
                    {
                        curr = &strInfo[3];
                        i = 3;
                    }
                    else if (strInfo[1].str[0] != EOS && (COLBTL_IS(COLBTL) || GAME_LANGUAGE == LANGUAGE_JAPANESE || strInfo[1].str[0] != CHAR_HYPHEN))
                    {
                        curr = &strInfo[1];
                        i = 1;
                    }
                }
            }
            if (keys & DPAD_LEFT)
            {
                if (i == 1)
                {
                    if (strInfo[0].str[0] != EOS && (COLBTL_IS(COLBTL) || GAME_LANGUAGE == LANGUAGE_JAPANESE || strInfo[0].str[0] != CHAR_HYPHEN))
                    {
                        curr = &strInfo[0];
                        i = 0;
                    }
                    else if (strInfo[2].str[0] != EOS && (COLBTL_IS(COLBTL) || GAME_LANGUAGE == LANGUAGE_JAPANESE || strInfo[2].str[0] != CHAR_HYPHEN))
                    {
                        curr = &strInfo[2];
                        i = 2;
                    }
                }
                if (i == 3)
                {
                    if (strInfo[2].str[0] != EOS && (COLBTL_IS(COLBTL) || GAME_LANGUAGE == LANGUAGE_JAPANESE || strInfo[2].str[0] != CHAR_HYPHEN))
                    {
                        curr = &strInfo[2];
                        i = 2;
                    }
                    else if (strInfo[0].str[0] != EOS && (COLBTL_IS(COLBTL) || GAME_LANGUAGE == LANGUAGE_JAPANESE || strInfo[0].str[0] != CHAR_HYPHEN))
                    {
                        curr = &strInfo[0];
                        i = 0;
                    }
                }
            }
            if (keys & DPAD_DOWN)
            {
                if (i + 2 < count && curr[2].str[0] != EOS)
                {
                    curr += 2;
                    i += 2;
                }
            }
            if (keys & DPAD_UP)
            {
                if (i - 2 >= 0 && curr[-2].str[0] != EOS)
                {
                    curr -= 2;
                    i -= 2;
                }
            }

            if (previousId != i)
            {
                PlaySE(SONG_SE_SELECT);
                SetSpritePos(gBattleMenuState.cursorSprite, curr->x, curr->y);
                if (curr->callback != NULL)
                    curr->callback(i);
            }

            if (keys & B_BUTTON)
            {
                PlaySE(SONG_SE_SELECT);
                return 0;
            }
            if (keys & A_BUTTON)
            {
                PlaySE(SONG_SE_SELECT);
                gBattleMenuState.selectedOption = i;
                return curr->result;
            }
        }
    }
    return 0;
}

void PrintMoveTypeAndPP(s32 id)
{
    u8 text[16];
    s32 ppBonuses, ppTotal, ppCurr, type;
    struct Pokemon *mon = &gPlayerPartyPtr[gBattleMenuMonId];
    const struct BattleMove *moves = gAgbPmRomParams->moves;
    u32 move = gMonLinkData.moves[id];

    FillWindowCharBufferRect(gBattleMenuState.messageWindow, GAME_LANGUAGE == LANGUAGE_JAPANESE ? 24 : 22, 2, GAME_LANGUAGE == LANGUAGE_JAPANESE ? 4 : 6, 2, 0);
    type = moves[move].type;
    RenderTextAt(gBattleMenuState.messageWindow, GAME_LANGUAGE == LANGUAGE_JAPANESE ? 192 : 176, 16, gTypeNames[type]);

    FillWindowCharBufferRect(gBattleMenuState.messageWindow, GAME_LANGUAGE == LANGUAGE_JAPANESE ? 23 : 24, 0, GAME_LANGUAGE == LANGUAGE_JAPANESE ? 5 : 4, 2, 0);
    ppCurr = gMonLinkData.pps[id];
    ppBonuses = GetMonData(mon, MON_DATA_PP_BONUSES, NULL);
    ppTotal = CalculatePPWithBonus(move, ppBonuses, id);
    NumToPmString3RightAlign(text, ppCurr);
    text[3] = CHAR_SLASH;
    NumToPmString3RightAlign(text + 4, ppTotal);
    text[4] = text[5];
    text[5] = text[6];
    text[6] = text[7];
    RenderTextAt(gBattleMenuState.messageWindow, GAME_LANGUAGE == LANGUAGE_JAPANESE ? 176 : 191, 0, text);
}

s32 DrawMoveSelectionMenu(s32 monId)
{
    u8 text[16];
    s32 i, count;

    CopyRectWithinBgTilemapBuffer(2, 0, 38, 30, 6, 0, 14);
    FillBgTilemapBufferRect(3, 0, 14, 30, 6, 0);
    ClearWindowCharBuffer(gBattleMenuState.messageWindow, 0);
    RenderTextAt(gBattleMenuState.messageWindow, GAME_LANGUAGE == LANGUAGE_JAPANESE ? 144 : 176, 0, gText_PP);
    if (GAME_LANGUAGE == LANGUAGE_JAPANESE)
        RenderTextAt(gBattleMenuState.messageWindow, 144, 16, gText_BattleType);
    PrintMoveTypeAndPP(0);

    for (count = 0, i = 0; i < MAX_MON_MOVES; i++)
    {
        u32 move = gMonLinkData.moves[i];
        if (move != MOVE_NONE)
        {
            CopyMoveName(text, move);
            count++;
        }
        else
        {
            StringCopy(text, gText_1Dash);
        }
        BufferString(i, text);
    }

    if (GAME_LANGUAGE == LANGUAGE_JAPANESE)
        PrintJapaneseMenuOptions(gBattleMenuState.messageWindow, gMoveMenuOptions, 4, 1);
    else
    PrintMenuOptions(gBattleMenuState.messageWindow, gMoveMenuOptions, 4);
    gBgTilemapBufferTransferScheduled[2] = TRUE;
    gBgTilemapBufferTransferScheduled[3] = TRUE;

    return count;
}

u8 *FormatLinkMonName(s32 id)
{
    u8 UNUSED unusedVar[4];
    u8 text[16];
    s32 len, i;
    u8 *txtPtr = gLinkMonNameBuffer;
#if GAME_LANGUAGE != LANGUAGE_JAPANESE
    bool8 isJapanese;
#endif

    CpuFill16(0, txtPtr, 0x20);
    if (gMonLinkData.monName[id].str[0] == EOS)
    {
        txtPtr[0] = EOS;
    }
    else
    {
#if COLBTL_IS(COLBTL_G) || COLBTL_IS(COLBTL_S)
        txtPtr[0] = GAME_LANGUAGE == LANGUAGE_GERMAN ? CHAR_S : CHAR_J;
        txtPtr[1] = GAME_LANGUAGE == LANGUAGE_GERMAN ? CHAR_P : CHAR_PERIOD;
        txtPtr[2] = gMonLinkData.monName[id].playerId + CHAR_0;
        txtPtr[3] = CHAR_COLON;
        txtPtr[4] = CHAR_SPACE;
        len = 5;
#else
#if COLBTL_IS(COLBTL_F) || COLBTL_IS(COLBTL_I)
        txtPtr[0] = GAME_LANGUAGE == LANGUAGE_FRENCH ? CHAR_J : CHAR_G;
        txtPtr[1] = gMonLinkData.monName[id].playerId + CHAR_0;
#else
        txtPtr[0] = gMonLinkData.monName[id].playerId + CHAR_0;
        txtPtr[1] = CHAR_P;
#endif
        if (GAME_LANGUAGE != LANGUAGE_JAPANESE)
        {
            txtPtr[2] = CHAR_COLON;
            txtPtr[3] = CHAR_SPACE;
        }
        len = GAME_LANGUAGE == LANGUAGE_JAPANESE ? 2 : 4;
#endif
#if GAME_LANGUAGE != LANGUAGE_JAPANESE
        isJapanese = gMonLinkData.monName[id].isJapanese;
        if (isJapanese)
        {
            txtPtr[len] = EXT_CTRL_CODE_BEGIN;
            txtPtr[len + 1] = EXT_CTRL_CODE_JPN;
            len += 2;
        }
#endif
#if GAME_LANGUAGE == LANGUAGE_JAPANESE
        // Keep the short-lived link-data address out of r9; emits no instructions.
        asm("" ::: "r9");
#endif
        for (i = 0; i < gAgbPmRomParams->pokemonNameLength2; i++)
        {
            if (gMonLinkData.monName[id].str[i] == EOS)
                break;
            if (GAME_LANGUAGE == LANGUAGE_JAPANESE)
                txtPtr[i + 2] = gMonLinkData.monName[id].str[i];
            else
                txtPtr[i + len] = gMonLinkData.monName[id].str[i];
        }
        i += len;
        if (gMonLinkData.monName[id].isNidoranM || gMonLinkData.monName[id].isNidoranF)
        {
            u32 species;

            if (gMonLinkData.monName[id].isNidoranM)
                species = SPECIES_NIDORAN_M;
            else
                species = SPECIES_NIDORAN_F;
            GetSpeciesName(text, species);
#if COLBTL_IS(COLBTL) || COLBTL_IS(COLBTL_E) || GAME_LANGUAGE == LANGUAGE_JAPANESE
            if (StringCompare((u8 *)gMonLinkData.monName[id].str, GAME_LANGUAGE == LANGUAGE_JAPANESE ? unusedVar : text) == 0)
            {
                if (gMonLinkData.monName[id].gender == GENDER_MALE)
                    txtPtr[i++] = CHAR_MALE;
                else if (gMonLinkData.monName[id].gender == GENDER_FEMALE)
                    txtPtr[i++] = CHAR_FEMALE;
            }
#endif
        }
#if COLBTL_IS(COLBTL) || COLBTL_IS(COLBTL_E) || GAME_LANGUAGE == LANGUAGE_JAPANESE
        else
        {
            if (gMonLinkData.monName[id].gender == GENDER_MALE)
                txtPtr[i++] = CHAR_MALE;
            else if (gMonLinkData.monName[id].gender == GENDER_FEMALE)
                txtPtr[i++] = CHAR_FEMALE;
        }
#endif
#if GAME_LANGUAGE == LANGUAGE_JAPANESE
        txtPtr[i++] = CHAR_L;
        txtPtr[i++] = CHAR_v;
        NumToPmString3CustomZeroChar(gMonLinkData.monName[id].level, unusedVar, CHAR_0);
        if (unusedVar[0] != CHAR_0)
        {
            txtPtr[i++] = unusedVar[0];
            txtPtr[i++] = unusedVar[1];
        }
        else if (unusedVar[1] != CHAR_0)
            txtPtr[i++] = unusedVar[1];
        txtPtr[i++] = unusedVar[2];
#endif
        txtPtr[i] = EOS;
    }

    return txtPtr;
}

s32 DrawTargetSelectionMenu(void)
{
    u8 text[1];

    text[0] = EOS;
    CopyRectWithinBgTilemapBuffer(3, 0, 20, 30, 6, 0, 14);
    FillBgTilemapBufferRect(2, 0, 14, 30, 6, 0);
    ClearWindowCharBuffer(gBattleMenuState.messageWindow, 0xFFFF);
    StringCopy(gStringBuffers[0], FormatLinkMonName(0));
    StringCopy(gStringBuffers[1], FormatLinkMonName(1));
    if (gMonLinkData.monCount == 4)
    {
        StringCopy(gStringBuffers[2], FormatLinkMonName(2));
        StringCopy(gStringBuffers[3], FormatLinkMonName(3));
    }
    else
    {
        StringCopy(gStringBuffers[2], text);
        StringCopy(gStringBuffers[3], text);
    }
    if (GAME_LANGUAGE == LANGUAGE_JAPANESE)
        PrintJapaneseMenuOptions(gBattleMenuState.messageWindow, gTargetMenuOptions, 4, 2);
    else
    PrintMenuOptions(gBattleMenuState.messageWindow, gTargetMenuOptions, 4);
    gBgTilemapBufferTransferScheduled[3] = TRUE;
    gBgTilemapBufferTransferScheduled[2] = TRUE;
    return gMonLinkData.monCount;
}

s32 ConfirmGiveIn(void)
{
    s32 choice;

    CopyRectWithinBgTilemapBuffer(3, 0, 20, 30, 6, 0, 14);
    FillBgTilemapBufferRect(2, 0, 14, 30, 6, 0);
    ClearWindowCharBuffer(gBattleMenuState.messageWindow, 0xFFFF);
    RenderTextAt(gBattleMenuState.messageWindow, 0, 0, gText_WillYouGiveIn);
    if (GAME_LANGUAGE == LANGUAGE_JAPANESE)
        PrintJapaneseMenuOptions(gBattleMenuState.messageWindow, gGiveInMenuOptions, 2, 3);
    else
    PrintMenuOptions(gBattleMenuState.messageWindow, gGiveInMenuOptions, 2);
    gBgTilemapBufferTransferScheduled[3] = TRUE;
    gBgTilemapBufferTransferScheduled[2] = TRUE;
    choice = HandleMenuInput(gGiveInMenuOptions, 2);
    if (choice != 1)
        ClearWindowCharBuffer(gBattleMenuState.messageWindow, 0);
    return choice;
}

static inline void RestoreBattleMenuInline(void)
{
    CopyRectWithinBgTilemapBuffer(2, 0, 32, 30, 6, 0, 14);
    FillWindowCharBufferRect(gBattleMenuState.messageWindow, 18, 0, 10, 4, 0);
    RenderTextAt(gBattleMenuState.messageWindow, 0, 0, gText_WhatWillMonDo);
    if (GAME_LANGUAGE == LANGUAGE_JAPANESE)
        PrintJapaneseMenuOptions(gBattleMenuState.messageWindow, gText_BattleOptions, 4, 0);
    else
    PrintMenuOptions(gBattleMenuState.messageWindow, gText_BattleOptions, 4);
    gBgTilemapBufferTransferScheduled[2] = TRUE;
}

static inline void BufferSelectedMoveNameInline(u32 unused, s32 moveSlot)
{
    u8 text[GAME_LANGUAGE == LANGUAGE_JAPANESE ? 16 : 28];
    u32 move = gMonLinkData.moves[moveSlot];
    if (move != MOVE_NONE)
        CopyMoveName(text, move);
    else
        StringCopy(text, gText_1Dash);
    BufferString(0, text);
}

void PrintBattleMenuMessage(s32 monId, s32 moveSlot, u32 stringId, bool32 keepMessage)
{
    u8 text[24];

    FillBgTilemapBufferRect(2, 0, 14, 30, 6, 0);
    CopyRectWithinBgTilemapBuffer(3, 0, 20, 30, 6, 0, 14);
    ClearWindowCharBuffer(gBattleMenuState.messageWindow, 0);
    MoveSpriteToHead(gBattleMenuState.cursorSprite);
    gBattleMenuState.cursorSprite = NULL;

    if (moveSlot != 0)
        BufferSelectedMoveNameInline(0, moveSlot - 1);

    GetMonData(&gPlayerPartyPtr[monId], MON_DATA_NICKNAME, text);
    BufferString(4, text);
    gBgTilemapBufferTransferScheduled[2] = TRUE;
    gBgTilemapBufferTransferScheduled[3] = TRUE;
    RenderText(gBattleMenuState.messageWindow, gBattleStringsTable[stringId]);
    if (!keepMessage)
    {
        ClearWindowCharBuffer(gBattleMenuState.messageWindow, 0);
        if (GAME_LANGUAGE == LANGUAGE_JAPANESE)
            RestoreBattleMenu();
        else
            RestoreBattleMenuInline();
    }
}

static inline bool32 ValidateSelectedMoveInline(s32 monId, s32 moveSlot)
{
    switch (GetMoveRestriction(moveSlot - 1))
    {
    case 0:
        return TRUE;
    case 1:
        PrintBattleMenuMessage(monId, moveSlot, 4, FALSE);
        break;
    case 2:
        PrintBattleMenuMessage(monId, moveSlot, 5, FALSE);
        break;
    case 3:
        PrintBattleMenuMessage(monId, moveSlot, 6, FALSE);
        break;
    case 4:
        PrintBattleMenuMessage(monId, moveSlot, 7, FALSE);
        break;
    case 5:
        PrintBattleMenuMessage(monId, GetForcedMoveSlot(), 8, FALSE);
        break;
    case 6:
        PrintBattleMenuMessage(monId, moveSlot, 9, FALSE);
        break;
    }
    return FALSE;
}

static inline void RestoreTargetMenuInline(void)
{
    CopyRectWithinBgTilemapBuffer(3, 0, 20, 30, 6, 0, 14);
    ClearWindowCharBuffer(gBattleMenuState.messageWindow, 0);
    gBgTilemapBufferTransferScheduled[3] = TRUE;
}

bool32 ShowBattleMenu(u32 monId)
{
    u32 responseWord;
    bool32 loop;
    u32 giveInChoice;
    struct BattleMenuInputState *inputState = &gBattleMenuInputState;

    gBattleMenuMonId = monId;
    InitBattleMenu();
    DrawBattleMonInfo(monId);
    if (GAME_LANGUAGE == LANGUAGE_JAPANESE)
            RestoreBattleMenu();
        else
            RestoreBattleMenuInline();
    FadeIn();
    loop = TRUE;
    while (loop)
    {
        switch (inputState->state)
        {
        case 0:
            if (GAME_LANGUAGE == LANGUAGE_JAPANESE)
            RestoreBattleMenu();
        else
            RestoreBattleMenuInline();
            inputState->state = HandleMenuInput(gText_BattleOptions, 4);
            if (inputState->state == 0 && gMonLinkData.selectedMonIdPlusOne == 2)
            {
                gMonLinkData.battleAction = 0;
                loop = FALSE;
            }
            break;
        case 1:
            if (GetBattleMenuRestriction() == 1)
            {
                PrintBattleMenuMessage(gBattleMenuMonId, 0, 14, TRUE);
                gMonLinkData.battleTarget = 0;
                gMonLinkData.battleAction = 1;
                inputState->state = 0;
                loop = FALSE;
            }
            else if (GetBattleMenuRestriction() == 2)
            {
                gMonLinkData.battleTarget = 0;
                gMonLinkData.battleAction = 1;
                inputState->state = 0;
                loop = FALSE;
            }
            else
            {
                u32 numMoves = DrawMoveSelectionMenu(monId);
                inputState->moveSlot = HandleMenuInput(gMoveMenuOptions, numMoves);
                if (inputState->moveSlot == 0)
                {
                    inputState->state = 0;
                    RestoreTargetMenuInline();
                }
                else if (IsMoveSelectionLocked(inputState->moveSlot - 1) == 0)
                {
                    if (ValidateSelectedMoveInline(gBattleMenuMonId, inputState->moveSlot))
                    {
                        gMonLinkData.battleTarget = giveInChoice,
                        gMonLinkData.battleAction = 1,
                        loop = FALSE;
                    }
                    inputState->state = 0;
                }
                else
                {
                    inputState->state = 4;
                }
            }
            break;
        case 2:
            giveInChoice = ConfirmGiveIn();
            inputState->state = 0;
            if (giveInChoice == 1)
            {
                gMonLinkData.battleAction = 3;
                loop = FALSE;
            }
            break;
        case 3:
            FadeOut();
            ResetPartyMenuCursor();
            if (gMonLinkData.selectedMonIdPlusOne == 0)
                gMonLinkData.partyMenuLayout = 1;
            else
                gMonLinkData.partyMenuLayout = 2;
            ShowPartyMenu(0, monId, 0);
            inputState->state = 0;
            if (gMonLinkData.battleAction != 4)
            {
                loop = FALSE;
            }
            else
            {
                InitBattleMenu();
                DrawBattleMonInfo(monId);
                if (GAME_LANGUAGE == LANGUAGE_JAPANESE)
            RestoreBattleMenu();
        else
            RestoreBattleMenuInline();
                FadeIn();
            }
            break;
        case 4:
            DrawTargetSelectionMenu();
            giveInChoice = HandleMenuInput(gTargetMenuOptions, 4);
            if (giveInChoice == 0)
            {
                inputState->state = 1;
            }
            else
            {
                if (ValidateSelectedMoveInline(gBattleMenuMonId, inputState->moveSlot))
                {
                    gMonLinkData.battleTarget = giveInChoice,
                    gMonLinkData.battleAction = 1,
                    loop = FALSE;
                }
                inputState->state = 0;
            }
            break;
        }

        DelayFrames(1);
    }

    responseWord = 0;
    switch (gMonLinkData.battleAction)
    {
    case 1:
        responseWord = inputState->moveSlot - 1;
        responseWord |= (((gMonLinkData.battleTarget - 1) << 8) & 0xFF00);
        break;
    case 2:
        responseWord = gMonLinkData.partyActionMonId;
        break;
    }

    responseWord |= (gMonLinkData.battleAction) << 16;
    gMonLinkData.response = responseWord;
    gMonLinkData.responsePending = 1;

    if (!IsScreenFadedOut())
        FadeOut();

    return FALSE;
}

s32 UNUSED ShowBattleMonMessage(s32 monId, u32 stringId)
{
    u8 text[24];

    gBattleMenuMonId = monId;
    InitBattleMenu();
    DrawBattleMonInfo(monId);
    CopyRectWithinBgTilemapBuffer(2, 0, 38, 30, 6, 0, 14);
    FillBgTilemapBufferRect(3, 0, 14, 30, 6, 0);
    ClearWindowCharBuffer(gBattleMenuState.messageWindow, 0);
    GetMonData(&gPlayerPartyPtr[monId], MON_DATA_NICKNAME, text);
    BufferString(4, text);
    FadeIn();
    RenderText(gBattleMenuState.messageWindow, gBattleStringsTable[stringId]);
    FadeOut();

    return; // Garbage return
}

void RestoreBattleMenu(void)
{
    RestoreBattleMenuInline();
}

void UNUSED RestoreTargetMenu(void)
{
    RestoreTargetMenuInline();
}

void UNUSED BufferSelectedMoveName(u32 unused, s32 moveSlot)
{
    BufferSelectedMoveNameInline(unused, moveSlot);
}

bool32 UNUSED ValidateSelectedMove(s32 monId, s32 moveSlot)
{
    return ValidateSelectedMoveInline(monId, moveSlot);
}

u32 GetMoveRestriction(s32 moveSlot)
{
    return GetNibbleByIndex(gMonLinkData.moveRestrictions, moveSlot) & 7;
}

u32 IsMoveSelectionLocked(s32 moveSlot)
{
    return (GetNibbleByIndex(gMonLinkData.moveRestrictions, moveSlot) >> 3) & 1;
}

#ifdef PROGRAM_COUPON
u32 GetBattleMenuRestriction(void)
{
    return gMonLinkData.battleRestriction;
}

// coupon.bin's version searches moveRestrictions instead. It returns the
// first move whose nibble is not 5, counted from 1, or 0 if there is none.
u32 GetForcedMoveSlot(void)
{
    s32 i;

    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        if (GetNibbleByIndex(gMonLinkData.moveRestrictions, i) != 5)
            return i + 1;
    }
    return 0;
}
#else
u32 GetBattleMenuRestriction(void)
{
    return gMonLinkData.battleMenuRestriction;
}

u32 GetForcedMoveSlot(void)
{
    return gMonLinkData.forcedMoveIndex + 1;
}
#endif

bool32 IsMonFainted(u32 monId)
{
    bool32 ret;
    if (GetMonAilment(&gPlayerPartyPtr[monId]) == STATUS_PRIMARY_FAINTED)
        ret = TRUE;
    else
        ret = FALSE;
    return ret;
}
