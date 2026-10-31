#include "global.h"
#include "gba/gba.h"
#include "libpmagb/save_sector.h"
#include "target_game.h"
#include "berry_fix.h"
#include "gflib/keys.h"
#include "libpmagb/save_block_pointers.h"
#include "pokemon.h"
#include "receive_jirachi.h"
#include "startup.h"

// The distributor's main sequence identifies the cartridge, loads its save,
// runs the Berry Program Update, then hands over to the receive scene.

// Interrupt-table slot passed to the save timer initialization.
extern void (**gSaveTimerIntrSlotPtr)(void);
extern u8 gTargetGameIdentificationResult;  // DetectROM's result
extern u8 gSaveLoadResult;  // ReadSaveBlockChunks's result
extern u8 gBerryFixResult;  // the BerryFix loop's summary result
extern u8 gMonLinkData[];


// Steps BerryFix's state machine to the end. Returns 1 if the clock was
// fixed, 2 on an error, and 0 otherwise.
static inline u32 RunBerryFix(void)
{
    u8 unused1[64];
    u32 berryFixState = STATE_INIT;
    u32 berryFixResult = 0;

    while (berryFixState != STATE_NO_NEED_TO_FIX
          && berryFixState != STATE_ERROR_YEAR
          && berryFixState != STATE_ERROR)
    {
        BerryFix(&berryFixState, unused1, NULL);
        if (berryFixState == STATE_FIX_DATE)
            berryFixResult = 1;
        if (berryFixState == STATE_ERROR)
            berryFixResult = 2;
    }
    return berryFixResult;
}

void RunJirachiDistributor(void)
{
    u16 zero;

    // DetectROM returns u8, but its result is stored here without
    // being narrowed, as if it returned int.
    gTargetGameIdentificationResult = ((u32 (*)(void))DetectROM)();
    InitFlash(2, gSaveTimerIntrSlotPtr);
    SaveBlocksInit();
    SetSaveSectorPtrs();
    gSaveLoadResult = ReadSaveBlockChunks();

    gBerryFixResult = RunBerryFix();

    SetKeyRepeatTiming(40, 5);

    zero = 0;
    CpuSet(&zero, gMonLinkData, CPU_SET_16BIT | CPU_SET_SRC_FIXED | 0x442);

    GetPtrToEmptyPartySlot();

    REG_IE = INTR_FLAG_VBLANK;
    REG_DISPSTAT = DISPSTAT_VBLANK_INTR;
    REG_IME = 1;

    ReceiveJirachi();
    SoftReset(0);
}
