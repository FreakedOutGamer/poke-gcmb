#include "global.h"
#include "libpmagb/save_block_pointers.h"
#include "gba/gba.h"
#include "joybus.h"
#include "target_game.h"
#include "libpmagb/save_sector.h"

// Sends the external-event status, then the connected save's PC items.
// recvBytes counts words here, and recvBuf is the outgoing source.
// The command that reaches this handler has not yet been identified.

extern u8 *gPlayerPartyCountPtr;
extern u8 gJoyBusExternalEventBuf[16];

// The transmitted fields are packed in bytes 8-11 of the event buffer.
// Their meaning is still unknown; the second 24-bit value is in bytes 12-14.
struct EventFlags
{
    u32 unused0[2];
    u32 value : 24;
    u32 flag0 : 1;
    u32 flag1 : 1;
    u32 flag2 : 1;
    u32 flag3 : 1;
    u32 unused1 : 4;
};

static inline void CopyEvent(const u8 *src, u8 *dest, u32 count)
{
    // Initialization order keeps the source live across the switch.
    u32 size = count;
    u8 *d = dest;
    const u8 *s = src;

    while (size != 0)
    {
        *d = *s;
        s++;
        d++;
        size--;
    }
}

void UNUSED TransmitGiftData(void)
{
    u8 *externalEventData = gSaveBlock1Ptr + gAgbPmRomParams->externalEventDataOffset;
    u8 *buf = gJoyBusExternalEventBuf;
    u32 status;
    u32 count;
    volatile u32 *trans;

    switch (gJoyBusState.state)
    {
    case 0:
        CopyEvent(externalEventData, buf, 16);
        REG_JOY_TRANS = ((struct EventFlags *)buf)->value;
        gJoyBusState.state++;
        break;

    case 1:
        REG_JOY_TRANS = ((struct EventFlags *)(buf + 4))->value;
        gJoyBusState.state++;
        break;

    case 2:
        // CheckGameClear returns bool8, but its result is used here
        // without being narrowed, as in JoyBusHandleCmdAA.
        status = ((struct EventFlags *)buf)->flag0
               | (((struct EventFlags *)buf)->flag1 << 1)
               | (((struct EventFlags *)buf)->flag2 << 2)
               | (((struct EventFlags *)buf)->flag3 << 3)
               | (((u32 (*)(void))CheckGameClear)() << 4);
        REG_JOY_TRANS = status;
        gJoyBusState.state++;
        break;

    case 3:
        gJoyBusState.transferSize = gAgbPmRomParams->pcItemsCount;
        gJoyBusState.transferBytes = 0;
        status = *gPlayerPartyCountPtr << 16;
        // The original reloads the count after writing recvBytes.
        status |= *(volatile u32 *)&gJoyBusState.transferSize;
        REG_JOY_TRANS = status;
        gJoyBusState.buffer = (void *)gPcItemsPtr;
        gJoyBusState.state++;
        break;

    case 4:
        trans = &REG_JOY_TRANS;
        count = gJoyBusState.transferBytes;
        *trans = ((u32 *)gJoyBusState.buffer)[count];
        gJoyBusState.transferBytes = count + 1;
        if (gJoyBusState.transferBytes == gJoyBusState.transferSize)
        {
            gJoyBusState.command = 0;
            gJoyBusState.linked = 0;
        }
        break;
    }
}
