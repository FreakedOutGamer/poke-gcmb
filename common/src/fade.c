#include "global.h"
#include "gflib.h"
#include "fade.h"

extern u8 gScreenIsFadedOut;

bool32 IsScreenFadedOut(void)
{
    return gScreenIsFadedOut;
}

void OverrideScreenFadeState(bool32 faded)
{
    gScreenIsFadedOut = faded;
}

void FadeIn(void)
{
    s32 i;

    DelayFrames(1);
    REG_BLDY = 0x1F;
    REG_BLDCNT = 0xFF;
    for (i = 29; i > 0; i -= 2)
    {
        DelayFrames(1);
        REG_BLDY = i;
    }
    REG_BLDCNT = 0;
    gScreenIsFadedOut = FALSE;
}

void FadeOut(void)
{
    s32 i;

    gScreenIsFadedOut = TRUE;
    DelayFrames(1);
    REG_BLDY = 0;
    REG_BLDCNT = 0xFF;
    for (i = 1; i <= 31; i += 2)
    {
        DelayFrames(1);
        REG_BLDY = i;
    }
    REG_BLDY = 0x1F;
    ResetSprites();
}
