#include "global.h"
#include <stdarg.h>
#include <stdio.h>

struct TextWindow
{
    bool32 is8bpp;
    void *tiles;
    u8 width;
    u8 height;
    u8 x;
    u8 y;
    u8 inkColor;
    u8 shadowColor;
    u8 paperColor;
    u8 glyphRowBytes;
    u8 glyphTiles;
    u8 queueTail;
    u8 queueHead;
    const u8 *fontIndex;
    const u8 *fontGlyphs;
    u8 queue[256];
};

struct Font
{
    u8 glyphRowBytes;
    u8 glyphTiles;
    const u8 *index;
    const u8 *glyphs;
};

struct TilemapFont
{
    u8 font;
    const u16 *chars;
    const u16 *tiles;
};

extern const u8 gFont0Index[];
extern const u8 gFont0Glyphs[];

const struct Font gFonts[] = {
    {2, 1, gFont0Index, gFont0Glyphs},
};

#include "data/text_tables.h"
#include "main.h"
#include "text.h"

const struct TilemapFont gTilemapFonts[] = {
    {0, sTilemapFontChars, sTilemapFontTiles},
};
extern u8 gCurrentTilemapFont;
extern char gPrintfBuffer[];


// Finds a Shift-JIS character's glyph. The lead byte picks a row of the
// index, whose trail-byte table (from 0x40) gives the glyph number, or
// 0xFF if the font lacks it (then the first glyph is used).
const u8 *GetGlyph(u16 code, const u8 *index, const u8 *glyphs, u8 glyphRowBytes)
{
    const u8 *glyph = glyphs;
    const u8 *row = (const u8 *)(((code & 0x7F00) >> 6) + (u32)index);
    u16 offset = (row[1] << 8) | row[0];
    u16 base = (row[3] << 8) | row[2];

    if (offset != 0xFFFF)
    {
        register u32 entry = offset + 0xFFC0;
        register const u8 *entryRow;
        entry += code & 0xFF;
        entryRow = (const u8 *)((u32)index + (u16)entry);
        if (*entryRow != 0xFF)
        {
            u16 stride;
            u16 number = base + *entryRow;
            stride = glyphRowBytes * 8;
            glyph += number * stride;
        }
    }
    return glyph;
}

