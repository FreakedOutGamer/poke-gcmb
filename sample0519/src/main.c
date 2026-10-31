#include "global.h"
#include "main.h"
#include "rtc.h"
#include "delivery.h"


struct CopyRequest
{
    void *dest;
    const void *src;
    u32 size;
};

// Checks whether the VBlank interrupt is on and will fire.
#define VBLANK_INTR_ACTIVE()                         \
    ((REG_IME & 1)                                   \
     && (REG_DISPSTAT & DISPSTAT_VBLANK_INTR)        \
     && (REG_IE & INTR_FLAG_VBLANK)                  \
     && !(REG_DISPCNT & DISPCNT_FORCED_BLANK))

// One affine parameter group, spread over four OAM entries.
struct OamAffine
{
    u16 fill0[3];
    s16 pa;
    u16 fill1[3];
    s16 pb;
    u16 fill2[3];
    s16 pc;
    u16 fill3[3];
    s16 pd;
};

extern vu32 gRestartRequested;
extern u32 gLagFrame;
extern u8 gNextOamIndex;
extern u8 gNextAffineIndex;
extern u16 gRng2High;
extern u16 gRng2Low;
extern IntrFunc *gHBlankIntrSlot;
extern vu8 gCopyRequestCount;
extern IntrFunc gVCountCallback;
extern u16 gPaletteBuffer[];
extern IntrFunc gHBlankCallback;
extern IntrFunc gVBlankCallback;
extern u16 *gPaletteBufferPtr;
extern u32 gStartedOnce;
extern vu8 gVBlankWaiting;
extern u32 gVBlankCounter;
extern IntrFunc gPendingVBlankCallback;
extern IntrFunc *gVCountIntrSlot;
extern struct CopyRequest gCopyRequests[8];
extern u32 gIntrMainBuffer[];
extern IntrFunc gIntrTable[];


void InitIntrHandlers(void);
IntrFunc SetVBlankCallback(IntrFunc func);
void DefaultVBlankCallback(void);
void ProcessCopyRequests(void);
void WaitForVBlank(void);
IntrFunc ClearHBlankCallback(void);
IntrFunc ClearVCountCallback(void);
void ResetPaletteBufferPtr(void);
void InitKeys(u16, s16, s16);
void ReadKeys(void);
void ClearMainCallbacks(void);
void EnableVBlankIntr(void);
void DisableHBlankIntr(void);
void DisableVCountIntr(void);
static void IntrDummy(void);
void HideSpritesFrom(u8 start);
void ResetAffineParamsFrom(u8 start);

const u16 gOamAttrMasks[10] = {
    0xFF00, 0x00FF, 0x001F,
    0xFE00, 0x01FF, 0x03E0,
    0xFC00, 0x03FF, 0xFC00,
};

const u16 gRepeatKeys[12] = {
    DPAD_UP, DPAD_DOWN, DPAD_LEFT, DPAD_RIGHT, A_BUTTON, B_BUTTON,
    L_BUTTON, R_BUTTON, START_BUTTON, SELECT_BUTTON, 0,
};

// sin and cos of i/256 of a turn, times 256.
const s16 gSineTable[256] = {
    0, 6, 12, 18, 25, 31, 37, 43, 49, 56, 62, 68, 74, 80, 86, 92,
    97, 103, 109, 115, 120, 126, 131, 136, 142, 147, 152, 157, 162, 167, 171, 176,
    181, 185, 189, 193, 197, 201, 205, 209, 212, 216, 219, 222, 225, 228, 231, 234,
    236, 238, 241, 243, 244, 246, 248, 249, 251, 252, 253, 254, 254, 255, 255, 255,
    256, 255, 255, 255, 254, 254, 253, 252, 251, 249, 248, 246, 244, 243, 241, 238,
    236, 234, 231, 228, 225, 222, 219, 216, 212, 209, 205, 201, 197, 193, 189, 185,
    181, 176, 171, 167, 162, 157, 152, 147, 142, 136, 131, 126, 120, 115, 109, 103,
    97, 92, 86, 80, 74, 68, 62, 56, 49, 43, 37, 31, 25, 18, 12, 6,
    0, -6, -12, -18, -25, -31, -37, -43, -49, -56, -62, -68, -74, -80, -86, -92,
    -97, -103, -109, -115, -120, -126, -131, -136, -142, -147, -152, -157, -162, -167, -171, -176,
    -181, -185, -189, -193, -197, -201, -205, -209, -212, -216, -219, -222, -225, -228, -231, -234,
    -236, -238, -241, -243, -244, -246, -248, -249, -251, -252, -253, -254, -254, -255, -255, -255,
    -256, -255, -255, -255, -254, -254, -253, -252, -251, -249, -248, -246, -244, -243, -241, -238,
    -236, -234, -231, -228, -225, -222, -219, -216, -212, -209, -205, -201, -197, -193, -189, -185,
    -181, -176, -171, -167, -162, -157, -152, -147, -142, -136, -131, -126, -120, -115, -109, -103,
    -97, -92, -86, -80, -74, -68, -62, -56, -49, -43, -37, -31, -25, -18, -12, -6,
};

