#include "global.h"
#include "libpmagb/save_block_pointers.h"
#include "gflib.h"
#include "colbtl.h"
#include "strings.h"
#include "graphics.h"
#include "joybus.h"
#include "libpmagb/save.h"
#include "fade.h"
#include "link_message.h"
#include "link_transfer.h"
#include "render_text.h"
#include "window.h"

// Error related
extern u8 gSaveLoadResult;
extern u8 gRomDetection_IsEnglishROM;
extern u8 gBerryFixResult;
extern const struct Window gFont0LatinInfo;
extern const struct Window gWindowTemplateLinkMessage;
extern struct Window *gMessageWindowPtr;

extern const u8 *gErrorMessagePtrs[];

void InitLinkMessageScreen(void)
{
    ClearVram();
    REG_DISPCNT = DISPCNT_BG0_ON | DISPCNT_BG2_ON | DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP;
    REG_BG0CNT = BGCNT_PRIORITY(1) | BGCNT_CHARBASE(0) | BGCNT_SCREENBASE(31);
    REG_BG2CNT = BGCNT_PRIORITY(0) | BGCNT_CHARBASE(2) | BGCNT_SCREENBASE(30);
    REG_BG0CNT |= BGCNT_256COLOR;
    AutoUnCompVram(gTextPal, (void *) PLTT);
    AutoUnCompVram(gTitleExtraPal, (void *) PLTT + 0x1C0);
    AutoUnCompVram(gMessageBox_Gfx, (void *) VRAM + 0x8000);
    AutoUnCompVram(gTitle_Pal, (void *) PLTT);
    AutoUnCompVram(gTitle_Gfx, (void *) VRAM);
    AutoUnCompVram(gTitle_Tilemap, gBGTilemapBuffers);
    DrawTextWindowBorder(0, 16, 30, 4, 0xE001);
    gMessageWindowPtr = AddWindow(0, &gWindowTemplateLinkMessage);
    SetTextColor(gMessageWindowPtr, 1, 8);
    gBgTilemapBufferTransferScheduled[0] = TRUE;
    gBgTilemapBufferTransferScheduled[2] = TRUE;
}

static inline void PrintErrorMsg(struct Window *win, u32 msgId)
{
    ClearWindowCharBuffer(win, 0xFFFF);
    RenderText(win, gErrorMessagePtrs[msgId]);
}

static inline void PrintSaveStatusMessageInline(void)
{
    switch (gSaveLoadResult)
    {
    case SAVE_STATUS_CORRUPT:
        PrintErrorMsg(gMessageWindowPtr, 6);
        break;
    case SAVE_STATUS_ERROR:
        PrintErrorMsg(gMessageWindowPtr, 7);
        break;
    case SAVE_STATUS_EMPTY:
        PrintErrorMsg(gMessageWindowPtr, 8);
        break;
    default:
        PrintErrorMsg(gMessageWindowPtr, 1);
        break;
    }
}

void PrintSaveStatusMessage(void)
{
    PrintSaveStatusMessageInline();
}

// ErrorPrint's body. RunLinkStandbyLoop has it inlined in the original;
// ErrorPrint keeps its own copy, as calling this from it compiles differently.
static inline void ErrorPrintInline(u32 msgId)
{
    struct Window *win;
    struct Window winTemplate;

    winTemplate = gFont0LatinInfo;
    FillBgTilemapBufferRect(2, 0, 16, 30, 4, 0);
    DrawTextWindowBorder(3, 14, 24, 6, 0xE001);
    win = AddWindow(0, &winTemplate);
    SetTextColor(win, 1, 8);
    ClearWindowCharBuffer(win, 0xFFFF);
    gBgTilemapBufferTransferScheduled[2] = TRUE;
    if (IsScreenFadedOut() == (GAME_LANGUAGE == LANGUAGE_JAPANESE ? FALSE : TRUE))
        FadeIn();
    RenderText(win, gErrorMessagePtrs[msgId]);
    // Japanese failures return to the cartridge; later revisions stop here.
#if GAME_LANGUAGE == LANGUAGE_JAPANESE
    SoftReset(0);
#else
    while (1)
    {
        DelayFrames(GAME_LANGUAGE == LANGUAGE_JAPANESE ? 0 : 100);
    }
#endif
}

#ifdef PROGRAM_COUPON
// The message last shown for the GameCube's command, or 0.
extern u8 gLinkCommandMessageId;

