#include "global.h"
#include "save.h"
#include "text.h"
#include "delivery.h"

// The SDK's OamData layout. It has the same bits as pret's struct OamData,
// but the matrix number is split into AffineParamNo_L, HFlip and VFlip, as
// this file clears them one at a time.
struct SdkOamData
{
    /*0x00*/ u32 y:8;
    /*0x01*/ u32 affineMode:2;
             u32 objMode:2;
             u32 mosaic:1;
             u32 bpp:1;
             u32 shape:2;
    /*0x02*/ u32 x:9;
             u32 matrixNumLow:3;
             u32 hFlip:1;
             u32 vFlip:1;
             u32 size:2;
    /*0x04*/ u16 tileNum:10;
             u16 priority:2;
             u16 paletteNum:4;
    /*0x06*/ u16 affineParam;
};

extern struct SdkOamData gOamBuffer[];
extern const s16 gSineTable[];
extern u8 gSaveIconAngle;
extern const u8 gJirachiSpriteGfx[];
extern const u8 gSaveIconGfx[];
extern const u16 gDeliverySpritePalettes[][16];
extern const u8 gText_ConnectCable[];
extern const u8 gText_ReceivedJirachi[];
#if REVISION == 1
extern u8 gBerryFixStatus;
extern const u8 gText_BerryProgramUpdated[];
extern const u8 gText_BerryProgramUpToDate[];
extern const u8 gText_BerryProgramUpdateFailed[];
#endif

void SetJirachiSpriteScale(s32 scale);

// Jirachi (64x64) and the spinning save icon (32x32), then their palettes.
void LoadDeliveryGfx(void)
{
    DmaCopy32(3, gJirachiSpriteGfx, OBJ_VRAM0, 0x800);
    DmaCopy32(3, gSaveIconGfx, OBJ_VRAM0 + 0x800, 0x200);
    DmaCopy32(3, gDeliverySpritePalettes, OBJ_PLTT, 0x40);
}

void ShowJirachiSprite(void)
{
    gOamBuffer[0].size = 3;
    gOamBuffer[0].x = 88;
    gOamBuffer[0].y = 48;
    gOamBuffer[0].tileNum = 0;
    gOamBuffer[0].paletteNum = 0;
    gOamBuffer[0].priority = 0;
    gOamBuffer[0].affineMode = ST_OAM_AFFINE_NORMAL;
    gOamBuffer[0].matrixNumLow = 0;
    gOamBuffer[0].hFlip = 0;
    gOamBuffer[0].vFlip = 0;
    SetJirachiSpriteScale(1);
}

// 1/x in 8.8 fixed point, like pokeemerald's Q_8_8_inv.
s16 Q_8_8_inv(s16 y)
{
    s32 x = 0x10000;

    if (y < 0)
        x = x / y;
    else
        x = -(x / -y);
    return x;
}

void SetJirachiSpriteScale(s32 scale)
{
    s16 a, b, c, d;
    s16 zero;
    s16 size = scale * 4 + 1;

    // The matrix lives on the stack.
    (void)&a, (void)&b, (void)&c, (void)&d;

    a = Q_8_8_inv(size);
    d = Q_8_8_inv(size);
    c = zero = 0;
    b = zero;
    gOamBuffer[0].affineParam = a;
    gOamBuffer[1].affineParam = b;
    gOamBuffer[2].affineParam = c;
    gOamBuffer[3].affineParam = d;
}

// Moves Jirachi along a parabola as it flies in.
void SetJirachiSpritePos(s32 step, s32 size)
{
    s32 x = step * 3;
    s32 y = (80 - x) * (x - 80) / 120 + 120;

    gOamBuffer[0].x = x - size / 2;
    gOamBuffer[0].y = y - size / 2;
}

#if REVISION == 0
// Declared to return a value but never sets one.
u8 AnimateJirachiEntrance(void)
{
    s32 i;

    for (i = 0; i < 40; i++)
    {
        SetJirachiSpriteScale((i << 6) / 40);
        SetJirachiSpritePos(i, 64);
        VBlankIntrWait();
    }
}
#else
// Declared to return a value but never sets one.
u8 AnimateJirachiEntrance(void)
{
    s32 i;

    for (i = 1; i < 40; i++)
    {
        SetJirachiSpriteScale((i << 6) / 40 + 1);
        SetJirachiSpritePos(i, 64);
        VBlankIntrWait();
    }
    gOamBuffer[0].affineMode = ST_OAM_AFFINE_OFF;
    VBlankIntrWait();
}
#endif

