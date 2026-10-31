#include "global.h"
#include "gba/gba.h"
#include "joybus.h"
#include "colbtl.h"
#include "gflib/init.h"

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
        case LINK_CMD_0x03:
            if (gJoyBusState.transferBytes < 0x400)
            {
                REG_JOY_TRANS = gJoyBusState.fillWord;
                gJoyBusState.transferBytes += 4;
            }
            else
            {
                REG_JOYSTAT = 0;
                gJoyBusState.command = LINK_CMD_RESET;
                gJoyBusState.linked = 0;
            }
            break;
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
        case LINK_CMD_TRAN_GIFT_DATA:
            TransmitGiftData();
            break;
        case LINK_CMD_0x44:
            gMonLinkData.cmd44Active = 0;
            gJoyBusState.command = LINK_CMD_RESET;
            break;
        case LINK_CMD_RECV_GIFT_DATA:
        case LINK_CMD_RECV_PARTY_MON:
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
    u16 joycnt = REG_JOYCNT;
    u32 one;

    if (joycnt & JOYCNT_SEND)
    {
        if (ProcessTransmitCommand() == 0)
            goto reset;
    }

    if (joycnt & JOYCNT_RECV)
    {
        gJoyBusState.recvActive = 1;
        if (ProcessReceiveCommand(REG_JOY_RECV) == 0)
        {
reset:
            REG_JOYSTAT = 0;
            gJoyBusState.linked = 0;
            gJoyBusState.handshakeStep = 0;
        }
    }

    one = 1;
    if (joycnt & one)
    {
        REG_JOY_RECV;
        REG_JOY_TRANS = gJoyBusState.ownGameCode;
        REG_JOYSTAT = 0;
        gJoyBusState.linked = 0;
        gJoyBusState.handshakeStep = one;
        gJoyBusState.resetVCount = REG_VCOUNT;
    }

    REG_JOYCNT = joycnt;
    gJoyBusState.linkTimeoutCounter = 0;
}


extern const u32 gCrc32Table[256];

void InitJoyBus(void)
{
    u16 imeBackup = REG_IME;

    REG_IME = 0;
    if (gJoyBusState.skipRcntInit == 0)
        REG_RCNT = 0x8000;
    REG_RCNT = 0xC000;

    REG_JOYSTAT = 0;
    REG_JOY_RECV;
    REG_JOY_TRANS = 0;
    REG_JOYCNT = JOYCNT_RESET | JOYCNT_RECV | JOYCNT_SEND | JOYCNT_INTR_ENABLE;
    REG_IF = INTR_FLAG_SERIAL;
    REG_IE |= INTR_FLAG_SERIAL;

    gJoyBusState.linkTimeoutCounter = 0;
    gJoyBusState.linked = 0;
    gJoyBusState.handshakeStep = 0;
    gJoyBusState.skipRcntInit = 0;
    gJoyBusState.command = 0;
    gJoyBusState.recvActive = 0;

    REG_IME = imeBackup;
}

void InitLink(const u8 *gameCode)
{
    const u8 *src = gameCode;
    u16 imeBackup = REG_IME;
    u32 i;

    REG_IME = 0;
    for (i = 0; i <= 0x33; i++)
        ((u8 *)&gJoyBusState)[i] = 0;

    gJoyBusState.skipRcntInit = 1;
    InitJoyBus();
    REG_IE |= INTR_FLAG_SERIAL;

    if (src[0] == 'T' && src[1] == 'E' && src[2] == 'S' && src[3] == 'T')
        gJoyBusState.linkTimeoutLimit = 30;
    else
        gJoyBusState.linkTimeoutLimit = 10;

    gJoyBusState.ownGameCode = (src[3] << 24) | (src[2] << 16) | (src[1] << 8) | src[0];
    gJoyBusState.gameCodeRaw = gJoyBusState.ownGameCode;
    gJoyBusState.gameCodeLower = gJoyBusState.ownGameCode | 0x20202020;
    gJoyBusState.makerCodeWord = (src[4] << 24) | (src[5] << 16) | (src[6] << 8) | src[7];

    SetIntrFunc(0, JoyBusVCountIntr);
    REG_IME = imeBackup;
}

u32 VBlankCB(void)
{
    if (((REG_KEYINPUT ^ 0x3FF) & 4) == 0 && gJoyBusState.recvActive != 0)
    {
        if (gJoyBusState.linkTimeoutCounter > gJoyBusState.linkTimeoutLimit)
        {
            gJoyBusState.linkTimedOut = 1;
            SoftReset(0);
            InitJoyBus();
            return 1;
        }

        REG_IME = 0;
        gJoyBusState.linkTimeoutCounter++;
        REG_IME = 1;
    }
    return 0;
}

u32 UNUSED CalcCRC32(const u8 *data, u32 size)
{
    u32 i = 0;
    u32 crc = 0xAA478422;

    do
    {
        crc = (crc >> 8) ^ gCrc32Table[(crc ^ data[i]) & 0xFF];
        i++;
    } while (i != size);
    return crc;
}
