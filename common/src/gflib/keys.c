#include "global.h"
#include "gflib/keys.h"

// An image that places this bss itself builds with SDK_EXTERN_BSS.
#ifdef SDK_EXTERN_BSS
extern u16 gNewKeys;
extern u16 gNewAndRepeatedKeys;
extern u16 gHeldKeys;

extern u16 sKeyRepeatDelay;
extern u16 sKeyRepeatRate;
extern u16 sKeyRepeatTimer;
#else
u16 gNewKeys;
u16 gNewAndRepeatedKeys;
u16 gHeldKeys;

static u16 sKeyRepeatDelay;
static u16 sKeyRepeatRate;
static u16 sKeyRepeatTimer;
#endif

void ReadKeys(void)
{
    u32 keyInput;
    u16 * prevKeys = &gHeldKeys;
    u16 newKeys = (keyInput = REG_KEYINPUT ^ KEYS_MASK) & ~*prevKeys;
    gNewKeys = newKeys;

    if (gHeldKeys != keyInput)
    {
        gNewAndRepeatedKeys = keyInput;
        sKeyRepeatTimer = sKeyRepeatDelay;
    }
    else if (--sKeyRepeatTimer == 0)
    {
        gNewAndRepeatedKeys = keyInput;
        sKeyRepeatTimer = sKeyRepeatRate;
    }
    else
    {
        gNewAndRepeatedKeys = 0;
    }

    gHeldKeys = keyInput;
}

void SetKeyRepeatTiming(u16 delay, u16 rate)
{
    sKeyRepeatDelay = delay;
    sKeyRepeatRate = rate;
    sKeyRepeatTimer = 1;
}
