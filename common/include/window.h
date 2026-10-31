#ifndef GUARD_WINDOW_H
#define GUARD_WINDOW_H

#include "gflib/text.h"

void DrawTextWindowBorder(u32 x, u32 y, s32 width, s32 height, u32 baseTile);
void ClearVram(void);
struct Window *CreateTextWindow(u32 winId, u32 left, u32 top, u32 width, u32 height, u32 glyphSize);

#endif // GUARD_WINDOW_H
