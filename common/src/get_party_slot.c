#include "global.h"
#include "libpmagb/save_block_pointers.h"
#include "pokemon.h"










struct Pokemon *GetPtrToEmptyPartySlot(void)
{
    if (*gPlayerPartyCountPtr == PARTY_SIZE)
        return NULL;
    else
        return &gPlayerPartyPtr[*gPlayerPartyCountPtr];
};