// Expands one 8x8 tile of a 1bpp or 2bpp glyph into an 8bpp tile.
void ExpandGlyphTile8bpp(u32 *dest, const u32 *src, u8 bpp, u32 ink, u32 shadow, u32 paper)
{
    u32 colors[4];
    u32 bits;

    colors[0] = ink;
    colors[1] = paper;
    colors[2] = shadow;
    colors[3] = 0;
    if (bpp == 1)
    {
        bits = src[0];
        dest[1] = (colors[bits & 1] << 24) | (colors[(bits >> 1) & 1] << 16) | (colors[(bits >> 2) & 1] << 8) | colors[(bits >> 3) & 1];
        dest[0] = (colors[(bits >> 4) & 1] << 24) | (colors[(bits >> 5) & 1] << 16) | (colors[(bits >> 6) & 1] << 8) | colors[(bits >> 7) & 1];
        bits >>= 8;
        dest[3] = (colors[bits & 1] << 24) | (colors[(bits >> 1) & 1] << 16) | (colors[(bits >> 2) & 1] << 8) | colors[(bits >> 3) & 1];
        dest[2] = (colors[(bits >> 4) & 1] << 24) | (colors[(bits >> 5) & 1] << 16) | (colors[(bits >> 6) & 1] << 8) | colors[(bits >> 7) & 1];
        bits >>= 8;
        dest[5] = (colors[bits & 1] << 24) | (colors[(bits >> 1) & 1] << 16) | (colors[(bits >> 2) & 1] << 8) | colors[(bits >> 3) & 1];
        dest[4] = (colors[(bits >> 4) & 1] << 24) | (colors[(bits >> 5) & 1] << 16) | (colors[(bits >> 6) & 1] << 8) | colors[(bits >> 7) & 1];
        bits >>= 8;
        dest[7] = (colors[bits & 1] << 24) | (colors[(bits >> 1) & 1] << 16) | (colors[(bits >> 2) & 1] << 8) | colors[(bits >> 3) & 1];
        dest[6] = (colors[(bits >> 4) & 1] << 24) | (colors[(bits >> 5) & 1] << 16) | (colors[(bits >> 6) & 1] << 8) | colors[(bits >> 7) & 1];
        bits = src[1];
        dest[9] = (colors[bits & 1] << 24) | (colors[(bits >> 1) & 1] << 16) | (colors[(bits >> 2) & 1] << 8) | colors[(bits >> 3) & 1];
        dest[8] = (colors[(bits >> 4) & 1] << 24) | (colors[(bits >> 5) & 1] << 16) | (colors[(bits >> 6) & 1] << 8) | colors[(bits >> 7) & 1];
        bits >>= 8;
        dest[11] = (colors[bits & 1] << 24) | (colors[(bits >> 1) & 1] << 16) | (colors[(bits >> 2) & 1] << 8) | colors[(bits >> 3) & 1];
        dest[10] = (colors[(bits >> 4) & 1] << 24) | (colors[(bits >> 5) & 1] << 16) | (colors[(bits >> 6) & 1] << 8) | colors[(bits >> 7) & 1];
        bits >>= 8;
        dest[13] = (colors[bits & 1] << 24) | (colors[(bits >> 1) & 1] << 16) | (colors[(bits >> 2) & 1] << 8) | colors[(bits >> 3) & 1];
        dest[12] = (colors[(bits >> 4) & 1] << 24) | (colors[(bits >> 5) & 1] << 16) | (colors[(bits >> 6) & 1] << 8) | colors[(bits >> 7) & 1];
        bits >>= 8;
        dest[15] = (colors[bits & 1] << 24) | (colors[(bits >> 1) & 1] << 16) | (colors[(bits >> 2) & 1] << 8) | colors[(bits >> 3) & 1];
        dest[14] = (colors[(bits >> 4) & 1] << 24) | (colors[(bits >> 5) & 1] << 16) | (colors[(bits >> 6) & 1] << 8) | colors[(bits >> 7) & 1];
    }
    else
    {
        bits = src[0];
        dest[1] = (colors[bits & 3] << 24) | (colors[(bits >> 2) & 3] << 16) | (colors[(bits >> 4) & 3] << 8) | colors[(bits >> 6) & 3];
        dest[0] = (colors[(bits >> 8) & 3] << 24) | (colors[(bits >> 10) & 3] << 16) | (colors[(bits >> 12) & 3] << 8) | colors[(bits >> 14) & 3];
        bits >>= 16;
        dest[3] = (colors[bits & 3] << 24) | (colors[(bits >> 2) & 3] << 16) | (colors[(bits >> 4) & 3] << 8) | colors[(bits >> 6) & 3];
        dest[2] = (colors[(bits >> 8) & 3] << 24) | (colors[(bits >> 10) & 3] << 16) | (colors[(bits >> 12) & 3] << 8) | colors[(bits >> 14) & 3];
        bits = src[1];
        dest[5] = (colors[bits & 3] << 24) | (colors[(bits >> 2) & 3] << 16) | (colors[(bits >> 4) & 3] << 8) | colors[(bits >> 6) & 3];
        dest[4] = (colors[(bits >> 8) & 3] << 24) | (colors[(bits >> 10) & 3] << 16) | (colors[(bits >> 12) & 3] << 8) | colors[(bits >> 14) & 3];
        bits >>= 16;
        dest[7] = (colors[bits & 3] << 24) | (colors[(bits >> 2) & 3] << 16) | (colors[(bits >> 4) & 3] << 8) | colors[(bits >> 6) & 3];
        dest[6] = (colors[(bits >> 8) & 3] << 24) | (colors[(bits >> 10) & 3] << 16) | (colors[(bits >> 12) & 3] << 8) | colors[(bits >> 14) & 3];
        bits = src[2];
        dest[9] = (colors[bits & 3] << 24) | (colors[(bits >> 2) & 3] << 16) | (colors[(bits >> 4) & 3] << 8) | colors[(bits >> 6) & 3];
        dest[8] = (colors[(bits >> 8) & 3] << 24) | (colors[(bits >> 10) & 3] << 16) | (colors[(bits >> 12) & 3] << 8) | colors[(bits >> 14) & 3];
        bits >>= 16;
        dest[11] = (colors[bits & 3] << 24) | (colors[(bits >> 2) & 3] << 16) | (colors[(bits >> 4) & 3] << 8) | colors[(bits >> 6) & 3];
        dest[10] = (colors[(bits >> 8) & 3] << 24) | (colors[(bits >> 10) & 3] << 16) | (colors[(bits >> 12) & 3] << 8) | colors[(bits >> 14) & 3];
        bits = src[3];
        dest[13] = (colors[bits & 3] << 24) | (colors[(bits >> 2) & 3] << 16) | (colors[(bits >> 4) & 3] << 8) | colors[(bits >> 6) & 3];
        dest[12] = (colors[(bits >> 8) & 3] << 24) | (colors[(bits >> 10) & 3] << 16) | (colors[(bits >> 12) & 3] << 8) | colors[(bits >> 14) & 3];
        bits >>= 16;
        dest[15] = (colors[bits & 3] << 24) | (colors[(bits >> 2) & 3] << 16) | (colors[(bits >> 4) & 3] << 8) | colors[(bits >> 6) & 3];
        dest[14] = (colors[(bits >> 8) & 3] << 24) | (colors[(bits >> 10) & 3] << 16) | (colors[(bits >> 12) & 3] << 8) | colors[(bits >> 14) & 3];
    }
}

