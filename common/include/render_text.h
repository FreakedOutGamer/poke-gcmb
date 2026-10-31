#ifndef GUARD_RENDER_TEXT_H
#define GUARD_RENDER_TEXT_H

#include "gflib/text.h"

void RenderText(struct Window *win, const u8 *str);
void RenderText_NoPlaceholders(struct Window *win, const u8 *str);
void BufferString(u32 bufferId, const u8 *src);
s32 GetStringWidth(struct Window *win, const u8 *src);
u8 *NumToPmString3CustomZeroChar(s32 num, u8 *str, u32 zeroChar);
u8 *NumToPmString3RightAlign(u8 *str, s32 num);
void RenderTextAt(struct Window *win, u32 x, u32 y, const u8 *str);

#endif // GUARD_RENDER_TEXT_H