const s16 gCosineTable[256] = {
    256, 255, 255, 255, 254, 254, 253, 252, 251, 249, 248, 246, 244, 243, 241, 238,
    236, 234, 231, 228, 225, 222, 219, 216, 212, 209, 205, 201, 197, 193, 189, 185,
    181, 176, 171, 167, 162, 157, 152, 147, 142, 136, 131, 126, 120, 115, 109, 103,
    97, 92, 86, 80, 74, 68, 62, 56, 49, 43, 37, 31, 25, 18, 12, 6,
    0, -6, -12, -18, -25, -31, -37, -43, -49, -56, -62, -68, -74, -80, -86, -92,
    -97, -103, -109, -115, -120, -126, -131, -136, -142, -147, -152, -157, -162, -167, -171, -176,
    -181, -185, -189, -193, -197, -201, -205, -209, -212, -216, -219, -222, -225, -228, -231, -234,
    -236, -238, -241, -243, -244, -246, -248, -249, -251, -252, -253, -254, -254, -255, -255, -255,
    -256, -255, -255, -255, -254, -254, -253, -252, -251, -249, -248, -246, -244, -243, -241, -238,
    -236, -234, -231, -228, -225, -222, -219, -216, -212, -209, -205, -201, -197, -193, -189, -185,
    -181, -176, -171, -167, -162, -157, -152, -147, -142, -136, -131, -126, -120, -115, -109, -103,
    -97, -92, -86, -80, -74, -68, -62, -56, -49, -43, -37, -31, -25, -18, -12, -6,
    0, 6, 12, 18, 25, 31, 37, 43, 49, 56, 62, 68, 74, 80, 86, 92,
    97, 103, 109, 115, 120, 126, 131, 136, 142, 147, 152, 157, 162, 167, 171, 176,
    181, 185, 189, 193, 197, 201, 205, 209, 212, 216, 219, 222, 225, 228, 231, 234,
    236, 238, 241, 243, 244, 246, 248, 249, 251, 252, 253, 254, 254, 255, 255, 255,
};
static void VBlankIntr(void);

void AgbMain(void)
{
    DmaFill32(3, 0, (void *)0x03000000, 0x7F00);
    DmaFill32(3, 0, (void *)0x02000000, 0x18000);
    while (1)
    {
        InitIoRegs();
        gIoRegs.ime = 0;
        REG_IME = 0;
        FlushIoRegs();
        DmaFill16(3, 0, (void *)VRAM, VRAM_SIZE);
        DmaFill32(3, 0, (void *)OAM, OAM_SIZE);
        DmaFill16(3, 0, (void *)PLTT, PLTT_SIZE);
        InitInterrupts();
        ResetPaletteBufferPtr();
        RtcInit();
        gVBlankCounter = 0;
        InitKeys(0x20, 4, 2);
        ClearMainCallbacks();
        if (gStartedOnce != 0)
        {
            PushMainCallback(DeliveryMainCallback, 0);
        }
        else
        {
            PushMainCallback(DeliveryMainCallback, 0);
            gStartedOnce = 1;
        }
        SeedRng2(0x19720917);
        gLagFrame = 0;
        gRestartRequested = 0;
        while (gRestartRequested == 0)
        {
            ReadKeys();
            if (gMainCallbacks.count != 0)
                gMainCallbacks.funcs[gMainCallbacks.count - 1]();
            WaitForVBlank();
        }
    }
}

void InitIntrHandlers(void)
{
    u32 i;

    for (i = 0; i < 15; i++)
        gIntrTable[i] = IntrDummy;
    gIntrTable[3] = VBlankIntr;
    gHBlankIntrSlot = &gIntrTable[1];
    gVCountIntrSlot = &gIntrTable[2];
    ClearHBlankCallback();
    ClearVCountCallback();
}

void InitInterrupts(void)
{
    InitIntrHandlers();
    SetVBlankCallback(DefaultVBlankCallback);
    CpuCopy16(IntrMain, gIntrMainBuffer, 0x400);
    INTR_VECTOR = gIntrMainBuffer;
}

void UNUSED ResetInterrupts(void)
{
    EnableVBlankIntr();
    DisableHBlankIntr();
    DisableVCountIntr();
    InitIntrHandlers();
    SetVBlankCallback(DefaultVBlankCallback);
}

static void IntrDummy(void)
{
}

static void VBlankIntr(void)
{
    gVBlankCallback = gPendingVBlankCallback;
    *gHBlankIntrSlot = gHBlankCallback;
    *gVCountIntrSlot = gVCountCallback;
    if (gVBlankCallback)
        gVBlankCallback();
    gVBlankCounter++;
    REG_IME = 0;
    INTR_CHECK |= INTR_FLAG_VBLANK;
    REG_IME = 1;
    gLagFrame = (gVBlankWaiting == 0);
    gVBlankWaiting = 0;
}