// Expands one 8x8 tile of a 1bpp or 2bpp glyph into a 4bpp tile.
void ExpandGlyphTile4bpp(u32 *dest, const u32 *src, u8 bpp, u32 ink, u32 shadow, u32 paper)
{
    u32 colors[4];
    u32 bits;

    colors[0] = paper;
    colors[1] = ink;
    colors[2] = shadow;
    colors[3] = 0;
    if (bpp == 1)
    {
        bits = src[0];
        dest[0] = (colors[bits & 1] << 28) | (colors[(bits >> 1) & 1] << 24) | (colors[(bits >> 2) & 1] << 20) | (colors[(bits >> 3) & 1] << 16) | (colors[(bits >> 4) & 1] << 12) | (colors[(bits >> 5) & 1] << 8) | (colors[(bits >> 6) & 1] << 4) | colors[(bits >> 7) & 1];
        bits >>= 8;
        dest[1] = (colors[bits & 1] << 28) | (colors[(bits >> 1) & 1] << 24) | (colors[(bits >> 2) & 1] << 20) | (colors[(bits >> 3) & 1] << 16) | (colors[(bits >> 4) & 1] << 12) | (colors[(bits >> 5) & 1] << 8) | (colors[(bits >> 6) & 1] << 4) | colors[(bits >> 7) & 1];
        bits >>= 8;
        dest[2] = (colors[bits & 1] << 28) | (colors[(bits >> 1) & 1] << 24) | (colors[(bits >> 2) & 1] << 20) | (colors[(bits >> 3) & 1] << 16) | (colors[(bits >> 4) & 1] << 12) | (colors[(bits >> 5) & 1] << 8) | (colors[(bits >> 6) & 1] << 4) | colors[(bits >> 7) & 1];
        bits >>= 8;
        dest[3] = (colors[bits & 1] << 28) | (colors[(bits >> 1) & 1] << 24) | (colors[(bits >> 2) & 1] << 20) | (colors[(bits >> 3) & 1] << 16) | (colors[(bits >> 4) & 1] << 12) | (colors[(bits >> 5) & 1] << 8) | (colors[(bits >> 6) & 1] << 4) | colors[(bits >> 7) & 1];
        bits = src[1];
        dest[4] = (colors[bits & 1] << 28) | (colors[(bits >> 1) & 1] << 24) | (colors[(bits >> 2) & 1] << 20) | (colors[(bits >> 3) & 1] << 16) | (colors[(bits >> 4) & 1] << 12) | (colors[(bits >> 5) & 1] << 8) | (colors[(bits >> 6) & 1] << 4) | colors[(bits >> 7) & 1];
        bits >>= 8;
        dest[5] = (colors[bits & 1] << 28) | (colors[(bits >> 1) & 1] << 24) | (colors[(bits >> 2) & 1] << 20) | (colors[(bits >> 3) & 1] << 16) | (colors[(bits >> 4) & 1] << 12) | (colors[(bits >> 5) & 1] << 8) | (colors[(bits >> 6) & 1] << 4) | colors[(bits >> 7) & 1];
        bits >>= 8;
        dest[6] = (colors[bits & 1] << 28) | (colors[(bits >> 1) & 1] << 24) | (colors[(bits >> 2) & 1] << 20) | (colors[(bits >> 3) & 1] << 16) | (colors[(bits >> 4) & 1] << 12) | (colors[(bits >> 5) & 1] << 8) | (colors[(bits >> 6) & 1] << 4) | colors[(bits >> 7) & 1];
        bits >>= 8;
        dest[7] = (colors[bits & 1] << 28) | (colors[(bits >> 1) & 1] << 24) | (colors[(bits >> 2) & 1] << 20) | (colors[(bits >> 3) & 1] << 16) | (colors[(bits >> 4) & 1] << 12) | (colors[(bits >> 5) & 1] << 8) | (colors[(bits >> 6) & 1] << 4) | colors[(bits >> 7) & 1];
    }
    else
    {
        bits = src[0];
        dest[0] = (colors[(bits >> 8) & 3] << 28) | (colors[(bits >> 10) & 3] << 24) | (colors[(bits >> 12) & 3] << 20) | (colors[(bits >> 14) & 3] << 16) | (colors[bits & 3] << 12) | (colors[(bits >> 2) & 3] << 8) | (colors[(bits >> 4) & 3] << 4) | colors[(bits >> 6) & 3];
        bits >>= 16;
        dest[1] = (colors[(bits >> 8) & 3] << 28) | (colors[(bits >> 10) & 3] << 24) | (colors[(bits >> 12) & 3] << 20) | (colors[(bits >> 14) & 3] << 16) | (colors[bits & 3] << 12) | (colors[(bits >> 2) & 3] << 8) | (colors[(bits >> 4) & 3] << 4) | colors[(bits >> 6) & 3];
        bits = src[1];
        dest[2] = (colors[(bits >> 8) & 3] << 28) | (colors[(bits >> 10) & 3] << 24) | (colors[(bits >> 12) & 3] << 20) | (colors[(bits >> 14) & 3] << 16) | (colors[bits & 3] << 12) | (colors[(bits >> 2) & 3] << 8) | (colors[(bits >> 4) & 3] << 4) | colors[(bits >> 6) & 3];
        bits >>= 16;
        dest[3] = (colors[(bits >> 8) & 3] << 28) | (colors[(bits >> 10) & 3] << 24) | (colors[(bits >> 12) & 3] << 20) | (colors[(bits >> 14) & 3] << 16) | (colors[bits & 3] << 12) | (colors[(bits >> 2) & 3] << 8) | (colors[(bits >> 4) & 3] << 4) | colors[(bits >> 6) & 3];
        bits = src[2];
        dest[4] = (colors[(bits >> 8) & 3] << 28) | (colors[(bits >> 10) & 3] << 24) | (colors[(bits >> 12) & 3] << 20) | (colors[(bits >> 14) & 3] << 16) | (colors[bits & 3] << 12) | (colors[(bits >> 2) & 3] << 8) | (colors[(bits >> 4) & 3] << 4) | colors[(bits >> 6) & 3];
        bits >>= 16;
        dest[5] = (colors[(bits >> 8) & 3] << 28) | (colors[(bits >> 10) & 3] << 24) | (colors[(bits >> 12) & 3] << 20) | (colors[(bits >> 14) & 3] << 16) | (colors[bits & 3] << 12) | (colors[(bits >> 2) & 3] << 8) | (colors[(bits >> 4) & 3] << 4) | colors[(bits >> 6) & 3];
        bits = src[3];
        dest[6] = (colors[(bits >> 8) & 3] << 28) | (colors[(bits >> 10) & 3] << 24) | (colors[(bits >> 12) & 3] << 20) | (colors[(bits >> 14) & 3] << 16) | (colors[bits & 3] << 12) | (colors[(bits >> 2) & 3] << 8) | (colors[(bits >> 4) & 3] << 4) | colors[(bits >> 6) & 3];
        bits >>= 16;
        dest[7] = (colors[(bits >> 8) & 3] << 28) | (colors[(bits >> 10) & 3] << 24) | (colors[(bits >> 12) & 3] << 20) | (colors[(bits >> 14) & 3] << 16) | (colors[bits & 3] << 12) | (colors[(bits >> 2) & 3] << 8) | (colors[(bits >> 4) & 3] << 4) | colors[(bits >> 6) & 3];
    }
}

