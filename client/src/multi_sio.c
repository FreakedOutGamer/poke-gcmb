#include "global.h"
#include "multi_sio.h"

// Nintendo AGB multiplayer serial communication library (MultiSio010918).
// Ported from the Aura Mew distribution decomp, which uses the same version.

const char gMultiSioLibVer[] = "MultiSio010918";

void UNUSED MultiSioInit(void)
{
    int i;

    REG_IME = 0;
    REG_IE &= ~(INTR_FLAG_SERIAL | INTR_FLAG_TIMER3);
    REG_IME = 1;

    REG_RCNT = 0;
    *(vu32 *)REG_ADDR_SIOCNT = SIO_MULTI_MODE;
    REG_SIOCNT |= SIO_INTR_ENABLE | SIO_115200_BPS;

    CpuFill32(0, &gMultiSioArea, sizeof(gMultiSioArea));
    CpuCopy32(MultiSioRecvBufChange, gMultiSioRecvFuncBuf, sizeof(gMultiSioRecvFuncBuf));
    CpuCopy32(MultiSioIntr, gMultiSioIntrFuncBuf, sizeof(gMultiSioIntrFuncBuf));

    gMultiSioArea.sendBufCounter = -1;
    gMultiSioArea.nextSendBufp = &gMultiSioArea.sendBuf[0];
    gMultiSioArea.currentSendBufp = &gMultiSioArea.sendBuf[1];

    for (i = 0; i < MULTI_SIO_PLAYERS_MAX; i++)
    {
        gMultiSioArea.currentRecvBufp[i] = &gMultiSioArea.recvBuf[i][0];
        gMultiSioArea.lastRecvBufp[i] = &gMultiSioArea.recvBuf[i][1];
        gMultiSioArea.recvCheckBufp[i] = &gMultiSioArea.recvBuf[i][2];
    }

    REG_IME = 0;
    REG_IE |= INTR_FLAG_SERIAL;
    REG_IME = 1;
}

void UNUSED MultiSioStart(void)
{
    if (gMultiSioArea.type)
        gMultiSioArea.startFlag = 1;
}

void UNUSED MultiSioStop(void)
{
    REG_IME = 0;
    REG_IE &= ~(INTR_FLAG_SERIAL | INTR_FLAG_TIMER3);
    REG_IME = 1;

    REG_SIOCNT = SIO_MULTI_MODE | SIO_115200_BPS;
    REG_TM3CNT = MULTI_SIO_TIMER_COUNT;
    REG_IF = INTR_FLAG_SERIAL | INTR_FLAG_TIMER3;

    gMultiSioArea.startFlag = 0;
}

u32 UNUSED MultiSioMain(void *sendp, void *recvp)
{
    struct SioMultiCnt siocnt = *(struct SioMultiCnt *)REG_ADDR_SIOCNT;
    u32 ret;

    switch (gMultiSioArea.state)
    {
    case 0:
        if (siocnt.id == 0)
        {
            if (siocnt.sd == 1 && siocnt.enable == 0)
            {
                if (siocnt.si == 0 && gMultiSioArea.sendBufCounter == -1)
                {
                    REG_IME = 0;
                    REG_IE &= ~INTR_FLAG_SERIAL;
                    REG_IE |= INTR_FLAG_TIMER3;
                    REG_IME = 1;

                    ((struct SioMultiCnt *)REG_ADDR_SIOCNT)->intrEnable = 0;
                    REG_IF = INTR_FLAG_SERIAL | INTR_FLAG_TIMER3;
                    REG_TM3CNT = MULTI_SIO_TIMER_COUNT;

                    gMultiSioArea.type = SIO_MULTI_SD;
                }
            }
            else
            {
                break;
            }
        }
        gMultiSioArea.state = 1;
    case 1:
        if (gMultiSioArea.connectedFlags)
        {
            if (gMultiSioArea.connectCount < 8)
                gMultiSioArea.connectCount++;
            else
                gMultiSioArea.state = 2;
        }
    case 2:
        MultiSioRecvDataCheck(recvp);
        MultiSioSendDataSet(sendp);
        break;
    }

    gMultiSioArea.sendFrameCounter++;

    ret = (gMultiSioArea.recvSuccessFlags | (gMultiSioArea.type == SIO_MULTI_SD ? MULTI_SIO_PARENT : 0))
        | (gMultiSioArea.connectedFlags << 8)
        | (gMultiSioArea.hardError ? MULTI_SIO_HARD_ERROR : 0);

    return (ret | (siocnt.id >= MULTI_SIO_PLAYERS_MAX ? MULTI_SIO_ID_OVER_ERROR : 0))
         | ((gMultiSioArea.connectCount >> 3) << 15);
}

void MultiSioSendDataSet(void *sendp)
{
    s32 checkSum = 0;
    u32 i;

    gMultiSioArea.nextSendBufp->frameCounter = gMultiSioArea.sendFrameCounter;
    gMultiSioArea.nextSendBufp->recvErrorFlags = gMultiSioArea.connectedFlags ^ gMultiSioArea.recvSuccessFlags;
    gMultiSioArea.nextSendBufp->checkSum = 0;
    CpuCopy32(sendp, gMultiSioArea.nextSendBufp->data, MULTI_SIO_BLOCK_SIZE);

    for (i = 0; i < sizeof(struct MultiSioPacket) / 2 - 2; i++)
        checkSum += ((u16 *)gMultiSioArea.nextSendBufp)[i];

    gMultiSioArea.nextSendBufp->checkSum = ~checkSum - 12;

    if (gMultiSioArea.type)
        REG_TM3CNT_H = 0;

    gMultiSioArea.sendBufCounter = -1;

    if (gMultiSioArea.type && gMultiSioArea.startFlag)
        REG_TM3CNT_H = TIMER_ENABLE | TIMER_INTR_ENABLE;
}

u32 MultiSioRecvDataCheck(void *recvp)
{
    u32 (*recvBufChange)(void) = (u32 (*)(void))gMultiSioRecvFuncBuf;
    vu32 unused = 0;
    u8 syncRecvFlag[MULTI_SIO_PLAYERS_MAX];
    s32 checkSum;
    s32 i;
    u32 j;

    *(u32 *)syncRecvFlag = recvBufChange();
    gMultiSioArea.recvSuccessFlags = 0;

    for (i = 0; i < MULTI_SIO_PLAYERS_MAX; i++)
    {
        checkSum = 0;
        for (j = 0; j < sizeof(struct MultiSioPacket) / 2 - 2; j++)
            checkSum += ((u16 *)gMultiSioArea.recvCheckBufp[i])[j];

        if (syncRecvFlag[i] && (s16)checkSum == -13)
        {
            CpuCopy32(gMultiSioArea.recvCheckBufp[i]->data, (u8 *)recvp + i * MULTI_SIO_BLOCK_SIZE, MULTI_SIO_BLOCK_SIZE);
            gMultiSioArea.recvSuccessFlags |= 1 << i;
        }

        CpuFill32(0, gMultiSioArea.recvCheckBufp[i]->data, MULTI_SIO_BLOCK_SIZE);
    }

    gMultiSioArea.connectedFlags |= gMultiSioArea.recvSuccessFlags;

    return gMultiSioArea.recvSuccessFlags;
}