// BUG: This sets DISPSTAT bit 0 (the read-only VBlank flag) and IE bit 3
// (timer 0) where it means DISPSTAT_VBLANK_INTR and INTR_FLAG_VBLANK, the
// bits DisableVBlankIntr clears.
void EnableVBlankIntr(void)
{
    if (!(gIoRegs.ie & INTR_FLAG_VBLANK))
    {
        if (REG_IF & INTR_FLAG_VBLANK)
            REG_IF |= INTR_FLAG_VBLANK;
        gIoRegs.dispstat |= INTR_FLAG_VBLANK;
        gIoRegs.ie |= DISPSTAT_VBLANK_INTR;
        REG_DISPSTAT = gIoRegs.dispstat;
        REG_IE = gIoRegs.ie;
    }
}

void UNUSED DisableVBlankIntr(void)
{
    gIoRegs.dispstat &= ~DISPSTAT_VBLANK_INTR;
    gIoRegs.ie &= ~INTR_FLAG_VBLANK;
    if (REG_IF & INTR_FLAG_VBLANK)
        REG_IF |= INTR_FLAG_VBLANK;
    if (!VBLANK_INTR_ACTIVE())
    {
        REG_IME = 0;
        REG_DISPSTAT = gIoRegs.dispstat;
        REG_IME = gIoRegs.ime;
    }
    REG_IME = 0;
    REG_IE = gIoRegs.ie;
    REG_IME = gIoRegs.ime;
}

IntrFunc SetVBlankCallback(IntrFunc func)
{
    IntrFunc old = gVBlankCallback;

    gPendingVBlankCallback = func;
    if (!VBLANK_INTR_ACTIVE())
        gVBlankCallback = func;
    return old;
}

IntrFunc UNUSED SetDefaultVBlankCallback(void)
{
    return SetVBlankCallback(DefaultVBlankCallback);
}

void DefaultVBlankCallback(void)
{
    if (gVBlankWaiting)
    {
        FlushIoRegs();
        DmaCopy32(3, gOamBuffer, (void *)OAM, OAM_SIZE);
        ProcessCopyRequests();
        DmaCopy16(3, gPaletteBufferPtr, (void *)PLTT, PLTT_SIZE);
    }
}

void WaitForVBlank(void)
{
    if ((REG_IME & 1) && (REG_DISPSTAT & DISPSTAT_VBLANK_INTR) && (REG_IE & INTR_FLAG_VBLANK))
    {
        if (REG_DISPCNT & DISPCNT_FORCED_BLANK)
        {
            if (gVBlankCallback)
                gVBlankCallback();
        }
        else
        {
            gVBlankWaiting = 0xFF;
            while (gVBlankWaiting)
                VBlankIntrWait();
        }
    }
}

void UNUSED EnableHBlankIntr(void)
{
    if (REG_IF & INTR_FLAG_HBLANK)
        REG_IF |= INTR_FLAG_HBLANK;
    gIoRegs.dispstat |= DISPSTAT_HBLANK_INTR;
    gIoRegs.ie |= INTR_FLAG_HBLANK;
    if (!VBLANK_INTR_ACTIVE())
        REG_DISPSTAT = gIoRegs.dispstat;
    REG_IE = gIoRegs.ie;
}

void DisableHBlankIntr(void)
{
    gIoRegs.dispstat &= ~DISPSTAT_HBLANK_INTR;
    gIoRegs.ie &= ~INTR_FLAG_HBLANK;
    if (REG_IF & INTR_FLAG_HBLANK)
        REG_IF |= INTR_FLAG_HBLANK;
    if (!VBLANK_INTR_ACTIVE())
    {
        REG_IME = 0;
        REG_DISPSTAT = gIoRegs.dispstat;
        REG_IME = gIoRegs.ime;
    }
    REG_IME = 0;
    REG_IE = gIoRegs.ie;
    REG_IME = gIoRegs.ime;
}

IntrFunc SetHBlankCallback(IntrFunc func)
{
    IntrFunc old = *gHBlankIntrSlot;

    gHBlankCallback = func;
    if (!VBLANK_INTR_ACTIVE())
        *gHBlankIntrSlot = func;
    return old;
}

IntrFunc ClearHBlankCallback(void)
{
    return SetHBlankCallback(IntrDummy);
}

void DisableVCountIntr(void)
{
    gIoRegs.dispstat &= ~DISPSTAT_VCOUNT_INTR;
    gIoRegs.ie &= ~INTR_FLAG_VCOUNT;
    if (!VBLANK_INTR_ACTIVE())
    {
        REG_IME = 0;
        REG_DISPSTAT = gIoRegs.dispstat;
        REG_IME = gIoRegs.ime;
    }
    REG_IME = 0;
    REG_IE = gIoRegs.ie;
    REG_IME = gIoRegs.ime;
    if (REG_IF & INTR_FLAG_VCOUNT)
        REG_IF |= INTR_FLAG_VCOUNT;
}

IntrFunc SetVCountCallback(IntrFunc func)
{
    IntrFunc old = *gVCountIntrSlot;

    gVCountCallback = func;
    if (!VBLANK_INTR_ACTIVE())
        *gVCountIntrSlot = func;
    return old;
}

