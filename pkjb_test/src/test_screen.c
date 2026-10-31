#include "global.h"

extern const u8 gTestScreenGfx[];

void SoftResetRom(u32 resetFlags);

// The package is unpacked in two steps through EWRAM: Huffman, then LZ77.
#define HUFF_BUFFER ((void *)(EWRAM + 0x20000))
#define GFX_BUFFER  ((u8 *)(EWRAM + 0x10000))

// Unpacked, the package is the palette, then the tilemap, then the tiles.
#define GFX_TILEMAP (GFX_BUFFER + 0x20)
#define GFX_TILES   (GFX_BUFFER + 0x820)

// Also inlined into RunTestScreen (the file is built with -O3).
void UNUSED LoadTestScreen(void)
{
    void *huff;
    void *gfx;

    REG_DISPCNT = DISPCNT_BG0_ON;
    REG_BG0CNT = BGCNT_SCREENBASE(16);
    HuffUnComp(gTestScreenGfx, huff = HUFF_BUFFER);
    LZ77UnCompWram(huff, gfx = GFX_BUFFER);
    CpuSet(gfx, (void *)BG_PLTT, 16); // the palette
    CpuSet(GFX_TILEMAP, BG_SCREEN_ADDR(16), 0x400);
    CpuSet(GFX_TILES, BG_CHAR_ADDR(0), 0x4000);
}

// Shows the "download successful" screen until A resets the GBA.
void RunTestScreen(void)
{
    LoadTestScreen();
    for (;;)
    {
        VBlankIntrWait();
        if ((u16)~REG_KEYINPUT & A_BUTTON)
            SoftResetRom(RESET_SIO_REGS | RESET_SOUND_REGS | RESET_REGS);
    }
}
