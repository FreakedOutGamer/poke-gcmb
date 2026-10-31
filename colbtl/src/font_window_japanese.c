#include "global.h"
#include "gflib/text.h"

extern const u8 gFont0JapanGfx[];
extern const u8 gFont0JapanWidths[];

// The historical Latin name is shared with the Western error-screen code.
const struct Window gFont0LatinInfo =
{
    .bg = 2,
    .left = 4,
    .top = 15,
    .width = 22,
    .height = 4,
    .baseBlock = 0x20,
    .fillValue = 15,
    .glyphSize = 16,
    .vramCharBase = (void *)(BG_VRAM + 0x8400),
    .glyphGfx = gFont0JapanGfx,
    .glyphWidths = gFont0JapanWidths,
};