void DrawWindowGlyph(struct TextWindow *window, u8 *dest, const u8 *glyph)
{
    u8 i = window->glyphTiles;

    if (window->is8bpp)
    {
        while (i-- != 0)
        {
            ExpandGlyphTile8bpp((u32 *)dest, (const u32 *)glyph, window->glyphRowBytes, window->inkColor, window->shadowColor, window->paperColor);
            dest += window->width * 64;
            glyph += window->glyphRowBytes * 8;
        }
    }
    else
    {
        while (i-- != 0)
        {
            ExpandGlyphTile4bpp((u32 *)dest, (const u32 *)glyph, window->glyphRowBytes, window->inkColor, window->shadowColor, window->paperColor);
            dest += window->width * 32;
            glyph += window->glyphRowBytes * 8;
        }
    }
}

void DrawGlyph(u8 *dest, u16 code, u8 font, u8 ink, u8 shadow, u8 paper, bool32 is8bpp)
{
    const u8 *glyph = GetGlyph(code, gFonts[font].index, gFonts[font].glyphs, gFonts[font].glyphRowBytes);
    u8 i = gFonts[font].glyphTiles;

    if (is8bpp)
    {
        while (i-- != 0)
        {
            ExpandGlyphTile8bpp((u32 *)dest, (const u32 *)glyph, gFonts[font].glyphRowBytes, ink, shadow, paper);
            dest += 64;
            glyph += gFonts[font].glyphRowBytes * 8;
        }
    }
    else
    {
        while (i-- != 0)
        {
            ExpandGlyphTile4bpp((u32 *)dest, (const u32 *)glyph, gFonts[font].glyphRowBytes, ink, shadow, paper);
            dest += 32;
            glyph += gFonts[font].glyphRowBytes * 8;
        }
    }
}

