#include "global.h"
#include "gba/gba.h"
#include "link_buffers.h"
#include "target_game.h"

bool32 ProcessTransmitCommand(void)
{
    if (gJoyBusState.linked == 0)
    {
        if (gJoyBusState.handshakeStep != 1)
            return FALSE;

        gJoyBusState.handshakeStep = 2;
    }
    else
    {
        switch (gJoyBusState.command)
        {
        case 0x88:
        case 0x99:
        case 0xAA:
            break;
        case 0xAB:
            REG_JOY_TRANS = *(u32 *)(CART_ROM + 0xAC);
            gJoyBusState.command = 0xAA;
            break;
        default:
            return FALSE;
        }
    }

    return TRUE;
}

// The JOY bus interrupt handler. This is the GBA half of the GameCube link,
// and the path the Jirachi actually arrives over. InitLink installs it as a
// function pointer.
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
            // The send path above jumps in here too, which is why the block
            // sits inside the receive branch rather than after it.
        reset:
            REG_JOYSTAT = 0;
            gJoyBusState.linked = 0;
            gJoyBusState.handshakeStep = 0;
        }
    }

    // The 1 has to be a variable. The target loads it once, tests the reset
    // bit against it and stores it again below. As a literal agbcc builds it
    // twice.
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

// Resets the JoyBus link, hardware and state alike. InitLink sets
// skipRcntInit before its call, so that first reset goes straight to JoyBus
// mode without the REG_RCNT = 0x8000 step (general-purpose serial mode).
// The reset clears skipRcntInit, so any later one does both.
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

    gJoyBusState.recvFlags = -1;

    (void)JOYBUS_EXTRA(JOYBUS_XFER_DONE);
    JOYBUS_EXTRA(JOYBUS_XFER_DONE) = 0;
    (void)JOYBUS_EXTRA(JOYBUS_CMD99_DONE);
    JOYBUS_EXTRA(JOYBUS_CMD99_DONE) = 0;
    (void)JOYBUS_EXTRA(JOYBUS_CMD88_FLAG);
    JOYBUS_EXTRA(JOYBUS_CMD88_FLAG) = 0;
    (void)JOYBUS_EXTRA(0x2AA3);
    JOYBUS_EXTRA(0x2AA3) = 0;

    REG_IME = imeBackup;
}
