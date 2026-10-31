#ifndef GUARD_MAIN_H
#define GUARD_MAIN_H

// The engine in main.c. It has a shadow of the display registers flushed
// each frame, OAM, key repeat and two stacks of callbacks (the top one runs).

typedef void (*CallbackFunc)();

struct IoRegs
{
    vu16 dispcnt;
    vu16 dispstat;
    vu16 bgcnt[4];
    vu16 bghofs[4];
    vu16 bgvofs[4];
    vu16 bg2pa;
    vu16 bg2pb;
    vu16 bg2pc;
    vu16 bg2pd;
    vu16 bg2x_l;
    vu16 bg2x_h;
    vu16 bg2y_l;
    vu16 bg2y_h;
    vu16 bg3pa;
    vu16 bg3pb;
    vu16 bg3pc;
    vu16 bg3pd;
    vu16 bg3x_l;
    vu16 bg3x_h;
    vu16 bg3y_l;
    vu16 bg3y_h;
    vu16 win0h;
    vu16 win1h;
    vu16 win0v;
    vu16 win1v;
    vu16 winin;
    vu16 winout;
    vu16 mosaic;
    vu16 bldcnt;
    vu16 bldalpha;
    vu16 bldy;
    vu16 ime;
    vu16 ie;
};

struct OamEntry
{
    u16 attr0;
    u16 attr1;
    u16 attr2;
    s16 affineParam;
};

struct Keys
{
    u32 unused;
    u16 held;
    u16 newKeys;
    u16 repeat;
    u16 prevHeld;
    s16 initialDelay;
    s16 minDelay;
    s16 accel;
    s16 dpadDelay;
    s16 dpadTimer;
    s16 buttonDelay;
    s16 buttonTimer;
    s16 keyDelay[16];
    s16 keyTimer[16];
};

struct CallbackStack
{
    u16 count;
    CallbackFunc funcs[4];
    s16 args[4];
};

extern struct Keys gKeys;
extern struct CallbackStack gSubCallbacks;
extern struct IoRegs gIoRegs;
extern struct CallbackStack gMainCallbacks;
extern struct OamEntry gOamBuffer[128];

void RequestCopy(void *dest, const void *src, u32 size);
void LoadPalette(const void *src, u16 offset, u16 size);
void ResetOam(u8 oamStart, u8 affineStart);
void AddSprite(u16 x, u16 y, const u16 *data, u16 flags, s32 tileOffset);
void SetAffineParams(u8 index, u8 angle, u16 xScale, u16 yScale, u8 flip);
void DoOamBufferTransfer(void);
void InitInterrupts(void);
void IntrMain(void); // asm/crt0.s
void PushMainCallback(CallbackFunc func, s16 arg);
void ReplaceMainCallback(CallbackFunc func, u16 arg);
void ClearSubCallbacks(void);
void PushSubCallback(CallbackFunc func, s16 arg);
void PopSubCallback(void);
void ReplaceSubCallback(CallbackFunc func, s32 arg);
void InitIoRegs(void);
void FlushIoRegs(void);
void ExitForcedBlank(void);
void EnterForcedBlank(void);
void FillTilemapRect(u16 *tilemap, u8 x, u8 y, u8 width, u8 height, u16 tile, u16 tileDelta);
void SeedRng2(u32 seed);
u16 Random2(u16 max);

#define BG_CHAR_BASE(bgcnt) ((void *)((((bgcnt) & 0xC) << 12) | VRAM))
#define BG_SCREEN_BASE(bgcnt) ((void *)((((bgcnt) & 0x1F00) << 3) | VRAM))

#endif // GUARD_MAIN_H
