#include "global.h"
#include "gba/gba.h"
#include "link_buffers.h"
#include "target_game.h"

extern u8 *gPlayerPartyCountPtr;
extern u8 gTargetGameIdentificationResult;   // DetectROM's result; set by RunJirachiDistributor
extern u8 gSaveLoadResult;   // ReadSaveBlockChunks's result; set by RunJirachiDistributor
extern u8 gBerryFixResult;   // the BerryFix loop's summary result; set by RunJirachiDistributor

void JoyBusHandleCmdAA(u32 data)
{
    u8 *externalEventFlags = gSaveBlock1Ptr + gAgbPmRomParams->externalEventFlagsOffset;
    u32 status;

    switch (gJoyBusState.state)
    {
    case 0:
        status = 0xAA;
        if (gBerryFixResult == 1)
            status |= 0xFF0000;

        if (gTargetGameIdentificationResult != 1 || gAgbPmRomParams->blockJirachiDistribution)
        {
            status |= 0xFF000000;
            goto sendStatus;
        }

        if (gSaveLoadResult == 0)
            status |= 0xEE000000;
        else if (gSaveLoadResult == 2)
            status |= 0xDE000000;
        else if (gSaveLoadResult != 1)
            status |= 0xDD000000;
        else if (externalEventFlags[2] & 1)
            status |= 0xCC000000;
        else if (!CheckIfPokedexIsObtained())
            status |= 0xBB000000;
        // Called through a widened function pointer: CheckGameClear's
        // own bool8 prototype makes the target's own truncate-and-test
        // dance appear on *this* comparison too, which the target does not
        // do here (only on CheckIfPokedexIsObtained's, just above).
        else if (((s32 (*)(void))CheckGameClear)() == 0)
            status |= 0xBB000000;
        else if (*gPlayerPartyCountPtr == PARTY_SIZE)
            status |= 0xAA000000;

sendStatus:
        REG_JOY_TRANS = status;
        gJoyBusState.command = 0xAB;
        gJoyBusState.state = 1;
        break;

    case 1:
        gJoyBusState.recvFlags = data;
        gJoyBusState.transferBytes = 0;
        gJoyBusState.state++;

        switch (gJoyBusState.recvFlags & 3)
        {
        case 0:
            gJoyBusState.command = 0;
            // The target reads this byte before writing it, so the source
            // does too.
            (void)JOYBUS_EXTRA(JOYBUS_XFER_DONE);
            JOYBUS_EXTRA(JOYBUS_XFER_DONE) = 1;
            break;
        case 1:
            gJoyBusState.buffer = (void *)((u8 *)&gJoyBusState + 0xE38);
            gJoyBusState.transferSize = 0x1C00;
            break;
        case 2:
            gJoyBusState.buffer = (void *)((u8 *)&gJoyBusState + 0x38);
            gJoyBusState.transferSize = 0xE00;
            break;
        case 3:
            gJoyBusState.buffer = (void *)((u8 *)&gJoyBusState + 0x38);
            gJoyBusState.transferSize = 0x2A00;
            break;
        }
        break;

    case 2:
        ((u32 *)gJoyBusState.buffer)[gJoyBusState.transferBytes / 4] = data;
        gJoyBusState.transferBytes += 4;
        if (gJoyBusState.transferBytes >= gJoyBusState.transferSize)
        {
            gJoyBusState.command = 0;
            (void)JOYBUS_EXTRA(JOYBUS_XFER_DONE);
            JOYBUS_EXTRA(JOYBUS_XFER_DONE) = 1;
        }
        break;
    }
}
