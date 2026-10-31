#include "global.h"
#include "pokedex.h"
#include "gflib.h"
#include "colbtl.h"
#include "joybus.h"
#include "target_game.h"
#include "libpmagb/save.h"
#include "link_transfer.h"
#include "link_message.h"

extern u8 gTargetIsRubySapphire;

void ApplyLinkBattlePartyData(void)
{
    u16 value;
    u32 partyCountNibbles;
    s32 i;

    CpuFill16(0, gPlayerPartyPtr, sizeof(struct Pokemon) * PARTY_SIZE);
    partyCountNibbles = gMonLinkData.partyCountNibblefield;
    for (i = 0; i < PARTY_SIZE; i++, partyCountNibbles >>= 4)
    {
        u32 id = partyCountNibbles & 0xF;
        if ((id) != 0xF)
        {
            CpuCopy16(&gPlayerPartyBackupPtr[id], &gPlayerPartyPtr[i], sizeof(struct Pokemon));

            value = gMonLinkData.monData[i].hp;
            SetMonData(&gPlayerPartyPtr[i], MON_DATA_HP, &value);

            value = gMonLinkData.monData[i].status;
            SetMonData(&gPlayerPartyPtr[i], MON_DATA_STATUS, &value);

            value = 0;
            if (gMonLinkData.monData[i].giveMaxPokerusDays)
                value = 0xF;
            if (gMonLinkData.monData[i].giveMaxPokerusStrain)
                value |= 0xF0;
            SetMonData(&gPlayerPartyPtr[i], MON_DATA_POKERUS, &value);

            if (gMonLinkData.monData[i].setMailTo1)
            {
                value = 1;
                SetMonData(&gPlayerPartyPtr[i], MON_DATA_MAIL, &value);
            }

            value = 0;
            if (gMonLinkData.monData[i].preserveHeldItem)
                value = gMonLinkData.monData[i].heldItem;
            SetMonData(&gPlayerPartyPtr[i], MON_DATA_HELD_ITEM, &value);

            value = gMonLinkData.monData[i].moves[0];
            SetMonData(&gPlayerPartyPtr[i], MON_DATA_MOVE1, &value);
            value = gMonLinkData.monData[i].moves[1];
            SetMonData(&gPlayerPartyPtr[i], MON_DATA_MOVE2, &value);
            value = gMonLinkData.monData[i].moves[2];
            SetMonData(&gPlayerPartyPtr[i], MON_DATA_MOVE3, &value);
            value = gMonLinkData.monData[i].moves[3];
            SetMonData(&gPlayerPartyPtr[i], MON_DATA_MOVE4, &value);

            value = gMonLinkData.monData[i].pps[0];
            SetMonData(&gPlayerPartyPtr[i], MON_DATA_PP1, &value);
            value = gMonLinkData.monData[i].pps[1];
            SetMonData(&gPlayerPartyPtr[i], MON_DATA_PP2, &value);
            value = gMonLinkData.monData[i].pps[2];
            SetMonData(&gPlayerPartyPtr[i], MON_DATA_PP3, &value);
            value = gMonLinkData.monData[i].pps[3];
            SetMonData(&gPlayerPartyPtr[i], MON_DATA_PP4, &value);

            CpuCopy16((void*) gMonLinkData.monData[i].textBuffer, GetTextBufferPointer(i), LINK_TEXT_BUFFER_SIZE);
        }
    }

    if (gMonLinkData.selectedMonIdPlusOne != 0)
        gMonLinkData.partyMenuLayout = 1;
    else
        gMonLinkData.partyMenuLayout = 0;
}

bool32 FinalizeLinkSave(void)
{
    bool32 val = WriteLinkSave(0);

    gMonLinkData.cmd44Received = 0;
    if (!val)
    {
        REG_JOY_TRANS = 0;
        while (!gMonLinkData.cmd44Received)
            ;
        val = WriteLinkSave(1);
        while (gMonLinkData.cmd44Active)
            ;

        if (!val)
        {
            REG_JOY_TRANS = 0;
            return FALSE;
        }
    }

    REG_JOY_TRANS = -1;
    return TRUE;
}

