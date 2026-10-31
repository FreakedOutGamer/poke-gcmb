#ifndef GFLIB_BG_H
#define GFLIB_BG_H

#include "constants/global.h"

// colbtl's Western releases keep the tilemap buffers in IWRAM at a fixed
// address; every other program places gBgTilemapBuffers itself.
#if defined(COLBTL_VERSION) && GAME_LANGUAGE != LANGUAGE_JAPANESE
#define gBGTilemapBuffers ((void *)0x03000000)
#else
extern u8 gBgTilemapBuffers[];
#define gBGTilemapBuffers ((void *)gBgTilemapBuffers)
#endif

// Each background gets 0x1000 bytes of tilemap buffer, which is room for two
// 32x32 screens (BG_SCREEN_SIZE each), as a 512x256 or 256x512 map needs.
#define BG_TILEMAP_BUFFER(bg) ((u8 *)gBGTilemapBuffers + ((bg) << 12))

extern u16 gBgHofsBuffer[];
extern u16 gBgVofsBuffer[];
extern bool8 gBgTilemapBufferTransferScheduled[];

void ResetGpuBuffers(void);
void DoGpuUpdateAndTilemapTransfers(void);
void CopyToBgTilemapBufferRect(u32 bgNum, u32 left, u32 top, s32 width, u32 height, const u16 * src);
void UNUSED CopyFromBgTilemapBufferRect(u32 bgNum, u32 left, u32 top, s32 width, u32 height, u16 * dest);
void SetBgTilemapBufferPaletteRect(u32 bgNum, u32 left, u32 top, s32 width, u32 height, u32 paletteNum);
void SetBgTilemapBufferTileAt(u32 bgNum, u32 x, u32 y, u16 tileNum);
void FillBgTilemapBufferRect(u32 bgNum, u32 left, u32 top, s32 width, u32 height, u16 tileNum);
void CopyRectWithinBgTilemapBuffer(u32 bgNum, u32 srcLeft, u32 srcTop, s32 width, u32 height, u32 destLeft, u32 destTop);
void SetBgPos(u32 bgNum, s32 x, s32 y);

#endif //GFLIB_BG_H