IntrFunc ClearVCountCallback(void)
{
    return SetVCountCallback(IntrDummy);
}

void RequestCopy(void *dest, const void *src, u32 size)
{
    if (REG_DISPCNT & DISPCNT_FORCED_BLANK)
    {
        CpuCopy16(src, dest, size);
    }
    else if (gCopyRequestCount < 8)
    {
        struct CopyRequest *req = &gCopyRequests[gCopyRequestCount++];

        req->dest = dest;
        req->src = src;
        req->size = size;
    }
}

void ProcessCopyRequests(void)
{
    struct CopyRequest *req = gCopyRequests;

    while (gCopyRequestCount != 0)
    {
        CpuCopy16(req->src, req->dest, req->size);
        req++;
        gCopyRequestCount--;
    }
}

void ResetPaletteBufferPtr(void)
{
    gPaletteBufferPtr = gPaletteBuffer;
}

void LoadPalette(const void *src, u16 offset, u16 size)
{
    CpuSet(src, gPaletteBufferPtr + offset, size);
}

void UNUSED CopyPaletteBufferToPltt(void)
{
    DmaCopy16(3, gPaletteBufferPtr, (void *)PLTT, PLTT_SIZE);
}

void ResetOam(u8 oamStart, u8 affineStart)
{
    HideSpritesFrom(oamStart);
    ResetAffineParamsFrom(affineStart);
}

// Moves every OAM entry from start on off screen.
void HideSpritesFrom(u8 start)
{
    struct OamEntry *oam;
    u8 i;

    gNextOamIndex = start;
    if (!(start & 0x80))
    {
        oam = &gOamBuffer[start];
        for (i = start; i & 7; i++)
        {
            oam->attr0 = DISPLAY_HEIGHT + 32;
            oam++;
        }
        for (; i < 128; i += 8)
        {
            oam[0].attr0 = DISPLAY_HEIGHT + 32;
            oam[1].attr0 = DISPLAY_HEIGHT + 32;
            oam[2].attr0 = DISPLAY_HEIGHT + 32;
            oam[3].attr0 = DISPLAY_HEIGHT + 32;
            oam[4].attr0 = DISPLAY_HEIGHT + 32;
            oam[5].attr0 = DISPLAY_HEIGHT + 32;
            oam[6].attr0 = DISPLAY_HEIGHT + 32;
            oam[7].attr0 = DISPLAY_HEIGHT + 32;
            oam += 8;
        }
    }
}

// Resets every affine parameter group from start on to the identity.
// BUG: The groups are indexed like OAM entries, so this starts at group
// start / 4 rather than start (SetAffineParams gets it right).
void ResetAffineParamsFrom(u8 start)
{
    u8 i;

    gNextAffineIndex = start;
    if (!(start & 0x20))
    {
        s16 *affine = (s16 *)&gOamBuffer[start];
        for (i = start; i < 32; i++)
        {
            affine[3] = 0x100;
            affine[7] = 0;
            affine[11] = 0;
            affine[15] = 0x100;
            affine += 16;
        }
    }
}

void UNUSED BuildSprite(u8 index, u16 x, u16 y, const u16 *src, u16 flags, s32 tileOffset)
{
    const u16 *data = src;
    struct OamEntry *oam = &gOamBuffer[index];

    while (!(index & 0x80) && (u16)(data[0] + 1) != 0)
    {
        oam->attr0 = (data[0] & gOamAttrMasks[0]) | ((y + data[0]) & gOamAttrMasks[1]) | ((gOamAttrMasks[2] & flags) << 8);
        oam->attr1 = (data[1] & gOamAttrMasks[3]) | ((x + data[1]) & gOamAttrMasks[4]) | ((gOamAttrMasks[5] & flags) << 4);
        oam->attr2 = (data[2] & gOamAttrMasks[6]) | ((data[2] + tileOffset) & gOamAttrMasks[7]) | (gOamAttrMasks[8] & flags);
        oam++;
        index++;
        data += 3;
    }
}

void AddSprite(u16 x, u16 y, const u16 *src, u16 flags, s32 tileOffset)
{
    const u16 *data = src;
    struct OamEntry *oam = &gOamBuffer[gNextOamIndex];

    while (!(gNextOamIndex & 0x80) && (u16)(data[0] + 1) != 0)
    {
        oam->attr0 = (data[0] & gOamAttrMasks[0]) | ((y + data[0]) & gOamAttrMasks[1]) | ((gOamAttrMasks[2] & flags) << 8);
        oam->attr1 = (data[1] & gOamAttrMasks[3]) | ((x + data[1]) & gOamAttrMasks[4]) | ((gOamAttrMasks[5] & flags) << 4);
        oam->attr2 = (data[2] & gOamAttrMasks[6]) | ((data[2] + tileOffset) & gOamAttrMasks[7]) | (gOamAttrMasks[8] & flags);
        oam++;
        gNextOamIndex++;
        data += 3;
    }
}

