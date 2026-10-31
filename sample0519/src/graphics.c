#include "global.h"

// Font 0 (see gFonts in text.c). The index holds, for each Shift-JIS lead
// byte, the offset of its trail-byte table and its first glyph, followed by
// the tables. The glyphs are one 2bpp 8x8 tile each.
const u8 gFont0Index[] = INCBIN_U8("graphics/font/index.bin");
const u8 gFont0Glyphs[] = INCBIN_U8("graphics/font/glyphs.bin");

// The delivery screen's frame, on BG1.
const u8 gFrameGfx[] = INCBIN_U8("graphics/frame/tiles.4bpp");
const u16 gFrameTilemap[] = INCBIN_U16("graphics/frame/tilemap.bin");
