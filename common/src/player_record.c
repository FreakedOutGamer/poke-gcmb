#include "global.h"
#include "player_record.h"
#include "libpmagb/save_block_pointers.h"
#include "libpmagb/save.h"
#include "gf_rom_header.h"
#include "string_util.h"
#include "target_game.h"

// Builds the player, party and gift-ribbon record the GameCube asks for.

struct PlayerLinkInfo *SetPlayerLinkInfo(u8 *sav2, u8 *sav1, u32 saveStatus)
{
    s32 i;
    struct PlayerLinkInfo* structPtr = &gPlayerLinkInfo;

    CpuFill16(0, &gPlayerLinkInfo, sizeof(struct PlayerLinkInfo));

    if (saveStatus == SAVE_STATUS_OK)
    {
        u8 *saveData;

        if (!gAgbPmRomParams->blockLinkColoXD)
        {
            structPtr->hasPokedex = CheckIfPokedexIsObtained();
#ifdef PROGRAM_COLBTL
            structPtr->isInPokeCenter = ((GetPlayerMapType() & POKECENTER_SAVEWARP) != 0);
            structPtr->isFRLG = IsFRLG();
            structPtr->isChampion = ((GetPlayerMapType() & CHAMPION_SAVEWARP) != 0);
#else
            // Shifted down and masked with saveStatus, which is 1 here.
            structPtr->isInPokeCenter = (GetPlayerMapType() >> 1) & saveStatus;
            structPtr->isFRLG = IsFRLG();
            structPtr->isChampion = GetPlayerMapType() >> 7;
#endif
#if GAME_LANGUAGE != LANGUAGE_JAPANESE
            structPtr->language = gAgbPmRomParams->language;
#endif
        }

        saveData = &sav2[gAgbPmRomParams->playerNameOffset];
        StringCopy(structPtr->playerName, saveData);

        saveData = &sav2[gAgbPmRomParams->playerGenderOffset];
        structPtr->playerGender = *saveData;

        saveData = &sav2[gAgbPmRomParams->trainerIdOffset];

        for (i = 0; i < TRAINER_ID_LENGTH; i++)
            structPtr->playerTrainerId[i] = saveData[i];

        for (i = 0; i < PARTY_SIZE; i++)
            CpuCopy16(&gPlayerPartyPtr[i], &structPtr->party[i], sizeof(struct Pokemon));

        for (i = 0; i < GIFT_RIBBONS_COUNT; i++)
            structPtr->giftRibbons[i] = gGiftRibbonsPtr[i];
    }
#if defined(PROGRAM_COLBTL) && GAME_LANGUAGE != LANGUAGE_JAPANESE
    else if (saveStatus == SAVE_STATUS_CORRUPT)
    {
        structPtr->errorCode = 1;
    }
    else if (saveStatus == SAVE_STATUS_ERROR)
    {
        structPtr->errorCode = 2;
    }
    else if (saveStatus == SAVE_STATUS_EMPTY)
    {
        structPtr->errorCode = 3;
    }
#endif

    return structPtr;
}

struct PlayerLinkInfo *GetPlayerLinkInfo(void)
{
    return &gPlayerLinkInfo;
}
u8 *GetTextBufferPointer(u32 id)
{
    return gTextBuffer[id];
}

// colbtl keeps these in src/joybus_recv.c.
#ifndef PROGRAM_COLBTL
void SetLinkCommandReady(u8 ready)
{
    gLinkCommandReady = ready;
}

bool32 IsLinkCommandReady(void)
{
    return gLinkCommandReady;
}
#endif
