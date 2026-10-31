#include "global.h"
#include "player_record.h"
#include "pokemon.h"
#include "constants/species.h"
#include "colbtl_version.h"
#include "gflib/keys.h"
#include "colbtl.h"
#include "joybus.h"
#include "target_game.h"
#include "gba/syscall.h"





extern void SoftResetRom(u32);


static void SetLinkCommandReady(bool32 val)
{
    gLinkCommandReady = val;
}

bool32 IsLinkCommandReady(void)
{
    return gLinkCommandReady;
}

void ReceiveMonData(u32 val)
{
    s32 i;

    switch (gJoyBusState.state)
    {
    case 0:
        gJoyBusState.buffer[gJoyBusState.transferBytes / 4] = val;
        gJoyBusState.transferBytes += 4;

        if (gJoyBusState.transferBytes >= gJoyBusState.transferSize)
        {
            u32 partyCountNibbles = gMonLinkData.partyCountNibblefield;
            gMonLinkData.partyCount = 0;

            for (i = 0; i < PARTY_SIZE; i++)
            {
                if ((partyCountNibbles & 0xF) != 0xF)
                    gMonLinkData.partyCount++;
                partyCountNibbles >>= 4;
            }

            gJoyBusState.transferBytes = 0;
            gJoyBusState.buffer = (void *) gMonLinkData.monData;
            gJoyBusState.transferSize = gMonLinkData.partyCount * sizeof(struct MonData);
            gJoyBusState.state++;
        }
        break;
    case 1:
        gJoyBusState.buffer[gJoyBusState.transferBytes / 4] = val;
        gJoyBusState.transferBytes += 4;

        if (gJoyBusState.transferBytes >= gJoyBusState.transferSize)
        {
            gJoyBusState.transferBytes = 0;
            gJoyBusState.buffer = (void*) gMonLinkData.monName;
            gJoyBusState.transferSize = gMonLinkData.monCount * sizeof(struct MonName);
            gJoyBusState.state++;
        }
        break;
    case 2:
        gJoyBusState.buffer[gJoyBusState.transferBytes / 4] = val;
        gJoyBusState.transferBytes += 4;

        if (gJoyBusState.transferBytes >= gJoyBusState.transferSize)
        {
            gJoyBusState.command = LINK_CMD_RESET;
            gMonLinkData.linkSessionComplete = 1;
        }
        break;
    }
}

static inline void CopyN(s32 n, u8 *dst, const u8 *src)
{
    while (n != 0)
    {
        *dst = *src;
        src++, dst++;
        n--;
    }
}

#ifdef PROGRAM_COUPON
// coupon.bin copies its event data to the save as plain bytes.
struct ExternalEventDataBytes
{
    u8 bytes[sizeof(struct ExternalEventData2)];
};
#endif

