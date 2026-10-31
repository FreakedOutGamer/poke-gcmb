#include "global.h"

// The splash screen, a 240x160 mode-3 bitmap (see graphics/splash/), which
// the program decompresses straight to VRAM.
const u32 gSplashGfx[] = INCBIN_U32("build/graphics/splash.bin.lz");

// The receive scene's OBJ palettes: 0 for its window frame, 1 for the
// picture the GameCube sends to show inside it.
const u16 gReceiveSceneObjPalette[][16] =
{
    INCBIN_U16("graphics/receive_scene/frame.gbapal"),
    INCBIN_U16("graphics/receive_scene/picture.gbapal"),
};

// The window frame's tiles.

const u32 gReceiveSceneObjTiles[] = INCBIN_U32("graphics/receive_scene/frame.4bpp.lz");
