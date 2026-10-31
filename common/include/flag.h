#ifndef GUARD_FLAG_H
#define GUARD_FLAG_H

#include "global.h"
#include "libpmagb/save_block_pointers.h"

// Pointer to the connected cartridge's flags array, precomputed once from
// SaveBlock1 + the descriptor's sb1FlagsOffset.

u8 FlagSet(s32 id);
u8 UNUSED FlagClear(s32 id);
u32 FlagGet(s32 id);

#endif // GUARD_FLAG_H