static void ReceiveGiftData(u32 val)
{
    u8 *ptr = (u8 *)(gSaveBlock1Ptr) + gAgbPmRomParams->externalEventDataOffset;
    // Note: cast is needed here to make the code match. The whole struct is declared as volatile, but externalEventData isn't treated as volatile in this function.
    // It's possible only certain members of gMonLinkData were volatile.
    struct ExternalEventData2 *externalEventData = (struct ExternalEventData2 *) &gMonLinkData.externalEventData;

    switch (gJoyBusState.state)
    {
    case 0:
        externalEventData->currentPokeCoupons = val;
        gJoyBusState.state++;
        break;
    case 1:
        externalEventData->totalEarnedPokeCoupons = val;
        gJoyBusState.state++;
        break;
    case 2:
        externalEventData->receivedAgetoCelebi = 0;
        externalEventData->gotBronzePokeCouponTitleReward = 0;
        externalEventData->gotSilverPokeCouponTitleReward = 0;
        externalEventData->gotGoldPokeCouponTitleReward = 0;

        if (val & 1)
            externalEventData->gotGoldPokeCouponTitleReward = 1;

        if (val & 2)
            externalEventData->gotSilverPokeCouponTitleReward = 1;

        if (val & 4)
            externalEventData->gotBronzePokeCouponTitleReward = 1;

        if (val & 8)
            externalEventData->receivedAgetoCelebi = 1;

#ifdef PROGRAM_COUPON
        *(struct ExternalEventDataBytes *)ptr = *(struct ExternalEventDataBytes *)externalEventData;
#else
        CopyN(sizeof(*externalEventData), ptr, (void*) externalEventData);
#endif
        gJoyBusState.state++;
        break;
    case 3:
        gMonLinkData.species3 = val >> 16;
        gJoyBusState.transferSize = val & 0xFFFF;
        gJoyBusState.transferBytes = 0;
        gJoyBusState.buffer = (void *) gPcItemsPtr;
        gJoyBusState.state++;
        break;
    case 4:
        gJoyBusState.buffer[gJoyBusState.transferBytes++] = val;

        if (gJoyBusState.transferBytes >= gJoyBusState.transferSize)
        {
            if (gMonLinkData.species3 == SPECIES_NONE)
            {
                gJoyBusState.command = LINK_CMD_RESET;
                gMonLinkData.pokemonTransferPending = 1;
            }
            else
            {
                gJoyBusState.transferSize = sizeof(struct Pokemon);
                gJoyBusState.transferBytes = 0;
                gJoyBusState.buffer = (void *) GetPtrToEmptyPartySlot();
                gJoyBusState.state++;
            }
        }
        break;
    case 5:
        gJoyBusState.buffer[gJoyBusState.transferBytes / 4] = val;
        gJoyBusState.transferBytes += 4;

        if (gJoyBusState.transferBytes >= gJoyBusState.transferSize)
        {
            gJoyBusState.transferSize = sizeof(gMonLinkData.giftRibbons);
            gJoyBusState.transferBytes = 0;
            gJoyBusState.buffer = (void *) gMonLinkData.giftRibbons;
            gJoyBusState.state++;
        }
        break;
    case 6:
        gJoyBusState.buffer[gJoyBusState.transferBytes / 4] = val;
        gJoyBusState.transferBytes += 4;

        if (gJoyBusState.transferBytes >= gJoyBusState.transferSize)
        {
            gMonLinkData.pokemonTransferPending = 1;
            gJoyBusState.command = LINK_CMD_RESET;
        }
        break;
    }
}

