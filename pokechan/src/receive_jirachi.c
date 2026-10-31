#include "global.h"
#include "gba/gba.h"
#include "link_buffers.h"
#include "libpmagb/save_sector.h"
#include "target_game.h"
#include "pokedex.h"
#include "pokemon.h"
#include "joybus.h"
#include "fade.h"
#include "gflib/init.h"
#include "libpmagb/save.h"
#include "scene.h"
#include "receive_jirachi.h"

// Receives the Channel gift, copies it into the party, updates its Pokédex
// entry and performs the two-stage save handshake with the GameCube.


extern u16 gNewKeys;
extern u8 *gPlayerPartyCountPtr;

extern vu8 gReceivedGiftMonBuffer[];
extern u8 gReceiveSuccessGraphicsBuffer[];

// This program's own game code, "TEST", copied into EWRAM as part of the
// header; InitLink checks an incoming JoyBus word against it.
#define OWN_GAME_CODE ((void *)0x020000AC)

void ReceiveJirachi(void)
{
    u8 *externalEventFlags = gSaveBlock1Ptr + gAgbPmRomParams->externalEventFlagsOffset;
    u8 *ptr = (u8 *)&gJoyBusState;
    u32 saveResult;
    vu8 *p;

    (void)*(vu8 *)(ptr + JOYBUS_POLL_STATUS);
    *(vu8 *)(ptr + JOYBUS_POLL_STATUS) = 0;

    InitReceiveScene();
    FadeIn();
    InitLink(OWN_GAME_CODE);
    SetVBlankCallback((IntrFunc)VBlankCB);

    ptr += JOYBUS_XFER_DONE;
    while (*(vu8 *)ptr == 0)
        DelayFrames(1);

    // Only the optional first graphics window waits for a key here.
    if (gJoyBusState.recvFlags & 2)
    {
        UnpackReceivedGraphics((u8 *)&gJoyBusState + 0x38);
        do
            DelayFrames(1);
        while ((gNewKeys & (A_BUTTON | B_BUTTON)) == 0);
    }

    if ((gJoyBusState.recvFlags & 1) == 0)
        SoftReset(0);

    UnpackReceivedGraphics((u8 *)&gJoyBusState + 0xE38);

    if (JOYBUS_EXTRA(JOYBUS_CMD99_DONE) == 0)
    {
        ptr = (u8 *)&gJoyBusState + JOYBUS_CMD99_DONE;
        do
            DelayFrames(1);
        while (*(vu8 *)ptr == 0);
    }

    ptr = (u8 *)GetPtrToEmptyPartySlot();
    CpuCopy16((const u8 *)gReceivedGiftMonBuffer, ptr, sizeof(struct Pokemon));

    (*gPlayerPartyCountPtr)++;
    {
        // Preserve the full volatile word read before setting the event flag.
        u32 species = *(vu32 *)(gReceivedGiftMonBuffer - 4) & 0xFFFF;
        externalEventFlags[2] |= 1;
        SetSpeciesCaughtFlag(species, (struct Pokemon *)ptr);
    }

    saveResult = WriteLinkSave(0);
    if (saveResult == 0)
    {
        gReceivedGiftMonBuffer[0x68] = 1;
        p = gReceivedGiftMonBuffer + 0x66;
        while (*p == 0)
            ;

        saveResult = WriteLinkSave(1);
        if (saveResult == 0)
            REG_JOY_TRANS = 0x88;
        else
            REG_JOY_TRANS = -120;
    }
    else
    {
        gReceivedGiftMonBuffer[0x68] = 0xFF;
    }

    if (saveResult == 0)
        UnpackReceivedGraphics(gReceiveSuccessGraphicsBuffer);

    do
        DelayFrames(1);
    while ((gNewKeys & (A_BUTTON | B_BUTTON)) == 0);

    FadeOut();
    SoftReset(0);
}