// Rotates the save icon and moves it along; called once per sector saved.
// The later revision takes the steps as arguments.
#if REVISION == 0
#define SAVE_ICON_X_STEP 18
#define SAVE_ICON_ANGLE_STEP 18
void UpdateSaveIcon(void)
#else
#define SAVE_ICON_X_STEP xStep
#define SAVE_ICON_ANGLE_STEP angleStep
void UpdateSaveIcon(s32 xStep, s32 angleStep)
#endif
{
    s16 a, b, c, d;

    a = gSineTable[gSaveIconAngle + 64];
    b = gSineTable[gSaveIconAngle];
    c = -gSineTable[gSaveIconAngle];
    d = gSineTable[gSaveIconAngle + 64];
    gOamBuffer[1].x += SAVE_ICON_X_STEP;
    gSaveIconAngle += SAVE_ICON_ANGLE_STEP;
    // The matrix lives on the stack; taking the addresses only here (rather
    // than before the assignments above) is what matches the original.
    gOamBuffer[4].affineParam = *&a;
    gOamBuffer[5].affineParam = *&b;
    gOamBuffer[6].affineParam = *&c;
    gOamBuffer[7].affineParam = *&d;
}

void InitSaveIcon(void)
{
    gOamBuffer[1].size = 2;
    gOamBuffer[1].shape = 0;
#if REVISION == 0
    gOamBuffer[1].x = 0;
#else
    gOamBuffer[1].x = -32; // starts just off the left edge
#endif
    gOamBuffer[1].y = 100;
    gOamBuffer[1].tileNum = 64;
    gOamBuffer[1].paletteNum = 1;
    gOamBuffer[1].priority = 0;
    gOamBuffer[1].affineMode = ST_OAM_AFFINE_NORMAL;
    gOamBuffer[1].matrixNumLow = 1;
    gOamBuffer[1].hFlip = 0;
    gOamBuffer[1].vFlip = 0;
    gOamBuffer[4].affineParam = 0x100;
    gOamBuffer[5].affineParam = 0;
    gOamBuffer[6].affineParam = 0;
    gOamBuffer[7].affineParam = 0x100;
}

// Writes the Jirachi into the connected game's save, animating the save icon
// meanwhile, then shows the result. Returns TryGiveJirachi's result.
s32 DeliverJirachi(void)
{
    s32 result;

    LoadDeliveryGfx();
    PrintStringCentered(gText_ConnectCable, 1, MESSAGE_BOX_Y, 27, MESSAGE_BOX_HEIGHT, 0);
    InitSaveIcon();
    result = TryGiveJirachi();
#if REVISION == 0
    if (result == DELIVERY_SUCCESS)
    {
        ShowJirachiSprite();
        AnimateJirachiEntrance();
        PrintStringCentered(gText_ReceivedJirachi, 1, MESSAGE_BOX_Y, 27, MESSAGE_BOX_HEIGHT, 0);
    }
    else
    {
        PrintErrorMessage(result);
    }
#else
    if (result != DELIVERY_SUCCESS)
    {
        // Keep the berry fix even though the Jirachi could not be given.
        if (gBerryFixStatus == BERRY_FIX_SAVED)
            WriteSaveSectorOrSlot(gSaveBlockChunks);
        PrintErrorMessage(result);
    }

    if (gBerryFixStatus == BERRY_FIX_UPDATED || gBerryFixStatus == BERRY_FIX_SAVED)
        PrintStringCentered(gText_BerryProgramUpdated, 5, 18, 24, 2, 0);
    else if (gBerryFixStatus == BERRY_FIX_NOT_NEEDED)
        PrintStringCentered(gText_BerryProgramUpToDate, 5, 18, 24, 2, 0);
    else if (gBerryFixStatus != BERRY_FIX_NOT_RUN)
        PrintStringCentered(gText_BerryProgramUpdateFailed, 5, 18, 24, 2, 0);
#endif
    return result;
}

#if REVISION == 1
// After a successful delivery, the save icon keeps spinning and slides
// across the screen numLaps times over numFrames frames.
void SlideSaveIcon(s32 numFrames, s32 numLaps)
{
    s32 i;
    s32 pos;
    struct SdkOamData *oam;

    InitSaveIcon();
    for (i = 0; i < numFrames; i++)
    {
        UpdateSaveIcon(0, 0);
        oam = gOamBuffer;
        pos = numLaps * 300 * i / numFrames % 300;
        oam[1].x = pos - 32;
        gSaveIconAngle = (pos << 8) / 300;
        VBlankIntrWait();
    }
}

void ShowReceivedJirachi(void)
{
    ShowJirachiSprite();
    AnimateJirachiEntrance();
    PrintStringCentered(gText_ReceivedJirachi, 1, MESSAGE_BOX_Y, 27, MESSAGE_BOX_HEIGHT, 0);
}
#endif