void SetAffineParams(u8 index, u8 angle, u16 xScale, u16 yScale, u8 flip)
{
    struct OamAffine *affine;
    s32 a, b;

    if (index & 0xE0)
        return;
    if (xScale == 0)
        xScale = 0x100;
    if (yScale == 0)
        yScale = 0x100;
    affine = &((struct OamAffine *)gOamBuffer)[index];
    if (angle)
    {
        switch (flip & 3)
        {
        case 0:
            a = gCosineTable[angle];
            a <<= 8;
            affine->pa = a / xScale;
            b = gSineTable[angle];
            affine->pb = (-b << 8) / xScale;
            b <<= 8;
            affine->pc = b / yScale;
            affine->pd = a / yScale;
            break;
        case 1:
            a = gCosineTable[angle];
            affine->pa = (-a << 8) / xScale;
            b = gSineTable[angle];
            b <<= 8;
            affine->pb = b / xScale;
            affine->pc = b / yScale;
            a <<= 8;
            affine->pd = a / yScale;
            break;
        case 2:
            a = gCosineTable[angle];
            affine->pa = (a << 8) / xScale;
            b = gSineTable[angle];
            b = -b;
            b <<= 8;
            affine->pb = b / xScale;
            affine->pc = b / yScale;
            a = -a;
            a <<= 8;
            affine->pd = a / yScale;
            break;
        case 3:
            a = gCosineTable[angle];
            a = -a;
            a <<= 8;
            affine->pa = a / xScale;
            b = gSineTable[angle];
            affine->pb = (b << 8) / xScale;
            b = -b;
            b <<= 8;
            affine->pc = b / yScale;
            affine->pd = a / yScale;
            break;
        }
    }
    else
    {
        affine->pa = 0x10000 / xScale;
        affine->pb = angle;
        affine->pc = angle;
        affine->pd = 0x10000 / yScale;
    }
}

u8 UNUSED AddAffineParams(u8 angle, u16 xScale, u16 yScale, u8 flip)
{
    if (gNextAffineIndex & 0xE0)
        return 0;
    SetAffineParams(gNextAffineIndex, angle, xScale, yScale, flip);
    return gNextAffineIndex++;
}

void DoOamBufferTransfer(void)
{
    DmaCopy32(3, gOamBuffer, (void *)OAM, OAM_SIZE);
}

void InitKeys(u16 initialDelay, s16 minDelay, s16 accel)
{
    u32 i;

    gKeys.held = 0;
    gKeys.newKeys = 0;
    gKeys.prevHeld = 0;
    gKeys.initialDelay = initialDelay;
    gKeys.minDelay = minDelay;
    gKeys.accel = accel;
    gKeys.dpadDelay = initialDelay;
    gKeys.dpadTimer = 0;
    gKeys.buttonDelay = initialDelay;
    gKeys.buttonTimer = 0;
    for (i = 0; i < 16; i++)
    {
        gKeys.keyDelay[i] = initialDelay;
        gKeys.keyTimer[i] = initialDelay;
    }
}

void ReadKeys(void)
{
    u16 keys = REG_KEYINPUT ^ KEYS_MASK;

    gKeys.newKeys = keys & ~gKeys.held;
    gKeys.held = keys;
}

void UNUSED UpdateKeyRepeat(u16 mask)
{
    u16 keys;

    keys = mask & DPAD_ANY;
    gKeys.repeat = (gKeys.repeat & ~keys) | (gKeys.newKeys & keys);
    if ((gKeys.held & keys) && (gKeys.held & keys) == (gKeys.prevHeld & keys))
    {
        if (--gKeys.dpadTimer <= 0)
        {
            gKeys.dpadDelay -= gKeys.accel;
            if (gKeys.dpadDelay < gKeys.minDelay)
                gKeys.dpadDelay = gKeys.minDelay;
            gKeys.dpadTimer = gKeys.dpadDelay;
            gKeys.repeat = (gKeys.repeat & ~keys) | (keys & gKeys.held);
        }
    }
    else
    {
        gKeys.dpadDelay += gKeys.accel;
        if (gKeys.dpadDelay > gKeys.initialDelay)
            gKeys.dpadDelay = gKeys.initialDelay;
        gKeys.dpadTimer = gKeys.dpadDelay;
    }

    keys = mask & (A_BUTTON | B_BUTTON | SELECT_BUTTON | START_BUTTON | R_BUTTON | L_BUTTON);
    gKeys.repeat = (gKeys.repeat & ~keys) | (gKeys.newKeys & keys);
    if ((gKeys.held & keys) && (gKeys.held & keys) == (gKeys.prevHeld & keys))
    {
        if (--gKeys.buttonTimer <= 0)
        {
            gKeys.buttonDelay -= gKeys.accel;
            if (gKeys.buttonDelay < gKeys.minDelay)
                gKeys.buttonDelay = gKeys.minDelay;
            gKeys.buttonTimer = gKeys.buttonDelay;
            gKeys.repeat = (gKeys.repeat & ~keys) | (keys & gKeys.held);
        }
    }
    else
    {
        gKeys.buttonDelay += gKeys.accel;
        if (gKeys.buttonDelay > gKeys.initialDelay)
            gKeys.buttonDelay = gKeys.initialDelay;
        gKeys.buttonTimer = gKeys.buttonDelay;
    }

    gKeys.prevHeld = gKeys.held;
}