static inline u16 GetGcnLinkFlags(const struct GFRomHeader *rom, void *sav2)
{
    u16 *ptr = sav2 + rom->gcnLinkFlagsOffset;
    return *ptr;
}

static inline u8 GetSaveWarpFlags(const struct GFRomHeader *rom, void *sav2)
{
    u8 *ptr = sav2 + rom->warpFlagsOffset;
    return *ptr;
}

s32 FinalizeCouponTransfer(void)
{
    s32 i;
    s32 result;

    WarningPrint(2);
    while (!gMonLinkData.transferComplete)
    {
        if (!gJoyBusState.linked)
            return 2;
    }

    if (gTargetIsRubySapphire)
    {
        if (!(GetPlayerMapType() & POKECENTER_SAVEWARP))
        {
            REG_JOY_TRANS = -1;
            WarningPrint(5);
            return 2;
        }
    }
    else
    {
        u16 metGame, f;
        u16 flags = GetGcnLinkFlags(gAgbPmRomParams, gSaveBlock2Ptr);
        if (!(flags & 0xFFFE))
        {
            REG_JOY_TRANS = -1;
            ErrorPrint(10);
        }
        if (!(GetSaveWarpFlags(gAgbPmRomParams, gSaveBlock2Ptr) & 7))
        {
            REG_JOY_TRANS = -1;
            ErrorPrint(10);
        }

        metGame = GetMonData(&gPlayerPartyPtr[gMonLinkData.monId], MON_DATA_MET_GAME, NULL);
        f = 1 << metGame;
        if (!(flags & f))
        {
            REG_JOY_TRANS = -1;
            ErrorPrint(10);
        }
    }

    WarningPrint(3);
    // Both calls index the party by species rather than by slot, so for an
    // Unown or Spinda, SetSpeciesCaughtFlag reads a personality from well past
    // the party.
    SetSpeciesCaughtFlag(gMonLinkData.species2, &gPlayerPartyPtr[gMonLinkData.species2]);
    if ((gMonLinkData.speciesLowerByte | (gMonLinkData.speciesUpperByte << 8)) != 0)
        SetSpeciesCaughtFlag((gMonLinkData.speciesLowerByte | (gMonLinkData.speciesUpperByte << 8)),
                             &gPlayerPartyPtr[(gMonLinkData.speciesLowerByte | (gMonLinkData.speciesUpperByte << 8))]);

    for (i = 0; i < 11; i++)
    {
        if (gGiftRibbonsPtr[i] == 0 && gMonLinkData.giftRibbons[i] != 0)
        {
            gGiftRibbonsPtr[i] = gMonLinkData.giftRibbons[i];
#ifndef PROGRAM_COUPON
            GiveGiftRibbonToParty(i, gMonLinkData.giftRibbons[i]);
#endif
        }
    }

    result = FinalizeLinkSave();
#ifndef PROGRAM_COUPON
    DelayFrames(GetFrameTotal() & 0x3F);
#endif
    switch (result)
    {
    case 0:
        WarningPrint(1);
        break;
    case 1:
        WarningPrint(4);
        break;
    case 2:
        WarningPrint(5);
        break;
    }

    gMonLinkData.partyMonIncoming = 0;
    gMonLinkData.transferComplete = 0;
    gMonLinkData.cmd44Received = 0;
    return result;
}

