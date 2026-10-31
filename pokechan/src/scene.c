#include "global.h"
#include "gba/gba.h"
#include "gflib/sprite.h"
#include "gflib/bg.h"
#include "gflib/decompress.h"
#include "scene.h"

extern const u32 gSplashGfx[];
extern const u32 gReceiveSceneObjTiles[];
extern const u16 gReceiveSceneObjPalette[][16];
extern struct Sprite *gReceiveSceneSprite1;
extern struct Sprite *gReceiveSceneSprite2;

// The window frame, from gReceiveSceneObjTiles (OBJ tiles 0x280 on).
const struct Subsprite gReceiveSceneSubsprites1[] =
{
    {0x40004000, 0, 0, 0x280},
    {0x40004000, 32, 0, 0x284},
    {0x40004000, 64, 0, 0x284},
    {0x40004000, 96, 0, 0x284},
    {0x40004000, 128, 0, 0x284},
    {0x40004000, 160, 0, 0x284},
    {0x40004000, 192, 0, 0x284},
    {0x10004000, 224, 0, 0x280},
    {0x60004000, 0, 40, 0x280},
    {0x60004000, 32, 40, 0x284},
    {0x60004000, 64, 40, 0x284},
    {0x60004000, 96, 40, 0x284},
    {0x60004000, 128, 40, 0x284},
    {0x60004000, 160, 40, 0x284},
    {0x60004000, 192, 40, 0x284},
    {0x30004000, 224, 40, 0x280},
    {0x40008000, 0, 8, 0x288},
    {0x50008000, 232, 8, 0x288},
    {0x0000FFFF, 0, 0, 0x0},
};

// The picture inside it, in OBJ palette 1, from the tiles
// UnpackReceivedGraphics copies in (OBJ tiles 0x200 on).
const struct Subsprite gReceiveSceneSubsprites2[] =
{
    {0x80000000, 8, 8, 0x200},
    {0x80000000, 40, 8, 0x210},
    {0x80000000, 72, 8, 0x220},
    {0x80000000, 104, 8, 0x230},
    {0x80000000, 136, 8, 0x240},
    {0x80000000, 168, 8, 0x250},
    {0x80000000, 200, 8, 0x260},
    {0x0000FFFF, 0, 0, 0x0},
};

void InitReceiveScene(void)
{
    ResetGpuBuffers();

    CpuFill32(0, (void *)VRAM, 0x18000);

    REG_BLDY = 31;
    REG_BLDCNT = BLDCNT_TGT1_ALL | BLDCNT_EFFECT_DARKEN;
    REG_DISPCNT = DISPCNT_MODE_3 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
    REG_BG2CNT = 0;

    AutoUnCompVram(gSplashGfx, (void *)VRAM);

    CpuCopy16(gReceiveSceneObjPalette, (void *)OBJ_PLTT, 64);

    AutoUnCompVram(gReceiveSceneObjTiles, (void *)0x06015000);

    gReceiveSceneSprite1 = AddSprite(0, 112, gReceiveSceneSubsprites1);
    gReceiveSceneSprite2 = AddSprite(0, 112, gReceiveSceneSubsprites2);

    SetSpritePaletteNum(gReceiveSceneSprite2, 1);
    SetSpriteInvisible(gReceiveSceneSprite1, 1);
    SetSpriteInvisible(gReceiveSceneSprite2, 1);
}

// Unpacks the two sprites' graphics from a buffer the GameCube filled (gJoyBusState.recvFlags & 3 == 2's receive window), then
// hides both sprites again.
void UnpackReceivedGraphics(const void *src)
{
    vu32 *dmaRegs = (vu32 *)REG_ADDR_DMA3;
    u32 ctrl = (DMA_ENABLE << 16) | 0x40;
    s32 i;
    void *d3 = (void *)0x06014180;
    const u8 *s3 = (const u8 *)src + 0xA80;
    void *d2 = (void *)0x06014100;
    const u8 *s2 = (const u8 *)src + 0x700;
    void *d1 = (void *)0x06014080;
    const u8 *s1 = (const u8 *)src + 0x380;
    void *d0 = (void *)0x06014000;
    const u8 *s0 = src;
    u32 stride = 0x200;

    for (i = 6; i >= 0; i--)
    {
        dmaRegs[0] = (vu32)s0; dmaRegs[1] = (vu32)d0; dmaRegs[2] = ctrl; dmaRegs[2];
        dmaRegs[0] = (vu32)s1; dmaRegs[1] = (vu32)d1; dmaRegs[2] = ctrl; dmaRegs[2];
        dmaRegs[0] = (vu32)s2; dmaRegs[1] = (vu32)d2; dmaRegs[2] = ctrl; dmaRegs[2];
        dmaRegs[0] = (vu32)s3; dmaRegs[1] = (vu32)d3; dmaRegs[2] = ctrl; dmaRegs[2];

        d3 = (u8 *)d3 + stride;
        s3 += 0x80;
        d2 = (u8 *)d2 + stride;
        s2 += 0x80;
        d1 = (u8 *)d1 + stride;
        s1 += 0x80;
        d0 = (u8 *)d0 + stride;
        s0 += 0x80;
    }

    SetSpriteInvisible(gReceiveSceneSprite1, 0);
    SetSpriteInvisible(gReceiveSceneSprite2, 0);
}
