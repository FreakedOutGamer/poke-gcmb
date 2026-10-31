#include "global.h"

// The background graphics, which are the lines framing the message box (the
// later revision moves the top line up for its taller box), the font, and
// their palettes.
#if REVISION == 0
const u16 gBgTilemap[] = INCBIN_U16("graphics/background/tilemap_rev0.bin");
#else
const u16 gBgTilemap[] = INCBIN_U16("graphics/background/tilemap_rev1.bin");
#endif

// Unused.
const u16 gUnknown_02014F00[] = {0x0030, 0x0040, 0x1041, 0x0000};

const u16 gBgPalettes[] = INCBIN_U16("graphics/background/palettes.gbapal");
const u32 gBgTiles[] = INCBIN_U32("graphics/background/tiles.4bpp");
