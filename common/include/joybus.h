#ifndef GUARD_JOYBUS_H
#define GUARD_JOYBUS_H

#include "global.h"

// The commands the GameCube sends.
#define LINK_CMD_RESET              0x00
#define LINK_CMD_0x01               0x01
#define LINK_CMD_0x02               0x02 // coupon.bin: receives fillWord, then sends it
#define LINK_CMD_0x03               0x03 // coupon.bin: sends fillWord, 1 KiB
#define LINK_CMD_TRAN_PLAYER_DATA2  0x04
#define LINK_CMD_RECV_GIFT_DATA     0x22
#define LINK_CMD_TRAN_GIFT_DATA     0x33
#define LINK_CMD_0x44               0x44
#define LINK_CMD_RECV_PARTY_MON     0x55
#define LINK_CMD_SOFT_RESET_ROM     0x60
#define LINK_CMD_SOFT_RESET         0x61
#define LINK_CMD_RECV_MON_DATA      0x66
#define LINK_CMD_RECV_UNKNOWN       0x77
#define LINK_CMD_RECV_TEXT          0x88
#define LINK_CMD_TRAN_PLAYER_DATA1  0x99
#define LINK_CMD_READ_INPUT         0xAA

// The JoyBus link's state, at a fixed address in each program.
struct JoyBusState
{
    /*0x00*/ volatile u32 *volatile buffer; // Received into, or sent from, by the current command.
    /*0x04*/ vu32 transferSize; // Expected transfer size in bytes.
    /*0x08*/ u8 filler_08[2];
    /*0x0A*/ vu8 state; // State within the current command.
    /*0x0B*/ vu8 recvActive; // Set by a receive interrupt; cleared on link reset.
    /*0x0C*/ vu8 linked; // The game-code handshake has completed.
    /*0x0D*/ vu8 handshakeStep; // 0: reset, 1: RESET received, 2: SEND completed.
    /*0x0E*/ vu8 linkTimeoutLimit; // Maximum inactive VBlanks before timing out.
    /*0x0F*/ vu8 linkTimeoutCounter; // Inactive VBlanks since the last JoyBus interrupt.
    /*0x10*/ vu8 linkTimedOut; // Set when the timeout limit is exceeded.
    /*0x11*/ vu8 command; // Current command; zero expects a new command word.
    /*0x12*/ vu8 skipRcntInit; // Skips serial-mode initialization during the first reset.
    /*0x13*/ u8 filler_13;
    /*0x14*/ vu8 resetVCount; // Scanline captured on a RESET interrupt.
    /*0x15*/ u8 filler_15[3];
    /*0x18*/ vu32 fillWord; // Sent over and over by coupon.bin's command 0x03.
    /*0x1C*/ vu32 transferBytes; // Transfer progress (words, in the PC-item sender).
    /*0x20*/ vu32 ownGameCode; // Local game code echoed on RESET.
    /*0x24*/ u32 handshakeWord; // Accepted game-code handshake word.
    /*0x28*/ vu32 gameCodeLower; // Lowercase game code accepted by the handshake.
    /*0x2C*/ vu32 gameCodeRaw; // Original game code accepted by the handshake.
    /*0x30*/ vu32 makerCodeWord; // Header bytes 4-7, packed most-significant byte first.
    /*0x34*/ vu32 recvFlags; // Bit 0: receive a gift; bit 1: display the first graphics window.
};

extern struct JoyBusState gJoyBusState;

u8 JoyBusHandshake(void);
void JoyBusSendResult(u32 result);
void InitJoyBus(void);
// Resets the link state and registers gameCode as the game the GameCube talks to.
void InitLink(const u8 *gameCode);
void JoyBusVCountIntr(void);
bool32 ProcessTransmitCommand(void);
u32 ProcessReceiveCommand(u32 data);
void ReceivePartyMon(u32 value);
void ReceiveMonData(u32 value);
void TransmitGiftData(void);
bool32 IsLinkCommandReady(void);
bool32 VBlankCB(void);
void JoyBusHandleCmdAA(u32 data);

#endif // GUARD_JOYBUS_H
