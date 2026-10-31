#include "global.h"
#include "constants/flags.h"
#include "libpmagb/save_block_pointers.h"
#include "libpmagb/save.h"
#include "gflib/characters.h"
#include "gflib/keys.h"
#include "gflib/init.h"
#include "pokeball.h"
#include "colbtl.h"
#include "pokedex.h"
#include "data.h"
#include "joybus.h"
#include "flag.h"
#include "target_game.h"
#include "string_util.h"
#include "constants/items.h"
#include "constants/maps.h"

extern bool8 gTargetIsRubySapphire;
// Misnamed. In the regional builds this flag denotes the payload language.
extern bool8 gRomDetection_IsEnglishROM;


extern struct GFRomHeader gAgbPmRomParams_AXV_Regional;
extern struct GFRomHeader gAgbPmRomParams_AXP_Regional;
extern struct GFRomHeader gUnusedRegionalRomHeader;

#if COLBTL_VERSION == VERSION_COLBTL_JP

void DetectROM(void)
{
    u32 rsVersion = 0;
    u32 gameCode = *(u32 *)0x080000AC;

    switch (gameCode)
    {
    case 0x4A565841:
        rsVersion = VERSION_RUBY;
        break;
    case 0x4A505841:
        rsVersion = VERSION_SAPPHIRE;
        break;
    default:
        gAgbPmRomParams = (void *)0x08000100;
        gTargetIsRubySapphire = FALSE;
        return;
    }
    if (rsVersion == VERSION_RUBY)
        gAgbPmRomParams = &gAgbPmRomParams_AXV_Regional;
    else
        gAgbPmRomParams = &gAgbPmRomParams_AXP_Regional;
    gTargetIsRubySapphire = TRUE;
}
#elif COLBTL_VERSION != VERSION_COLBTL && COLBTL_VERSION != VERSION_COLBTL_E
#if COLBTL_VERSION == VERSION_COLBTL_F
#define LOCAL_GAME_CODE 0x46000000
#elif COLBTL_VERSION == VERSION_COLBTL_G
#define LOCAL_GAME_CODE 0x44000000
#elif COLBTL_VERSION == VERSION_COLBTL_I
#define LOCAL_GAME_CODE 0x49000000
#else
#define LOCAL_GAME_CODE 0x53000000
#endif
void DetectROM(void)
{
    u32 gameCode, rsVersion, useRomHeader;

    gTargetIsRubySapphire = FALSE;
    gRomDetection_IsEnglishROM = FALSE;
    rsVersion = 0;
    gameCode = *(u32 *)0x080000AC;
    switch (gameCode)
    {
    default:
        useRomHeader = TRUE;
        gRomDetection_IsEnglishROM = TRUE;
        break;
    case LOCAL_GAME_CODE | 0x00565841:
        gRomDetection_IsEnglishROM = TRUE;
        rsVersion = VERSION_RUBY;
        useRomHeader = FALSE;
        gTargetIsRubySapphire = TRUE;
        break;
    case LOCAL_GAME_CODE | 0x00505841:
        gRomDetection_IsEnglishROM = TRUE;
        rsVersion = VERSION_SAPPHIRE;
        useRomHeader = FALSE;
        gTargetIsRubySapphire = TRUE;
        break;
    case 0x45565841:
    case 0x45505841:
#if COLBTL_VERSION != VERSION_COLBTL_F
    case 0x46565841:
    case 0x46505841:
#endif
#if COLBTL_VERSION != VERSION_COLBTL_G
    case 0x44565841:
    case 0x44505841:
#endif
#if COLBTL_VERSION != VERSION_COLBTL_I
    case 0x49565841:
    case 0x49505841:
#endif
#if COLBTL_VERSION != VERSION_COLBTL_S
    case 0x53565841:
    case 0x53505841:
#endif
        useRomHeader = TRUE;
        gTargetIsRubySapphire = TRUE;
        break;
    }
    if (useRomHeader)
    {
        if (*(u32 *)0x080000AC == 0x45565841 || *(u32 *)0x080000AC == 0x45505841)
            gAgbPmRomParams = &gUnusedRegionalRomHeader;
        else
            gAgbPmRomParams = (void *)0x08000100;
    }
    else if (rsVersion == VERSION_RUBY)
        gAgbPmRomParams = &gAgbPmRomParams_AXV_Regional;
    else
        gAgbPmRomParams = &gAgbPmRomParams_AXP_Regional;
    if (useRomHeader == TRUE)
    {
        if (gAgbPmRomParams->language == GAME_LANGUAGE)
            gRomDetection_IsEnglishROM = TRUE;
        else
            gRomDetection_IsEnglishROM = FALSE;
    }
}
#else
void DetectROM(void)
{
    u32 gameCode, rsVersion, notEnglishRS;

    gTargetIsRubySapphire = FALSE;
    gRomDetection_IsEnglishROM = FALSE;
    rsVersion = 0;
    gameCode = *(u32 *)(0x80000AC);

    switch (gameCode)
    {
    default:
        notEnglishRS = TRUE;
#if COLBTL_VERSION != VERSION_COLBTL
        gRomDetection_IsEnglishROM = TRUE;
#endif
        break;
    case 0x45565841: // AXVE
        gRomDetection_IsEnglishROM = TRUE;
        rsVersion = VERSION_RUBY;
        notEnglishRS = FALSE;
        gTargetIsRubySapphire = TRUE;
        break;
    case 0x45505841: // AXPE
        gRomDetection_IsEnglishROM = TRUE;
        rsVersion = VERSION_SAPPHIRE;
        notEnglishRS = FALSE;
        gTargetIsRubySapphire = TRUE;
        break;
    case 0x46505841: // AXPF
    case 0x44565841: // AXVD
    case 0x44505841: // AXPD
    case 0x49565841: // AXVI
    case 0x46565841: // AXVF
    case 0x49505841: // AXPI
    case 0x53505841: // AXPS
    case 0x53565841: // AXVS
        notEnglishRS = TRUE;
        gTargetIsRubySapphire = TRUE;
        break;
    }

    if (notEnglishRS)
    {
        gAgbPmRomParams = (void*)(0x8000100);
    }
    else if (rsVersion == VERSION_RUBY)
    {
        if (*(u8*)(0x80000BC) == 0)
            gAgbPmRomParams = &gAgbPmRomParams_AXVE_rev0;
        else
            gAgbPmRomParams = &gAgbPmRomParams_AXVE_rev2;
    }
    else
    {
        if (*(u8*)(0x80000BC) == 0)
            gAgbPmRomParams = &gAgbPmRomParams_AXPE_rev0;
        else
            gAgbPmRomParams = &gAgbPmRomParams_AXPE_rev2;
    }

#if COLBTL_VERSION != VERSION_COLBTL
    if (notEnglishRS == TRUE)
    {
        if (gAgbPmRomParams->language == LANGUAGE_ENGLISH)
            gRomDetection_IsEnglishROM = TRUE;
        else
            gRomDetection_IsEnglishROM = FALSE;
    }
#endif
}
#endif