void UNUSED UpdateKeyRepeatPerKey(u16 mask)
{
    s32 i;
    u16 key;
    s16 *delay = gKeys.keyDelay;
    s16 *timer = gKeys.keyTimer;

    for (i = 0; (key = gRepeatKeys[i]) != 0; i++)
    {
        if (key & mask)
        {
            gKeys.repeat = (gKeys.repeat & ~key) | (gKeys.newKeys & key);
            if ((gKeys.held & key) && (gKeys.held & key) == (gKeys.prevHeld & key))
            {
                if (--timer[i] <= 0)
                {
                    delay[i] -= gKeys.accel;
                    if (delay[i] < gKeys.minDelay)
                        delay[i] = gKeys.minDelay;
                    timer[i] = delay[i];
                    gKeys.repeat = (gKeys.repeat & ~key) | (key & gKeys.held);
                }
            }
            else
            {
                delay[i] += gKeys.accel;
                if (delay[i] > gKeys.initialDelay)
                    delay[i] = gKeys.initialDelay;
                timer[i] = delay[i];
            }
        }
    }
    gKeys.prevHeld = gKeys.held;
}

void ClearMainCallbacks(void)
{
    gMainCallbacks.count = 0;
}

void PushMainCallback(CallbackFunc func, s16 arg)
{
    if (gMainCallbacks.count < 4)
    {
        gMainCallbacks.funcs[gMainCallbacks.count] = func;
        gMainCallbacks.args[gMainCallbacks.count] = arg;
        gMainCallbacks.count++;
    }
}

void PopMainCallback(void)
{
    if (gMainCallbacks.count != 0)
        gMainCallbacks.count--;
}

void ReplaceMainCallback(CallbackFunc func, u16 arg)
{
    PopMainCallback();
    PushMainCallback(func, arg);
}

void UNUSED MainCallbackDummy(void)
{
}

// BUG: This compares func with the funcs array itself rather than with
// funcs[i], so it is always FALSE.
bool32 UNUSED IsMainCallbackActive(CallbackFunc func)
{
    bool32 found = FALSE;
    u16 i = gMainCallbacks.count;

    while (--i != 0xFFFF)
    {
        if (func == (CallbackFunc)gMainCallbacks.funcs)
        {
            found = TRUE;
            break;
        }
    }
    return found;
}

void ClearSubCallbacks(void)
{
    gSubCallbacks.count = 0;
}

void PushSubCallback(CallbackFunc func, s16 arg)
{
    if (gSubCallbacks.count < 4)
    {
        gSubCallbacks.funcs[gSubCallbacks.count] = func;
        gSubCallbacks.args[gSubCallbacks.count] = arg;
        gSubCallbacks.count++;
    }
}

void PopSubCallback(void)
{
    if (gSubCallbacks.count != 0)
        gSubCallbacks.count--;
}

void ReplaceSubCallback(CallbackFunc func, s32 value)
{
    u16 arg = value;
    PopSubCallback();
    PushSubCallback(func, arg);
}

void UNUSED SubCallbackDummy(void)
{
}

// BUG: As in IsMainCallbackActive, this is always FALSE.
bool32 UNUSED IsSubCallbackActive(CallbackFunc func)
{
    bool32 found = FALSE;
    u16 i = gSubCallbacks.count;

    while (--i != 0xFFFF)
    {
        if (func == (CallbackFunc)gSubCallbacks.funcs)
        {
            found = TRUE;
            break;
        }
    }
    return found;
}

void InitIoRegs(void)
{
    gIoRegs.dispcnt = DISPCNT_OBJ_ON | DISPCNT_BG0_ON | DISPCNT_FORCED_BLANK | DISPCNT_OBJ_1D_MAP | DISPCNT_HBLANK_INTERVAL;
    gIoRegs.dispstat = DISPSTAT_VBLANK_INTR;
    gIoRegs.bgcnt[0] = BGCNT_SCREENBASE(24);
    gIoRegs.bgcnt[1] = 0;
    gIoRegs.bgcnt[2] = 0;
    gIoRegs.bgcnt[3] = 0;
    gIoRegs.bghofs[0] = 0;
    gIoRegs.bghofs[1] = 0;
    gIoRegs.bghofs[2] = 0;
    gIoRegs.bghofs[3] = 0;
    gIoRegs.bgvofs[0] = 0;
    gIoRegs.bgvofs[1] = 0;
    gIoRegs.bgvofs[2] = 0;
    gIoRegs.bgvofs[3] = 0;
    gIoRegs.bg2pa = 0;
    gIoRegs.bg2pb = 0;
    gIoRegs.bg2pc = 0;
    gIoRegs.bg2pd = 0;
    gIoRegs.bg2x_l = 0;
    gIoRegs.bg2x_h = 0;
    gIoRegs.bg2y_l = 0;
    gIoRegs.bg2y_h = 0;
    gIoRegs.bg3pa = 0;
    gIoRegs.bg3pb = 0;
    gIoRegs.bg3pc = 0;
    gIoRegs.bg3pd = 0;
    gIoRegs.bg3x_l = 0;
    gIoRegs.bg3x_h = 0;
    gIoRegs.bg3y_l = 0;
    gIoRegs.bg3y_h = 0;
    gIoRegs.win0h = 0;
    gIoRegs.win1h = 0;
    gIoRegs.win0v = 0;
    gIoRegs.win1v = 0;
    gIoRegs.winin = 0;
    gIoRegs.winout = 0;
    gIoRegs.mosaic = 0;
    gIoRegs.bldcnt = 0;
    gIoRegs.bldalpha = 0;
    gIoRegs.bldy = 0;
    gIoRegs.ime = 1;
    gIoRegs.ie = INTR_FLAG_GAMEPAK | INTR_FLAG_VBLANK;
}