s32 FinalizePokemonTransfer(void)
{
    s32 i;
    s32 result;

    WarningPrint(3);
    if (gMonLinkData.species3 != SPECIES_NONE)
    {
        (*gPlayerPartyCountPtr)++;
        if (!GetSetPokedexFlag(gMonLinkData.species3, FLAG_GET_CAUGHT))
        {
            GetSetPokedexFlag(gMonLinkData.species3, FLAG_SET_SEEN);
            GetSetPokedexFlag(gMonLinkData.species3, FLAG_SET_CAUGHT);
        }
        for (i = 0; i < GIFT_RIBBONS_COUNT; i++)
        {
            if (gGiftRibbonsPtr[i] == 0 && gMonLinkData.giftRibbons[i] != 0)
            {
                gGiftRibbonsPtr[i] = gMonLinkData.giftRibbons[i];
                GiveGiftRibbonToParty(i, gMonLinkData.giftRibbons[i]);
            }
        }
    }

    result = FinalizeLinkSave();
    switch (result)
    {
    case 0:
        WarningPrint(1);
        break;
    case 1:
        WarningPrint(4);
        break;
    case 2:
        WarningPrint(5);
        break;
    }

    gMonLinkData.partyMonIncoming = 0;
    gMonLinkData.transferComplete = 0;
    gMonLinkData.cmd44Received = 0;
    return result;
}

void BufferFirstPartyMon(void)
{
    u16 species;
    u32 oldSpecies = gMonLinkData.species;
    if (oldSpecies != 0)
        return;

#ifndef PROGRAM_COUPON
    gMonLinkData.partyMenuLayout = 2;
#endif
    gMonLinkData.personality = GetMonData(&gPlayerPartyPtr[0], MON_DATA_PERSONALITY, NULL);
    gMonLinkData.otId = GetMonData(&gPlayerPartyPtr[0], MON_DATA_OT_ID, NULL);
    species = GetMonData(&gPlayerPartyPtr[0], MON_DATA_SPECIES, NULL);
    gMonLinkData.species = species;
    gMonLinkData.castformForm = oldSpecies;
    gMonLinkData.moves[0] = GetBoxMonMoveBySlot(&gPlayerPartyPtr[0].box, 0);
    gMonLinkData.moves[1] = GetBoxMonMoveBySlot(&gPlayerPartyPtr[0].box, 1);
    gMonLinkData.moves[2] = GetBoxMonMoveBySlot(&gPlayerPartyPtr[0].box, 2);
    gMonLinkData.moves[3] = GetBoxMonMoveBySlot(&gPlayerPartyPtr[0].box, 3);
    gMonLinkData.pps[0] = GetBoxMonPPByMoveSlot(&gPlayerPartyPtr[0].box, 0);
    gMonLinkData.pps[1] = GetBoxMonPPByMoveSlot(&gPlayerPartyPtr[0].box, 1);
    gMonLinkData.pps[2] = GetBoxMonPPByMoveSlot(&gPlayerPartyPtr[0].box, 2);
    gMonLinkData.pps[3] = GetBoxMonPPByMoveSlot(&gPlayerPartyPtr[0].box, 3);
}

u32 GetSwitchOutRestriction(void)
{
#ifdef PROGRAM_COUPON
    return (gMonLinkData.battleRestriction) | (gMonLinkData.trappingMonId << 16);
#else
    return (gMonLinkData.switchOutRestriction) | (gMonLinkData.trappingMonId << 16);
#endif
}

u32 GetLinkSelectedMonId(void)
{
    if (gMonLinkData.selectedMonIdPlusOne != 0)
        return gMonLinkData.selectedMonIdPlusOne - 1;
    else
        return 0;
}

u32 ShouldUseLinkPartyMenu(void)
{
    return gMonLinkData.usePartyMenu;
}

u32 GetNibbleByIndex(u32 value, s32 index)
{
    if (index > 0)
    {
        do
        {
            value >>= 4;
            index--;
        } while (index != 0);
    }

    return value & 0xF;
}

// The slot's nibble of partyCountNibblefield: 0xF if the slot is occupied.
u32 UNUSED IsPartySlotOccupied(s32 slot)
{
    u32 partyCountNibbles = gMonLinkData.partyCountNibblefield;
    if (slot > 0)
    {
        s32 remaining = slot;
        do
        {
            partyCountNibbles >>= 4;
            remaining--;
        } while (remaining != 0);
    }

    return partyCountNibbles & 0xF;
}
