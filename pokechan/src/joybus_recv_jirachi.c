#include "global.h"
#include "gba/gba.h"
#include "link_buffers.h"

// The Jirachi distributor's JoyBus receive handler. It does the game-code
// handshake (as in the shared src/joybus_recv.c), then its own commands and
// their data.

extern u16 gHeldKeys;

u32 ProcessReceiveCommand(u32 data)
{
    u32 linked = gJoyBusState.linked;
    s32 currentCmd;
    u32 receivedData;
    if (linked == 0)
    {
        if (gJoyBusState.handshakeStep != 2)
            goto returnFalse;

        if (data == gJoyBusState.gameCodeLower)
        {
            REG_JOYSTAT = 0x10;
            gJoyBusState.handshakeWord = data;
            gJoyBusState.linked = 1;
            gJoyBusState.command = 1;
        }
        else
        {
            if (data != gJoyBusState.gameCodeRaw)
            {
returnFalse:
                return FALSE;
            }

            REG_JOYSTAT = linked;
            gJoyBusState.handshakeWord = data;
            gJoyBusState.linked = 1;
            gJoyBusState.command = 0;
            gJoyBusState.state = 0;
        }

        gJoyBusState.handshakeStep = 0;
        gJoyBusState.linkTimedOut = 0;
        return TRUE;
    }

    receivedData = data;
    currentCmd = gJoyBusState.command;
    switch (currentCmd)
    {
    case 0:
    {
        data >>= 24;

        if (data == 0x77)
        {
            REG_JOY_TRANS = (gHeldKeys << 16) | (JOYBUS_EXTRA(JOYBUS_POLL_STATUS) << 8) | data;
        }
        else if (data == 0xAA)
        {
            JoyBusHandleCmdAA(receivedData);
        }
        else if (data == 0x99)
        {
            (void)JOYBUS_EXTRA(0x2AA3);
            JOYBUS_EXTRA(0x2AA3) = currentCmd;
            gJoyBusState.transferBytes = currentCmd;
            gJoyBusState.buffer = (void *)((u8 *)&gJoyBusState + 0x2A38);
            gJoyBusState.transferSize = 0x68;
            gJoyBusState.command = data;
            REG_JOY_TRANS = data;
            (void)JOYBUS_EXTRA(JOYBUS_CMD88_FLAG);
            JOYBUS_EXTRA(JOYBUS_CMD88_FLAG) = 0;
        }
        else if (data == 0x88)
        {
            (void)JOYBUS_EXTRA(JOYBUS_CMD88_FLAG);
            JOYBUS_EXTRA(JOYBUS_CMD88_FLAG) = 1;
        }
        break;
    }
    case 0xAA:
        JoyBusHandleCmdAA(data);
        break;
    case 0x99:
        ((u32 *)gJoyBusState.buffer)[gJoyBusState.transferBytes / 4] = data;
        gJoyBusState.transferBytes += 4;
        if (gJoyBusState.transferBytes >= gJoyBusState.transferSize)
        {
            gJoyBusState.command = 0;
            (void)JOYBUS_EXTRA(JOYBUS_CMD99_DONE);
            JOYBUS_EXTRA(JOYBUS_CMD99_DONE) = 1;
        }
        break;
    default:
        goto returnFalse;
    }

    return TRUE;
}