void FlushIoRegs(void)
{
    REG_DISPCNT = gIoRegs.dispcnt;
    REG_DISPSTAT = gIoRegs.dispstat;
    REG_BG0CNT = gIoRegs.bgcnt[0];
    REG_BG1CNT = gIoRegs.bgcnt[1];
    REG_BG2CNT = gIoRegs.bgcnt[2];
    REG_BG3CNT = gIoRegs.bgcnt[3];
    REG_BG0HOFS = gIoRegs.bghofs[0];
    REG_BG1HOFS = gIoRegs.bghofs[1];
    REG_BG2HOFS = gIoRegs.bghofs[2];
    REG_BG3HOFS = gIoRegs.bghofs[3];
    REG_BG0VOFS = gIoRegs.bgvofs[0];
    REG_BG1VOFS = gIoRegs.bgvofs[1];
    REG_BG2VOFS = gIoRegs.bgvofs[2];
    REG_BG3VOFS = gIoRegs.bgvofs[3];
    REG_BG2PA = gIoRegs.bg2pa;
    REG_BG2PB = gIoRegs.bg2pb;
    REG_BG2PC = gIoRegs.bg2pc;
    REG_BG2PD = gIoRegs.bg2pd;
    REG_BG2X_L = gIoRegs.bg2x_l;
    REG_BG2X_H = gIoRegs.bg2x_h;
    REG_BG2Y_L = gIoRegs.bg2y_l;
    REG_BG2Y_H = gIoRegs.bg2y_h;
    REG_BG3PA = gIoRegs.bg3pa;
    REG_BG3PB = gIoRegs.bg3pb;
    REG_BG3PC = gIoRegs.bg3pc;
    REG_BG3PD = gIoRegs.bg3pd;
    REG_BG3X_L = gIoRegs.bg3x_l;
    REG_BG3X_H = gIoRegs.bg3x_h;
    REG_BG3Y_L = gIoRegs.bg3y_l;
    REG_BG3Y_H = gIoRegs.bg3y_h;
    REG_WIN0H = gIoRegs.win0h;
    REG_WIN1H = gIoRegs.win1h;
    REG_WIN0V = gIoRegs.win0v;
    REG_WIN1V = gIoRegs.win1v;
    REG_WININ = gIoRegs.winin;
    REG_WINOUT = gIoRegs.winout;
    REG_MOSAIC = gIoRegs.mosaic;
    REG_BLDCNT = gIoRegs.bldcnt;
    REG_BLDALPHA = gIoRegs.bldalpha;
    REG_BLDY = gIoRegs.bldy;
    REG_IME = gIoRegs.ime;
    REG_IE = gIoRegs.ie;
}

void UNUSED WaitFrames(u32 frames)
{
    while (frames-- != 0)
        WaitForVBlank();
}

void ExitForcedBlank(void)
{
    if (REG_DISPCNT & DISPCNT_FORCED_BLANK)
    {
        gIoRegs.dispcnt &= ~DISPCNT_FORCED_BLANK;
        REG_DISPCNT = gIoRegs.dispcnt;
    }
}

void EnterForcedBlank(void)
{
    if (!(REG_DISPCNT & DISPCNT_FORCED_BLANK))
    {
        gIoRegs.dispcnt |= DISPCNT_FORCED_BLANK;
        if ((REG_DISPSTAT & DISPSTAT_VBLANK_INTR) && (REG_IE & INTR_FLAG_VBLANK))
            WaitForVBlank();
        else
            REG_DISPCNT = gIoRegs.dispcnt;
    }
}

void * UNUSED GetBgCharBase(u8 bg)
{
    switch (bg)
    {
    case 1:
        return (void *)(((gIoRegs.bgcnt[1] & 0xC) << 12) | VRAM);
    case 2:
        return (void *)(((gIoRegs.bgcnt[2] & 0xC) << 12) | VRAM);
    case 3:
        return (void *)(((gIoRegs.bgcnt[3] & 0xC) << 12) | VRAM);
    default:
        return (void *)(((gIoRegs.bgcnt[0] & 0xC) << 12) | VRAM);
    }
}

