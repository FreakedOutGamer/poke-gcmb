#include "global.h"
#include "libpmagb/save_block_pointers.h"
#include "string_util.h"



























void CopyMoveName(u8 *dst, u32 move)
{
    s32 i;
    const u8 (*moveNames)[][(GAME_LANGUAGE == LANGUAGE_JAPANESE ? 7 : MOVE_NAME_LENGTH) + 1] = (const u8 (*)[][(GAME_LANGUAGE == LANGUAGE_JAPANESE ? 7 : MOVE_NAME_LENGTH) + 1])gAgbPmRomParams->moveNames;

    for (i = 0; i < gAgbPmRomParams->moveNameLength + gAgbPmRomParams->endOfStringLength; i++)
    {
        dst[i] = (*moveNames)[move][i];
        if (dst[i] == EOS)
            break;
    }

    dst[i] = EOS;
}
