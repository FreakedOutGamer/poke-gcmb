#include "global.h"
#include "libpmagb/save_block_pointers.h"
#include "pokemon.h"
#include "target_game.h"


u32 UNUSED GetEnigmaBerryChecksum(u8 *enigmaBerry)
{
    void *description1;
    void *description2;
    u32 i;
    u32 checksum;
    u8 *dest;

    {
        u8 *base = (u8 *)gEnigmaBerryPtr;

        description1 = *(void **)(base + 12);
        description2 = *(void **)(base + 16);
        *(void **)(base + 12) = NULL;
        *(void **)(base + 16) = NULL;
    }

    dest = enigmaBerry;
    checksum = 0;

    for (i = 0; i < gAgbPmRomParams->enigmaBerrySize - 4; i++)
        checksum += dest[i];

    {
        u8 *base = (u8 *)gEnigmaBerryPtr;

        *(void **)(base + 12) = description1;
        *(void **)(base + 16) = description2;
    }

    return checksum;
}

bool8 UNUSED IsEnigmaBerryValid(void)
{
    if (((u8 *)gEnigmaBerryPtr)[20] == 0)
        return FALSE;
    if (((u8 *)gEnigmaBerryPtr)[10] == 0)
        return FALSE;
    return TRUE;
}