// There is only font 0; any other number gets it too.
void SetWindowFont(struct TextWindow *window, u8 font)
{
    if (font != 0)
        font = 0;
    window->glyphRowBytes = gFonts[font].glyphRowBytes;
    window->glyphTiles = gFonts[font].glyphTiles;
    window->fontIndex = gFonts[font].index;
    window->fontGlyphs = gFonts[font].glyphs;
}

void UNUSED InitTextWindow(struct TextWindow *window, void *tiles, bool32 is8bpp, u8 width, u8 height, u8 font)
{
    SetWindowFont(window, font);
    window->tiles = tiles;
    window->is8bpp = is8bpp;
    window->width = width;
    window->height = height;
    window->x = 0;
    window->y = 0;
    window->queueTail = 0;
    window->queueHead = 0;
    window->inkColor = 15;
    window->shadowColor = 0;
    window->paperColor = 0;
}

void UNUSED ClearWindowQueue(struct TextWindow *window)
{
    window->queueTail = 0;
    window->queueHead = 0;
}

bool32 IsWindowQueueEmpty(struct TextWindow *window)
{
    bool32 empty = FALSE;

    if (window->queueTail == window->queueHead)
        empty = TRUE;
    return empty;
}

void QueueWindowByte(struct TextWindow *window, u8 c)
{
    window->queue[window->queueTail++] = c;
}

void UNUSED QueueWindowChar(struct TextWindow *window, u16 c)
{
    window->queue[window->queueTail++] = (c & 0xFF00) >> 8;
    window->queue[window->queueTail++] = c;
}