u8 GetPlayerMapType(void)
{
    u8 retVal;

    if (!gTargetIsRubySapphire)
    {
        retVal = *((u8 *)(gSaveBlock2Ptr) + gAgbPmRomParams->warpFlagsOffset);
    }
    else
    {
        retVal = GetRSPlayerMapType(gSaveBlock1Ptr);
        if (retVal & CONTINUE_GAME_WARP)
            retVal |= CHAMPION_SAVEWARP;
    }

    return retVal;
}

static void UNUSED EmptyFunction(void)
{

}

u8 CheckIfPokedexIsObtained(void)
{
    bool32 val;

    if (!gTargetIsRubySapphire)
        val = ((*((u8 *)(gSaveBlock2Ptr) + gAgbPmRomParams->gcnLinkFlagsOffset) & 1));
    else
        val = FlagGet(FLAG_SYS_POKEDEX_GET);

    if (!val)
        return FALSE;
    else
        return TRUE;
}

bool32 IsFRLG(void)
{
    if (gAgbPmRomParams->version == VERSION_FIRE_RED || gAgbPmRomParams->version == VERSION_LEAF_GREEN)
        return TRUE;
    else
        return FALSE;
}

bool32 CheckGameClear(void)
{
    u8 *flagsPtr = gSaveBlock1Ptr + gAgbPmRomParams->flagsOffset + (gAgbPmRomParams->gameClearFlag / 8);

    return (*flagsPtr & (1 << (gAgbPmRomParams->gameClearFlag % 8))) != 0;
}