void * UNUSED GetBgScreenBase(u8 bg)
{
    switch (bg)
    {
    case 1:
        return (void *)(((gIoRegs.bgcnt[1] & 0x1F00) << 3) | VRAM);
    case 2:
        return (void *)(((gIoRegs.bgcnt[2] & 0x1F00) << 3) | VRAM);
    case 3:
        return (void *)(((gIoRegs.bgcnt[3] & 0x1F00) << 3) | VRAM);
    default:
        return (void *)(((gIoRegs.bgcnt[0] & 0x1F00) << 3) | VRAM);
    }
}

u32 UNUSED GetBgDummy(u8 bg)
{
    u16 bgcnt;

    switch (bg)
    {
    case 1:
        bgcnt = gIoRegs.bgcnt[1];
        break;
    case 2:
        bgcnt = gIoRegs.bgcnt[2];
        break;
    case 3:
        bgcnt = gIoRegs.bgcnt[3];
        break;
    default:
        bgcnt = gIoRegs.bgcnt[0];
        break;
    }
    return 0;
}

void FillTilemapRect(u16 *tilemap, u8 x, u8 y, u8 width, u8 height, u16 tile, u16 tileDelta)
{
    u16 i, j;
    u16 *base = tilemap + y * 32 + x;

    for (j = 0; j < height; j++)
    {
        tilemap = base + j * 32;
        for (i = 0; i < width; i++)
        {
            *tilemap = tile;
            tile = (tile & 0xFC00) | ((tile + tileDelta) & 0x3FF);
            tilemap++;
        }
    }
}

void SeedRng2(u32 seed)
{
    gRng2High = seed;
    gRng2Low = seed >> 16;
}

u16 Random2(u16 max)
{
    u16 lo = gRng2Low;
    u32 a = ((lo << 2) & 0xFFFF) + lo;
    u32 b = (gRng2High << 1) + (a >> 16);
    u32 c = b + (lo + (b >> 16));

    gRng2Low = a;
    gRng2High = c;
    return (max * gRng2High) >> 16;
}

void UNUSED CopyHalfwordsBackward(u16 *dest, const u16 *src, u32 size)
{
    s32 n = (size + 1) / 2;

    while (n-- != 0)
        dest[n] = src[n];
}

void UNUSED CopyWordsBackward(u32 *dest, const u32 *src, u32 size)
{
    s32 n = (size + 3) / 4;

    while (n-- != 0)
        dest[n] = src[n];
}

void UNUSED Fill16(void *dst, u16 value, u32 size)
{
    u16 *dest = dst;
    u32 rounded = size + 1;
    s32 n;

    // Keep both arms: agbcc coalesces them after allocating the rounding temporary.
    if (value)
        n = rounded / 2;
    else
        n = rounded / 2;
    while (n--)
        *dest++ = value;
}

void UNUSED Fill32(u32 *dest, u32 value, u32 size)
{
    u32 rounded = size + 3;
    s32 n;

    // Keep both arms: agbcc coalesces them after allocating the rounding temporary.
    if (value)
        n = rounded / 4;
    else
        n = rounded / 4;
    while (n--)
        *dest++ = value;
}

// Decompresses (or copies) data with a BIOS compression header into
// WRAM, and returns its size.
u32 UNUSED DecompressToWram(void *dest, const u8 *src)
{
    u32 size = (src[1] << 16) | (src[2] << 8) | src[3];

    switch (src[0] & COMP_TYPE_MASK)
    {
    case COMP_TYPE_LZ77:
        LZ77UnCompWram(src, dest);
        break;
    case COMP_TYPE_HUFF:
        HuffUnComp(src, dest);
        break;
    case COMP_TYPE_RL:
        RLUnCompWram(src, dest);
        break;
    case COMP_TYPE_DIFF:
        if ((src[0] & 0xF) == 1)
            Diff8bitUnFilterWram(src, dest);
        else
            Diff16bitUnFilter(src, dest);
        break;
    default:
        CpuCopy16(src, dest, size);
        break;
    }
    return size;
}

// Same as DecompressToWram, but into VRAM.
u32 UNUSED DecompressToVram(void *dest, const u8 *src)
{
    u32 size = (src[1] << 16) | (src[2] << 8) | src[3];

    switch (src[0] & COMP_TYPE_MASK)
    {
    case COMP_TYPE_LZ77:
        LZ77UnCompVram(src, dest);
        break;
    case COMP_TYPE_HUFF:
        HuffUnComp(src, dest);
        break;
    case COMP_TYPE_RL:
        RLUnCompVram(src, dest);
        break;
    case COMP_TYPE_DIFF:
        if ((src[0] & 0xF) == 1)
            Diff8bitUnFilterVram(src, dest);
        else
            Diff16bitUnFilter(src, dest);
        break;
    default:
        CpuCopy16(src, dest, size);
        break;
    }
    return size;
}