u8 DequeueWindowByte(struct TextWindow *window)
{
    if (window->queueTail == window->queueHead)
        return 0;
    return window->queue[window->queueHead++];
}

// Takes the next character, one byte or a two-byte Shift-JIS one.
u16 DequeueWindowChar(struct TextWindow *window)
{
    u16 c = DequeueWindowByte(window);

    if (c != '\n')
    {
        c <<= 8;
        c |= DequeueWindowByte(window);
    }
    return c;
}

void SetWindowCursor(struct TextWindow *window, u8 x, u8 y)
{
    window->x = x % window->width;
    window->y = y % window->height;
}

void CopyWindowToVram(struct TextWindow *window, void *src);

void UNUSED SetWindowColors(struct TextWindow *window, u8 ink, u8 shadow, u8 paper)
{
    window->inkColor = ink;
    window->shadowColor = shadow;
    window->paperColor = paper;
}

void UNUSED ClearWindow(struct TextWindow *window, void *dest, bool32 copy)
{
    u16 fill;

    if (dest == NULL)
        dest = window->tiles;
    // Identical arms below preserve agbcc's allocation of the fill value.
    if (window->is8bpp)
    {
        register u16 color = window->paperColor;
        color |= color << 8;
        if (copy)
            fill = color;
        else
            fill = color;
        CpuSet(&fill, dest, CPU_SET_SRC_FIXED | (window->width * window->height * 32));
    }
    else
    {
        u16 paper = window->paperColor;
        u32 color;
        color = (paper << 4) | paper;
        color |= color << 8;
        if (copy)
            fill = color;
        else
            fill = color;
        CpuSet(&fill, dest, CPU_SET_SRC_FIXED | (window->width * window->height * 16));
    }
    SetWindowCursor(window, 0, 0);
    if (copy)
        CopyWindowToVram(window, dest);
}

void CopyWindowToVram(struct TextWindow *window, void *src)
{
    u32 tileSize;

    if (window->is8bpp)
        tileSize = 64;
    else
        tileSize = 32;
    RequestCopy(window->tiles, src, window->width * window->height * tileSize);
}

void PrintNextWindowChar(struct TextWindow *window, u8 *dest, bool32 copy)
{
    u16 c = DequeueWindowChar(window);

    if (c & 0xFF00)
    {
        u8 size = window->glyphTiles * (window->is8bpp ? 64 : 32);
        u32 offset = (window->y * window->width + window->x) * size;

        if (dest == NULL)
            dest = (u8 *)window->tiles + offset;
        DrawWindowGlyph(window, dest, GetGlyph(c, window->fontIndex, window->fontGlyphs, window->glyphRowBytes));
        if (copy)
            RequestCopy((u8 *)window->tiles + offset, dest, size);
        if (++window->x >= window->width)
        {
            window->x = 0;
            window->y += window->glyphTiles;
            if (window->y + (window->glyphTiles - 1) > window->height)
                window->y = 0;
        }
    }
    else if (c != 0)
    {
        if (c == '\n')
        {
            window->x = 0;
            window->y += window->glyphTiles;
            if (window->y + (window->glyphTiles - 1) > window->height)
                window->y = 0;
        }
    }
}

void QueueWindowString(struct TextWindow *window, const u8 *str)
{
    while (*str != 0)
        QueueWindowByte(window, *str++);
}

void UNUSED PrintWindowChar(struct TextWindow *window, u8 *dest, u16 c, bool32 copy)
{
    if (c & 0xFF00)
    {
        u8 size = window->glyphTiles * (window->is8bpp ? 64 : 32);
        u32 offset = (window->y * window->width + window->x) * size;
        const u8 *glyph = GetGlyph(c, window->fontIndex, window->fontGlyphs, window->glyphRowBytes);

        if (dest == NULL)
            dest = (u8 *)window->tiles + offset;
        DrawWindowGlyph(window, dest, glyph);
        if (copy)
            RequestCopy((u8 *)window->tiles + offset, dest, size);
        if (++window->x >= window->width)
        {
            window->x = 0;
            window->y += window->glyphTiles;
            if (window->y + (window->glyphTiles - 1) > window->height)
                window->y = 0;
        }
    }
    else if (c != 0)
    {
        if (c == '\n')
        {
            window->x = 0;
            window->y += window->glyphTiles;
            if (window->y + (window->glyphTiles - 1) > window->height)
                window->y = 0;
        }
    }
}

