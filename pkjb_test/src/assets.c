#include "global.h"
#include "gflib/intr.h"

typedef void (*IntrFunc)(void);

// Unused: 0x100 zero bytes the image carries before the interrupt table.
u8 gReservedEwram[0x100] = {0};

IntrFunc gIntrTable[] =
{
    VBlankIntr,
    IntrDummy,
    IntrDummy,
    IntrDummy,
    IntrDummy,
    IntrDummy,
    IntrDummy,
    IntrDummy,
    IntrDummy,
    IntrDummy,
    IntrDummy,
    IntrDummy,
    IntrDummy,
    IntrDummy,
};

// "ダウンロードに成功しました / Aボタンを押してリセットして下さい。"
// ("Download successful. Press the A button to reset.")
// The palette, tilemap and tiles, LZ77-compressed and then Huffman-coded.
const u8 gTestScreenGfx[] = INCBIN_U8("build/graphics/test_screen.huff");
