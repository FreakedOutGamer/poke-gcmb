#include "global.h"
#include "gflib/text.h"
#include "constants/berry.h"
#include "constants/items.h"
#include "constants/maps.h"
#include "constants/pokemon.h"

#include "data/berries.h"

#include "data/species_to_national.h"

ALIGNED(4) const u8 gExtCtrlCodeLengths[] = {
    [EXT_CTRL_CODE_COLOR - 1]                  = 1,
    [EXT_CTRL_CODE_HIGHLIGHT - 1]              = 2,
    [EXT_CTRL_CODE_SHADOW - 1]                 = 2,
    [EXT_CTRL_CODE_COLOR_HIGHLIGHT_SHADOW - 1] = 2,
    [EXT_CTRL_CODE_PALETTE - 1]                = 4,
    [EXT_CTRL_CODE_FONT - 1]                   = 2,
    [EXT_CTRL_CODE_RESET_SIZE - 1]             = 2,
    [EXT_CTRL_CODE_PAUSE - 1]                  = 1,
    [EXT_CTRL_CODE_PAUSE_UNTIL_PRESS - 1]      = 2,
    [EXT_CTRL_CODE_WAIT_SE - 1]                = 1,
    [EXT_CTRL_CODE_PLAY_BGM - 1]               = 1,
    [EXT_CTRL_CODE_ESCAPE - 1]                 = 3,
    [EXT_CTRL_CODE_SHIFT_TEXT - 1]             = 2,
    [EXT_CTRL_CODE_SHIFT_DOWN - 1]             = 2,
    [EXT_CTRL_CODE_FILL_WINDOW - 1]            = 2,
    [EXT_CTRL_CODE_PLAY_SE - 1]                = 1,
    [EXT_CTRL_CODE_CLEAR - 1]                  = 3,
    [EXT_CTRL_CODE_SKIP - 1]                   = 2,
    [EXT_CTRL_CODE_CLEAR_TO - 1]               = 2,
    [EXT_CTRL_CODE_MIN_LETTER_SPACING - 1]     = 2,
    [EXT_CTRL_CODE_JPN - 1]                    = 2,
    [EXT_CTRL_CODE_ENG - 1]                    = 1,
    [EXT_CTRL_CODE_PAUSE_MUSIC - 1]            = 1,
    [EXT_CTRL_CODE_RESUME_MUSIC - 1]           = 0,
};
