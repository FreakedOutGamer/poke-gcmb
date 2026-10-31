#include "global.h"
#include "gflib.h"
#include "colbtl.h"
#include "libpmagb/save_block_pointers.h"
#include "libpmagb/save.h"
#include "berry_fix.h"
#include "joybus.h"
#include "flag.h"
#include "target_game.h"
#include "pokedex.h"
#include "string_util.h"
#include "gflib/keys.h"
#include "gflib/bg.h"
#include "gflib/text.h"
#include "gflib/characters.h"
#include "gflib/decompress.h"
#include "constants/pokemon.h"
#include "constants/items.h"
#include "constants/songs.h"
#include "pokemon.h"
#include "strings.h"
#include "graphics.h"
#include "link_transfer.h"
#include "party_menu.h"
#include "battle_menu.h"
#include "fade.h"
#include "link_message.h"

struct RomHeader
{
    u8 nintendoLogo[156];
    u8 gameTitle[12];
    u8 gameCode[8];
    u8 DeviceType;
    u8 zero[7];
    u8 revision;
};

extern const struct RomHeader gRomHeader;
extern u8 gSaveLoadResult;
extern u8 gBerryFixResult;

extern IntrFunc *gSaveTimerIntrSlotPtr;
extern struct PartyMenuState gPartyMenuState;

bool8 RunLinkPartyMenu(void)
{
    u32 responseWord;

    GetLinkSelectedMonId();
    if (gMonLinkData.selectedMonIdPlusOne == 0)
    {
        gMonLinkData.partyMenuLayout = 1;
    }
    else
    {
        gMonLinkData.partyMenuLayout = 2;
    }
    ShowPartyMenu(0, GetLinkSelectedMonId(), 1);
    responseWord = gMonLinkData.partyActionMonId | (gMonLinkData.battleAction << 16);
    gMonLinkData.response = responseWord;
    gMonLinkData.responsePending = 1;
    return FALSE;
}

inline u32 UNUSED RunBerryFix(void)
{
    u8 buffer[64];
    u32 state = 0;
    u32 result = 0;
    u32 language = gAgbPmRomParams->language;

    // The original release runs the update without the later language gate.
    if (COLBTL_VERSION != VERSION_COLBTL && language != GAME_LANGUAGE)
        return 0;

    if (COLBTL_VERSION == VERSION_COLBTL || language == GAME_LANGUAGE)
    {
#if COLBTL_VERSION == VERSION_COLBTL
        // Stop on no update needed, invalid year, or error.
        while (state != 6 && state != 7 && state != 11)
#else
        while (1)
#endif
        {
            BerryFix(&state, buffer, 0);
            if (state == 5)
                result = 1;
            if (state == 11)
                result = 2;
#if COLBTL_VERSION != VERSION_COLBTL
            if (state == 6 || state == 7 || state == 11)
                break;
#endif
        }
    }

    return result;
}

// Later releases run the Berry Fix only on a cartridge of this release's
// language, and hand InitLink the game code from the payload's own header.
#if COLBTL_VERSION != VERSION_COLBTL && COLBTL_VERSION != VERSION_COLBTL_JP
static inline u32 RunBerryFixForLink(const u8 **gameCodeOut)
{
    u8 buffer[64];
    u32 state = 0;
    u32 result = 0;
    u32 ret;

    if (gAgbPmRomParams->language != GAME_LANGUAGE)
    {
        ret = 0;
        *gameCodeOut = gRomHeader.gameCode;
    }
    else
    {
        *gameCodeOut = gRomHeader.gameCode;
        while (state != 6 && state != 7 && state != 11)
        {
            BerryFix(&state, buffer, 0);
            if (state == 5)
                result = 1;
            if (state == 11)
                result = 2;
        }
        ret = result;
    }
    return ret;
}
#endif

void GF_Main(void)
{
    s32 i;
    u16 monStatus;
#if COLBTL_VERSION != VERSION_COLBTL && COLBTL_VERSION != VERSION_COLBTL_JP
    const u8 *gameCode;
#endif

    DetectROM();
    InitFlash(2, gSaveTimerIntrSlotPtr);
    SaveBlocksInit();
    SetSaveSectorPtrs();
    gSaveLoadResult = ReadSaveBlockChunks();

#if COLBTL_VERSION == VERSION_COLBTL
    gBerryFixResult = RunBerryFix();
#elif COLBTL_VERSION != VERSION_COLBTL_JP
    gBerryFixResult = RunBerryFixForLink(&gameCode);
#endif

    InitSound();
    ResetPartyMenuState();
    SetKeyRepeatTiming(0x28, 5);
    REG_IE = INTR_FLAG_VBLANK;
    REG_DISPSTAT = 8;
    REG_DISPCNT &= (0xFF7F);
    REG_IME = 1;
    SetPlayerLinkInfo(gSaveBlock2Ptr, gSaveBlock1Ptr, gSaveLoadResult);

#if COLBTL_VERSION == VERSION_COLBTL || COLBTL_VERSION == VERSION_COLBTL_JP
    InitLink(gRomHeader.gameCode);
#else
    InitLink(gameCode);
#endif

    SetVBlankCallback((IntrFunc)VBlankCB);
    PauseSoundVSync();
#if COLBTL_VERSION == VERSION_COLBTL_JP
    GenerateFontHalfrowLookupTable((u32 *) 0x03000000);
#else
    GenerateFontHalfrowLookupTable((u32 *) 0x03004000);
#endif
    FadeOut();
    for (;;)
    {
        if (RunLinkStandbyLoop() == 0)
        {
            BufferFirstPartyMon();
            ApplyLinkBattlePartyData();
            if (ShouldUseLinkPartyMenu() == 0)
            {
                ShowBattleMenu((gMonLinkData.selectedMonIdPlusOne != 0) ? gMonLinkData.selectedMonIdPlusOne - 1 : 0);
            }
            else
            {
                RunLinkPartyMenu();
            }
        }
        else
        {
            CpuCopy16(gSaveBlock1BackupPtr, gSaveBlock1Ptr, gAgbPmRomParams->saveBlock1Size);
            monStatus = 0;
            for (i = 0; i < PARTY_SIZE; i++)
            {
                SetMonData(&gPlayerPartyPtr[i], MON_DATA_STATUS, &monStatus);
            }
            ShowPartyMenu(1, 0, 0);
        }
    }
}
