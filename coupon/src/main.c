#include "global.h"
#include "gba/gba.h"
#include "libpmagb/save_sector.h"
#include "target_game.h"
#include "joybus.h"
#include "colbtl.h"
#include "gflib/init.h"
#include "gflib/keys.h"
#include "gflib/sound.h"
#include "gflib/text.h"
#include "libpmagb/save_block_pointers.h"
#include "fade.h"
#include "link_message.h"

// The program's main sequence identifies the cartridge, loads its save, sets
// up the display and the JoyBus link, then leaves everything to the GameCube.
// It is colbtl's GF_Main without the Berry Program Update or the battle menus.

// Interrupt-table slot passed to the save timer initialization.
extern void (**gSaveTimerIntrSlotPtr)(void);
extern u8 gSaveLoadResult;  // ReadSaveBlockChunks's result
extern const u8 RomHeaderGameCode[];


void GF_Main(void)
{
    DetectROM();
    InitFlash(2, gSaveTimerIntrSlotPtr);
    SaveBlocksInit();
    SetSaveSectorPtrs();
    gSaveLoadResult = ReadSaveBlockChunks();
    InitSound();
    SetKeyRepeatTiming(40, 5);
    REG_IE = INTR_FLAG_VBLANK;
    REG_DISPSTAT = DISPSTAT_VBLANK_INTR;
    REG_DISPCNT &= ~DISPCNT_FORCED_BLANK;
    REG_IME = 1;
    SetPlayerLinkInfo(gSaveBlock2Ptr, gSaveBlock1Ptr, gSaveLoadResult);
    InitLink(RomHeaderGameCode);
    SetVBlankCallback((IntrFunc)VBlankCB);
    PauseSoundVSync();
    GenerateFontHalfrowLookupTable((u32 *)0x03000000);
    FadeOut();
    for (;;)
        RunLinkStandbyLoop();
}
