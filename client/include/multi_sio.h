#ifndef GUARD_MULTI_SIO_H
#define GUARD_MULTI_SIO_H

// Nintendo AGB multiplayer serial communication library (MultiSio010918).

#define MULTI_SIO_PLAYERS_MAX   4
#define MULTI_SIO_BLOCK_SIZE    16      // bytes of user data per frame
#define MULTI_SIO_TIMER_COUNT   0xB1FC  // timer 3 reload value (master)

// Bits of the value returned by MultiSioMain().
#define MULTI_SIO_RECV_ID_MASK      0x000F
#define MULTI_SIO_PARENT            0x0080
#define MULTI_SIO_CONNECTED_ID_MASK 0x0F00
#define MULTI_SIO_HARD_ERROR        0x1000
#define MULTI_SIO_ID_OVER_ERROR     0x2000
#define MULTI_SIO_LD_ENABLE         0x8000

struct MultiSioPacket
{
    u8 frameCounter;
    u8 recvErrorFlags;
    u16 checkSum;
    u16 data[MULTI_SIO_BLOCK_SIZE / 2];
    u16 overRunCatch[2];
};

struct MultiSioArea
{
    /*0x00*/ u8 type;               // SIO_MULTI_SD for the parent
    /*0x01*/ u8 state;
    /*0x02*/ u8 connectedFlags;
    /*0x03*/ u8 recvSuccessFlags;
    /*0x04*/ u8 syncRecvFlag[MULTI_SIO_PLAYERS_MAX];
    /*0x08*/ u8 startFlag;
    /*0x09*/ u8 hardError;
    /*0x0A*/ u8 connectCount;
    /*0x0B*/ u8 sendFrameCounter;
    /*0x0C*/ u8 recvFrameCounter[MULTI_SIO_PLAYERS_MAX][2];
    /*0x14*/ s32 sendBufCounter;
    /*0x18*/ s32 recvBufCounter[MULTI_SIO_PLAYERS_MAX];
    /*0x28*/ struct MultiSioPacket *nextSendBufp;
    /*0x2C*/ struct MultiSioPacket *currentSendBufp;
    /*0x30*/ struct MultiSioPacket *currentRecvBufp[MULTI_SIO_PLAYERS_MAX];
    /*0x40*/ struct MultiSioPacket *lastRecvBufp[MULTI_SIO_PLAYERS_MAX];
    /*0x50*/ struct MultiSioPacket *recvCheckBufp[MULTI_SIO_PLAYERS_MAX];
    /*0x60*/ struct MultiSioPacket sendBuf[2];
    /*0x90*/ struct MultiSioPacket recvBuf[MULTI_SIO_PLAYERS_MAX][3];
}; /* size = 0x1B0 */

extern struct MultiSioArea gMultiSioArea;
extern u32 gMultiSioRecvFuncBuf[0x40 / 4];
extern u32 gMultiSioIntrFuncBuf[0x140 / 4];

void UNUSED MultiSioInit(void);
void UNUSED MultiSioStart(void);
void UNUSED MultiSioStop(void);
u32 UNUSED MultiSioMain(void *sendp, void *recvp);
void MultiSioSendDataSet(void *sendp);
u32 MultiSioRecvDataCheck(void *recvp);

// ARM routines copied to IWRAM by MultiSioInit.
u32 MultiSioRecvBufChange(void);
void MultiSioIntr(void);

#endif // GUARD_MULTI_SIO_H
