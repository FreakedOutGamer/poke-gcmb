#include "global.h"
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

static inline void CopyN(s32 n, u8 *dst, const u8 *src)
{
    while (n != 0)
    {
        *dst = *src;
        src++, dst++;
        n--;
    }
}

void TransmitGiftData(void)
{
    bool32 gameClear;
    u32 joyTransVal;
    u8 *ptr = (u8 *)(gSaveBlock1Ptr) + gAgbPmRomParams->externalEventDataOffset;
    struct ExternalEventData2 *externalEventData = (struct ExternalEventData2 *) &gMonLinkData.externalEventData;

    switch (gJoyBusState.state)
    {
    case 0:
        CopyN(sizeof(*externalEventData), (void*) externalEventData, ptr);
        REG_JOY_TRANS = externalEventData->currentPokeCoupons;
        gJoyBusState.state++;
        break;
    case 1:
        REG_JOY_TRANS = externalEventData->totalEarnedPokeCoupons;
        gJoyBusState.state++;
        break;
    case 2:
        gameClear = CheckGameClear();
        joyTransVal = (externalEventData->gotGoldPokeCouponTitleReward << 0) | (externalEventData->gotSilverPokeCouponTitleReward << 1) | (externalEventData->gotBronzePokeCouponTitleReward << 2) | (externalEventData->receivedAgetoCelebi << 3) | (gameClear << 4);
        REG_JOY_TRANS = joyTransVal;
        gJoyBusState.state++;
        break;
    case 3:
        gJoyBusState.transferSize = gAgbPmRomParams->pcItemsCount;
        gJoyBusState.transferBytes = 0;
        joyTransVal = (*gPlayerPartyCountPtr << 16);
        joyTransVal |= gJoyBusState.transferSize;
        REG_JOY_TRANS = joyTransVal;
        gJoyBusState.buffer = (void *) gPcItemsPtr;
        gJoyBusState.state++;
        break;
    case 4:
        REG_JOY_TRANS = gJoyBusState.buffer[gJoyBusState.transferBytes++];

        if (gJoyBusState.transferBytes == gJoyBusState.transferSize)
        {
            gJoyBusState.command = LINK_CMD_RESET;
            gJoyBusState.linked = 0;
        }
        break;
    }
}

bool32 ProcessTransmitCommand(void)
{
    if (gJoyBusState.linked == 0)
    {
        if (gJoyBusState.handshakeStep == 1)
        {
            gJoyBusState.handshakeStep = 2;
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        switch (gJoyBusState.command)
        {
        case LINK_CMD_TRAN_PLAYER_DATA2:
            if (gJoyBusState.transferBytes < gJoyBusState.transferSize)
            {
                REG_JOY_TRANS = gJoyBusState.buffer[gJoyBusState.transferBytes / 4];
                gJoyBusState.transferBytes += 4;
            }
            else
            {
                REG_JOYSTAT = 0;
                gJoyBusState.command = LINK_CMD_RESET;
                gJoyBusState.linked = 0;
            }
            break;
        case LINK_CMD_RECV_GIFT_DATA:
            break;
        case LINK_CMD_TRAN_GIFT_DATA:
            TransmitGiftData();
            break;
        case LINK_CMD_0x44:
            gMonLinkData.cmd44Active = 0;
            gJoyBusState.command = LINK_CMD_RESET;
            break;
        case LINK_CMD_RECV_PARTY_MON:
        case LINK_CMD_SOFT_RESET_ROM:
        case LINK_CMD_RECV_MON_DATA:
        case LINK_CMD_RECV_UNKNOWN:
        case LINK_CMD_RECV_TEXT:
            break;
        default:
            return FALSE;
        }
    }
    return TRUE;
}

void JoyBusVCountIntr(void)
{
    u32 joyCnt = REG_JOYCNT;

    if (!(joyCnt & JOYCNT_TRAN_CMPLT) || ProcessTransmitCommand() != FALSE)
    {
        if (!(joyCnt & JOYCNT_RECV_CMPLT))
            goto loc_200D850;

        gJoyBusState.recvActive = 1;

        if (ProcessReceiveCommand(REG_JOY_RECV) != FALSE)
            goto loc_200D850;
    }

    REG_JOYSTAT = 0;
    gJoyBusState.linked = 0;
    gJoyBusState.handshakeStep = 0;

loc_200D850:
    if (joyCnt & JOYCNT_RESET)
    {
        u16 UNUSED joyRcv = REG_JOY_RECV;
        REG_JOY_TRANS = gJoyBusState.ownGameCode;
        REG_JOYSTAT = 0;
        gJoyBusState.linked = 0;
        gJoyBusState.handshakeStep = 1;
        gJoyBusState.resetVCount = REG_VCOUNT;
    }

    REG_JOYCNT = joyCnt;
    gJoyBusState.linkTimeoutCounter = 0;
}
