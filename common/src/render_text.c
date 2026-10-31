#include "global.h"
#include "gflib.h"
#include "string_util.h"
#include "gflib/text.h"
#include "colbtl_version.h"
#include "render_text.h"

extern u8 gStringBuffers[][32];
extern const u8 gFont1JapanGfx[];
extern const u8 gFont0JapanGfx[];
extern const u8 gFont1JapanWidths[];
extern const u8 gFont0JapanWidths[];

// This file's functions.
void RenderText(struct Window *win, const u8 *str)
{
#if GAME_LANGUAGE != LANGUAGE_JAPANESE
    const u8 *currStr;
    const u8 *glyphGfx = win->glyphGfx;
    const u8 *glyphWidths = win->glyphWidths;
#endif

    while (*str != EOS)
    {
        switch (*str)
        {
        case CHAR_NEWLINE:
            TextWindowNextLine(win);
            break;
        case PLACEHOLDER_BEGIN:
            str++;
            RenderText(win, gStringBuffers[*str]);
            break;
        case CHAR_PROMPT_CLEAR:
            do
            {
                DelayFrames(1);
            } while (!(gNewKeys & (A_BUTTON | B_BUTTON)));
            break;
#if GAME_LANGUAGE != LANGUAGE_JAPANESE
        case EXT_CTRL_CODE_BEGIN:
            str++;
            currStr = str;
            switch (*str)
            {
            case 0x14:
                win->glyphWidth = str[1];
                currStr = str + 2;
                break;
            case 0x15:
                if (win->glyphSize == 8)
                {
                    win->glyphGfx = gFont1JapanGfx;
                    win->glyphWidths = gFont1JapanWidths;
                }
                else
                {
                    win->glyphGfx = gFont0JapanGfx;
                    win->glyphWidths = gFont0JapanWidths;
                }
                break;
#if !COLBTL_IS(COLBTL)
            case 0xF:
                ClearWindowCharBuffer(win, 0);
                break;
#endif
            }
            str = currStr;
            break;
#endif
        default:
            if (win->shadowColor == 0)
                DrawGlyphOnWindow_NoShadow(win, *str, win->fgColor);
            else
                DrawGlyphOnWindow_WithShadow(win, *str, win->fgColor, win->shadowColor);
            break;
        }
        str++;
    }

#if GAME_LANGUAGE != LANGUAGE_JAPANESE
    win->glyphWidth = 0;
    win->glyphGfx = glyphGfx;
    win->glyphWidths = glyphWidths;
#endif
}

void RenderText_NoPlaceholders(struct Window *win, const u8 *str)
{
#if GAME_LANGUAGE != LANGUAGE_JAPANESE
    const u8 *currStr;
    const u8 *glyphGfx = win->glyphGfx;
    const u8 *glyphWidths = win->glyphWidths;
#endif

    while (*str != EOS)
    {
        switch (*str)
        {
        case CHAR_NEWLINE:
            TextWindowNextLine(win);
            break;
        case CHAR_EXTRA_SYMBOL:
            if (COLBTL_IS(COLBTL) || (GAME_LANGUAGE != LANGUAGE_JAPANESE && win->glyphWidths != gFont1JapanWidths))
                TextWindowShiftXY(win, 5, 0);
            else
                TextWindowShiftXY(win, 8, 0);
            break;
#if GAME_LANGUAGE != LANGUAGE_JAPANESE
        case EXT_CTRL_CODE_BEGIN:
            str++;
            currStr = str;
            switch (*str)
            {
            case 0x14:
                win->glyphWidth = str[1];
                currStr = str + 2;
                break;
            case 0x15:
                if (win->glyphSize == 8)
                {
                    win->glyphGfx = gFont1JapanGfx;
                    win->glyphWidths = gFont1JapanWidths;
                }
                else
                {
                    win->glyphGfx = gFont0JapanGfx;
                    win->glyphWidths = gFont0JapanWidths;
                }
                break;
#if !COLBTL_IS(COLBTL)
            case 0xF:
                ClearWindowCharBuffer(win, 0);
                break;
#endif
            }
            str = currStr;
            break;
#endif
        default:
            DrawGlyphOnWindow_WithShadow(win, *str, win->fgColor, win->shadowColor);
            break;
        }
        str++;
    }

#if GAME_LANGUAGE != LANGUAGE_JAPANESE
    win->glyphGfx = glyphGfx;
    win->glyphWidths = glyphWidths;
#endif
}

void BufferString(u32 bufferId, const u8 *src)
{
    StringCopy(gStringBuffers[bufferId], src);
}

s32 GetStringWidth(struct Window *win, const u8 *src)
{
    s32 width = 0;
    while (*src != EOS)
    {
        width += win->glyphWidths[*src];
        src++;
    }
    return width;
}

u8 *NumToPmString3CustomZeroChar(s32 num, u8 *str, u32 zeroChar)
{
    str[0] = Div(num, 100) + zeroChar;
    num = Mod(num, 100);
    str[1] = Div(num, 10) + zeroChar;
    str[2] = Mod(num, 10) + zeroChar;
    str[3] = EOS;
    return str;
}

u8 *NumToPmString3RightAlign(u8 *str, s32 num)
{
    s32 i;

    str[0] = Div(num, 100) + (unsigned) CHAR_0;
    num = Mod(num, 100);
    str[1] = Div(num, 10) + (unsigned) CHAR_0;
    str[2] = Mod(num, 10) + (unsigned) CHAR_0;
    str[3] = EOS;
    for (i = 0; str[i + 1] != EOS && str[i] == CHAR_0; i++)
        str[i] = CHAR_SPACE;
    return str;
}

void RenderTextAt(struct Window *win, u32 x, u32 y, const u8 *str)
{
    TextWindowSetXY(win, x, y);
    RenderText(win, str);
}
