#include "global.h"
#include "gflib/bg.h"
#include "gflib/text.h"
#include "window.h"

extern u16 gNextWindowBaseBlock;
extern const u8 gFont0LatinGfx[];
extern const u8 gFont1LatinGfx[];
extern const u8 gFont0LatinWidths[];
extern const u8 gFont1LatinWidths[];

void DrawTextWindowBorder(u32 x, u32 y, s32 width, s32 height, u32 baseTile)
{
    s32 row, i;
    u16 *tile;

    tile = ((u16 *)BG_TILEMAP_BUFFER(2) + y * 32 + x);
    *tile = baseTile;

    for (i = 1; i < width - 1; i++)
        tile[i] = baseTile + 1;
    tile[i] = baseTile + 2;

    tile += 0x20;
    for (row = 1; row < height - 1; row++, tile += 0x20)
    {
        *tile = baseTile + 3;
        // Skips the interior (baseTile + 4) without drawing it, so whatever
        // is already behind the window shows through.
        for (i = 1; i < width - 1; i++)
            ;
        tile[i] = baseTile + 5;
    }

    *tile = baseTile + 6;
    for (i = 1; i < width - 1; i++)
        tile[i] = baseTile + 7;
    tile[i] = baseTile + 8;
}

void ClearVram(void)
{
    CpuFill32(0, (void *)VRAM, VRAM_SIZE);
    ResetGpuBuffers();
}

struct Window *CreateTextWindow(u32 winId, u32 left, u32 top, u32 width, u32 height, u32 glyphSize)
{
    struct Window winTemplate;
    struct Window *winCreated;

    winTemplate.bg = 0;
    winTemplate.left = left;
    winTemplate.top = top;
    winTemplate.width = width;
    winTemplate.height = height;
    winTemplate.baseBlock = gNextWindowBaseBlock;
    winTemplate.fillValue = 0xF;
    winTemplate.glyphSize = glyphSize;
    winTemplate.vramCharBase = (void *) BG_VRAM + BG_CHAR_SIZE + (winTemplate.baseBlock * 32);
    if (glyphSize == 8)
    {
        winTemplate.glyphGfx = gFont1LatinGfx;
        winTemplate.glyphWidths = gFont1LatinWidths;
    }
    else
    {
        winTemplate.glyphGfx = gFont0LatinGfx;
        winTemplate.glyphWidths = gFont0LatinWidths;
    }

    winCreated = AddWindow(winId, &winTemplate);
    SetTextColor(winCreated, 15, 1);
    ClearWindowCharBuffer(winCreated, 0);
    gNextWindowBaseBlock += width * height;
    return winCreated;
}
