#include "global.h"
#include "libpmagb/save_block_pointers.h"

















u8 FlagSet(s32 flag)
{
    if (flag != 0)
    {
        u8 *flagPtr = &gFlagsPtr[flag / 8];

        if (flagPtr != NULL)
            *flagPtr |= 1 << (flag % 8);
    }

    return FALSE;
}

u8 UNUSED FlagClear(s32 flag)
{
    u8 *flagPtr;

    if (flag == 0)
        return FALSE;

    flagPtr = &gFlagsPtr[flag / 8];

    if (flagPtr != NULL)
        *flagPtr &= ~(1 << (flag % 8));

    return FALSE;
}

u32 FlagGet(s32 flag)
{
    u8 *flagPtr;

    if (flag == 0)
        return FALSE;

    flagPtr = &gFlagsPtr[flag / 8];

    if (flagPtr != NULL && *flagPtr & (1 << (flag % 8)))
        return TRUE;

    return FALSE;
}
