#include "global.h"
#include "libpmagb/save_block_pointers.h"
#include "gba/gba.h"
#include "joybus.h"
#include "colbtl.h"
#include "target_game.h"

extern u8 *gPlayerPartyCountPtr;

struct EventFlags
{
    u8 value[3];
    u8 flag0:1;
    u8 flag1:1;
    u8 flag2:1;
    u8 flag3:1;
    u8 unused1:4;
};

struct EventBuffer
{
    struct EventFlags entries[2];
};

void TransmitGiftData(void)
{
    u8 *externalEventData = gSaveBlock1Ptr
                          + gAgbPmRomParams->externalEventDataOffset;
    u8 *buf = (u8 *)&gMonLinkData.externalEventData;
    u32 status;
    u32 count;
    volatile u32 *trans;

    switch (gJoyBusState.state)
    {
    case 0:
        *(struct EventBuffer *)buf = *(struct EventBuffer *)externalEventData;
        REG_JOY_TRANS = (*(u32 *)&((struct EventBuffer *)buf)->entries[0] << 8) >> 8;
        gJoyBusState.state++;
        break;
    case 1:
        REG_JOY_TRANS = (*(u32 *)&((struct EventBuffer *)buf)->entries[1] << 8) >> 8;
        gJoyBusState.state++;
        break;
    case 2:
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