// Shows which command the GameCube is running, once each time it changes.
// coupon.bin carries it but never calls it.
void UNUSED PrintLinkCommandMessage(void)
{
    u32 msgId;

    switch (gJoyBusState.command)
    {
    case LINK_CMD_TRAN_PLAYER_DATA1:
        msgId = 12;
        break;
    case LINK_CMD_RECV_TEXT:
        msgId = 11;
        break;
    case LINK_CMD_RECV_UNKNOWN:
        msgId = 13;
        break;
    case LINK_CMD_RECV_MON_DATA:
        msgId = 14;
        break;
    case LINK_CMD_RECV_PARTY_MON:
        msgId = 15;
        break;
    case LINK_CMD_0x44:
        msgId = 16;
        break;
    case LINK_CMD_TRAN_GIFT_DATA:
        msgId = 17;
        break;
    case LINK_CMD_RECV_GIFT_DATA:
        msgId = 18;
        break;
    default:
        msgId = 0;
        break;
    }

    if (gLinkCommandMessageId != msgId)
    {
        gLinkCommandMessageId = msgId;
        if (msgId != 0)
            PrintErrorMsg(gMessageWindowPtr, gLinkCommandMessageId);
    }
}
#endif

u32 RunLinkStandbyLoop(void)
{
    s32 result;

#ifdef PROGRAM_COUPON
    gLinkCommandMessageId = 0;
#endif
    gMonLinkData.battleAction = 4;
    InitLinkMessageScreen();
    if (gAgbPmRomParams->blockLinkColoXD == TRUE
#if GAME_LANGUAGE != LANGUAGE_JAPANESE
        || !gRomDetection_IsEnglishROM
#endif
    )
    {
        // Unused, but the original's stack frame has room for it.
        u8 UNUSED buff[8];
#if GAME_LANGUAGE != LANGUAGE_JAPANESE
        OverrideScreenFadeState(TRUE);
#endif
        ErrorPrintInline(9);
    }

#if GAME_LANGUAGE == LANGUAGE_JAPANESE
    PrintSaveStatusMessageInline();
#else
    if (gBerryFixResult != 1)
        PrintSaveStatusMessage();
    else
        ClearWindowCharBuffer(gMessageWindowPtr, 0xFFFF);
#endif

    gMonLinkData.numMonsToSelect = 0;
    gMonLinkData.transferComplete = 0;
    gMonLinkData.linkSessionComplete = 0;
    gMonLinkData.partyMonIncoming = 0;
    FadeIn();
    if (gSaveLoadResult == SAVE_STATUS_CORRUPT || gSaveLoadResult == SAVE_STATUS_ERROR || gSaveLoadResult == SAVE_STATUS_EMPTY)
    {
#if GAME_LANGUAGE == LANGUAGE_JAPANESE
        do
        {
            DelayFrames(1);
        } while (gNewKeys == 0);
        FadeOut();
        SoftReset(0);
#else
        // Game stuck
        while (1)
        {
            DelayFrames(GAME_LANGUAGE == LANGUAGE_JAPANESE ? 0 : 0xFF);
        }
#endif
    }

#if GAME_LANGUAGE != LANGUAGE_JAPANESE
    if (gBerryFixResult == 1)
    {
        RenderText(gMessageWindowPtr, gText_BerryProgramUpdated);
        PrintSaveStatusMessage();
    }
#endif
    if (gMonLinkData.responsePending != 0)
    {
        REG_JOY_TRANS = gMonLinkData.response;
        gMonLinkData.responsePending = 0;
    }
    result = 0;
    while (result == 0)
    {
        DelayFrames(1);
        if (gMonLinkData.numMonsToSelect != 0 && IsLinkCommandReady() == 1)
            result = 2;
        if (gMonLinkData.partyMonIncoming == 1)
        {
            gMonLinkData.partyMonIncoming = 0;
            FinalizeCouponTransfer();
        }
        if (gMonLinkData.linkSessionComplete == 1)
            result = 1;
        if (gMonLinkData.pokemonTransferPending == 1)
        {
            gMonLinkData.pokemonTransferPending = 0;
            FinalizePokemonTransfer();
            PrintErrorMsg(gMessageWindowPtr, 1);
        }
    }
    FadeOut();
    return result - 1;
}

void WarningPrint(u32 msgId)
{
    struct Window *win = gMessageWindowPtr;

    ClearWindowCharBuffer(win, 0xFFFF);
    RenderText(win, gErrorMessagePtrs[msgId]);
}

void ErrorPrint(u32 msgId)
{
    struct Window *win;
    struct Window winTemplate;

    winTemplate = gFont0LatinInfo;
    FillBgTilemapBufferRect(2, 0, 16, 30, 4, 0);
    DrawTextWindowBorder(3, 14, 24, 6, 0xE001);
    win = AddWindow(0, &winTemplate);
    SetTextColor(win, 1, 8);
    ClearWindowCharBuffer(win, 0xFFFF);
    gBgTilemapBufferTransferScheduled[2] = TRUE;
    if (IsScreenFadedOut() == (GAME_LANGUAGE == LANGUAGE_JAPANESE ? FALSE : TRUE))
        FadeIn();
    RenderText(win, gErrorMessagePtrs[msgId]);
    // Japanese failures return to the cartridge; later revisions stop here.
#if GAME_LANGUAGE == LANGUAGE_JAPANESE
    SoftReset(0);
#else
    while (1)
    {
        DelayFrames(GAME_LANGUAGE == LANGUAGE_JAPANESE ? 0 : 100);
    }
#endif
}