bool32 ProcessReceiveCommand(u32 val)
{
    u32 wasLinked = gJoyBusState.linked;

    if (wasLinked == 0)
    {
        if (gJoyBusState.handshakeStep != 2)
        {
            RET_FALSE:
                return FALSE;
        }

        if (val == gJoyBusState.gameCodeLower)
        {
            REG_JOYSTAT = 0x10;
            gJoyBusState.handshakeWord = val;
            gJoyBusState.linked = 1;
            gJoyBusState.command = LINK_CMD_0x01;
        }
        else if (val == gJoyBusState.gameCodeRaw)
        {
            REG_JOYSTAT = wasLinked;
            gJoyBusState.handshakeWord = val;
            gJoyBusState.linked = 1;
            gJoyBusState.command = LINK_CMD_RESET;
        }
        else
        {
            return FALSE;
        }

        gJoyBusState.handshakeStep = 0;
        gJoyBusState.linkTimedOut = 0;
        return TRUE;
    }
    else
    {
        s32 currentCmd = gJoyBusState.command;

        switch (currentCmd)
        {
        case LINK_CMD_RESET:
            val >>= 24;
            if (val == LINK_CMD_READ_INPUT)
            {
                REG_JOY_TRANS = (gHeldKeys << 0x10) | val;
            }
            else if (val == LINK_CMD_TRAN_PLAYER_DATA1)
            {
                gJoyBusState.buffer = (void *)GetPlayerLinkInfo();
#if COLBTL_IS(COLBTL_JP) || defined(PROGRAM_COUPON)
                if (GetPlayerMapType() & POKECENTER_SAVEWARP)
                {
#endif
                REG_JOY_TRANS = val;
                gJoyBusState.transferSize = sizeof(struct PlayerLinkInfo);
                gJoyBusState.transferBytes = currentCmd; // 0
                gJoyBusState.command = LINK_CMD_TRAN_PLAYER_DATA2;
#if COLBTL_IS(COLBTL_JP) || defined(PROGRAM_COUPON)
                }
                else
                    REG_JOY_TRANS = currentCmd;
#endif
            }
            else if (val == LINK_CMD_RECV_TEXT)
            {
                gJoyBusState.buffer = (void *) GetTextBufferPointer(0);
                SetLinkCommandReady(0);
                REG_JOY_TRANS = val;
                gJoyBusState.transferSize = sizeof(*gTextBuffer) * 6;
                gJoyBusState.transferBytes = currentCmd; // 0
                gJoyBusState.command = val;
            }
            else if (val == LINK_CMD_RECV_UNKNOWN)
            {
                REG_JOY_TRANS = val;
                gJoyBusState.command = val;
            }
            else if (val == LINK_CMD_RECV_MON_DATA)
            {
                REG_JOY_TRANS = val;
                gJoyBusState.command = val;
                gJoyBusState.buffer = (void*) &gMonLinkData;
                gJoyBusState.transferSize = 0x24; // Everything before monData
                gJoyBusState.transferBytes = currentCmd; // 0
                gJoyBusState.state = currentCmd; // 0
                gMonLinkData.linkSessionComplete = currentCmd; // 0
            }
            else if (val == LINK_CMD_RECV_PARTY_MON)
            {
                gMonLinkData.partyMonIncoming = 1;
                gMonLinkData.transferComplete = 0;
                REG_JOY_TRANS = val;
                gJoyBusState.command = val;
#ifdef PROGRAM_COUPON
                gJoyBusState.state = 0;
#else
                gJoyBusState.state = currentCmd; // 0
#endif
            }
            else if (val == LINK_CMD_0x44)
            {
                REG_JOY_TRANS = val;
                gJoyBusState.command = val;
                gMonLinkData.cmd44Received = 1;
                gMonLinkData.cmd44Active = 1;
            }
            else if (val == LINK_CMD_TRAN_GIFT_DATA || val == LINK_CMD_RECV_GIFT_DATA)
            {
                REG_JOY_TRANS = val;
                gJoyBusState.command = val;
                gJoyBusState.state = 0;
            }
#ifndef PROGRAM_COUPON
            else if (val == LINK_CMD_SOFT_RESET_ROM)
            {
                REG_JOY_TRANS = val;
                REG_IME = 0;
                SoftResetRom(0);
            }
#endif
#if !COLBTL_IS(COLBTL_JP) && !defined(PROGRAM_COUPON)
            else if (val == LINK_CMD_SOFT_RESET)
            {
                REG_JOY_TRANS = val;
                REG_IME = 0;
                SoftReset(0);
            }
#endif
            break;
        case LINK_CMD_RECV_TEXT:
            gJoyBusState.buffer[gJoyBusState.transferBytes / 4] = val;
            gJoyBusState.transferBytes += 4;

            if (gJoyBusState.transferBytes >= gJoyBusState.transferSize)
            {
                REG_JOYSTAT = 0;
                gJoyBusState.command = LINK_CMD_RESET;
                gJoyBusState.linked = 0;
                SetLinkCommandReady(1);
            }
            break;
        case LINK_CMD_RECV_UNKNOWN:
            gMonLinkData.partyMenuLayout = val >> 24;
            gMonLinkData.numMonsToSelect = val;
            gMonLinkData.partySelectionReceived = 1;
#ifndef PROGRAM_COUPON
            gJoyBusState.command = LINK_CMD_RESET;
#endif
            break;
        case LINK_CMD_RECV_MON_DATA:
            ReceiveMonData(val);
            break;
        case LINK_CMD_RECV_PARTY_MON:
            ReceivePartyMon(val);
            break;
        case LINK_CMD_RECV_GIFT_DATA:
            ReceiveGiftData(val);
            break;
#ifdef PROGRAM_COUPON
        case LINK_CMD_0x02:
            gJoyBusState.fillWord = val;
            gJoyBusState.command = LINK_CMD_0x03;
            REG_JOY_TRANS = val;
            gJoyBusState.transferBytes = 4;
            break;
#endif
        default:
            // The original has one return FALSE, the handshake's above; a
            // second return here would compile to a second copy.
            goto RET_FALSE;
        }
    }
    return TRUE;
}

static void UNUSED CopyBytes(u8 *dst, const u8 *src, s32 n)
{
    CopyN(n, dst, src);
}