void UNUSED PrintWindowString(struct TextWindow *window, u8 *dest, const u8 *str, bool32 copy)
{
    QueueWindowString(window, str);
    while (!IsWindowQueueEmpty(window))
        PrintNextWindowChar(window, dest, FALSE);
    if (copy)
        CopyWindowToVram(window, dest);
}

// Puts one character on a 32x32 tilemap at (*x, *y), with the tiles
// LoadTilemapFont loaded, and advances *x.
void PutTilemapChar(u16 *tilemap, u8 *x, u8 *y, u16 attr, u16 c)
{
    u8 *font = &gCurrentTilemapFont;
    const struct TilemapFont *fonts = gTilemapFonts;
    u16 tile;

    *x &= 31;
    *y &= 31;
    if (c < 0x80)
        c = gAsciiToSjis[c];
    tile = 0;
    if (c >= 0x8100 && c < 0x8400)
    {
        u32 offset = *font * sizeof(*fonts);
        register const u16 *const *table = &fonts[0].tiles;
        tile = (*(const u16 **)(offset + (u32)table))[c - 0x8100];
    }
    tilemap += *y * 32 + *x;
    *tilemap = (attr & 0xFC00) | ((tile + (attr & 0x3FF)) & 0x3FF);
    (*x)++;
}

void UNUSED ResetTilemapFont(void)
{
    gCurrentTilemapFont = 0;
}

// Draws a tilemap font's characters into consecutive 4bpp tiles.
void LoadTilemapFont(u8 *dest, u8 tilemapFont, u8 ink, u8 shadow)
{
    const u16 *chars;

    gCurrentTilemapFont = tilemapFont;
    for (chars = gTilemapFonts[tilemapFont].chars; *chars != 0; chars++)
    {
        DrawGlyph(dest, *chars, gTilemapFonts[tilemapFont].font, ink, shadow, 0, FALSE);
        dest += 32;
    }
}

void UNUSED PutTilemapCharAt(u16 *tilemap, u8 x, u8 y, u16 attr, u16 c)
{
    PutTilemapChar(tilemap, &x, &y, attr, c);
}

// Puts a string on a tilemap; "\\n" or a newline starts a new line.
void UNUSED PutTilemapString(u16 *tilemap, u8 x, u8 y, u16 attr, const u8 *str)
{
    u8 curX = x;
    u8 curY = y;
    u16 c;

    while (*str != 0)
    {
        switch (*str)
        {
        default:
            c = *str++;
            if (c & 0x80)
            {
                c <<= 8;
                c |= *str++;
            }
            break;
        case '\\':
            // A backslash followed by n also starts a new line; a backslash
            // before anything else is dropped.
            str++;
            if (*str == 'n')
            {
                curX = x;
                curY++;
                str++;
            }
            continue;
        case '\n':
            curX = x;
            curY++;
            str++;
            continue;
        }
        PutTilemapChar(tilemap, &curX, &curY, attr, c);
    }
}

void PrintfTilemap(u16 *tilemap, u8 x, u8 y, u16 attr, const char *fmt, ...)
{
    u8 curX = x;
    u8 curY = y;
    va_list args;
    const u8 *str;
    u32 c;

    va_start(args, fmt);
    vsprintf(gPrintfBuffer, fmt, args);
    str = (const u8 *)gPrintfBuffer;
    while (*str != 0)
    {
        switch (*str)
        {
        default:
            c = *str++;
            if (c & 0x80)
            {
                c <<= 8;
                c |= *str++;
            }
            break;
        case '\\':
            str++;
            if (*str != 'n')
                continue;
            // fallthrough
        case '\n':
            curX = x;
            curY++;
            str++;
            continue;
        }
        PutTilemapChar(tilemap, &curX, &curY, attr, c);
    }
}
