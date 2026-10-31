#include "global.h"
#include "gflib/sprite.h"
#include "gba/gba.h"
#include "gflib/bg.h"
#include "gflib/init.h"
#include "gflib/intr.h"
#include "gflib/keys.h"
#include "startup.h"
#include "fade.h"

extern u8 __image_end[];
extern u8 __bss_end[];


extern u16 gHeldKeys;
extern u32 gVBlankCounter;
extern IntrFunc gVBlankCallback;
extern IntrFunc gIntrTable[]; // read by IntrMain


#define SOFT_RESET_KEYS (A_BUTTON | B_BUTTON | SELECT_BUTTON | START_BUTTON)

void InitIntr(void);

// BLDY counts down from full black, so the screen appears.
void FadeIn(void)
{
    s32 i;

    DelayFrames(1);
    REG_BLDY = 31;
    REG_BLDCNT = BLDCNT_TGT1_ALL | BLDCNT_EFFECT_DARKEN;
    for (i = 29; i > 0; i -= 2)
    {
        DelayFrames(1);
        REG_BLDY = i;
    }
    REG_BLDCNT = 0;
}

void FadeOut(void)
{
    s32 i;

    DelayFrames(1);
    REG_BLDY = 0;
    REG_BLDCNT = BLDCNT_TGT1_ALL | BLDCNT_EFFECT_DARKEN;
    for (i = 1; i <= 31; i += 2)
    {
        DelayFrames(1);
        REG_BLDY = i;
    }
    REG_BLDY = 31;
    ResetSprites();
}

void AgbMain(void)
{
    REG_IME = 0;
    RegisterRamReset(RESET_PALETTE | RESET_VRAM | RESET_OAM | RESET_SIO_REGS
                   | RESET_SOUND_REGS | RESET_REGS);
    CpuFill16(0, __image_end, __bss_end - __image_end);
    REG_WAITCNT = 0x45B4;
    InitIntr();
    ResetGpuBuffers();
    ResetSprites();
    RunJirachiDistributor();
    SoftReset(RESET_ALL);
}

u32 UNUSED GetFrameTotal(void)
{
    return gVBlankCounter;
}

// A + B + Select + Start at any point aborts the whole distribution.
void DelayFrames(u32 frames)
{
    while (frames != 0)
    {
        UpdateSprites();
        VBlankIntrWait();
        ReadKeys();
        if ((gHeldKeys & SOFT_RESET_KEYS) == SOFT_RESET_KEYS)
            SoftReset(0);
        frames--;
    }
}

// VBlankIntr lives in the same image, so the NULL test can never fail; it
// is there in the source all the same, since agbcc does not fold it.
void InitIntr(void)
{
    u32 i;

    gVBlankCallback = NULL;

    for (i = 0; i < 14; i++)
        gIntrTable[i] = IntrDummy;

    if (VBlankIntr != NULL)
        gIntrTable[1] = VBlankIntr;
    else
        gIntrTable[1] = IntrDummy;
}

void SetIntrFunc(u32 index, IntrFunc func)
{
    if (func != NULL)
        gIntrTable[index] = func;
    else
        gIntrTable[index] = IntrDummy;
}

void SetVBlankCallback(IntrFunc callback)
{
    u16 imeBackup = REG_IME;

    REG_IME = 0;
    gVBlankCallback = callback;
    REG_IME = imeBackup;
}
