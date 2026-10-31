#include "global.h"
#include "delivery.h"
#include "joybus.h"
#include "main.h"
#include "text.h"

extern u32 IntrMain[];
extern u32 IntrMain_Buffer[0x80];
extern u16 gTilemap[];
extern struct OamData gOamBuffer[128];
extern u16 gHeldKeys;
extern u16 gNewKeys;
extern const u16 gBgTilemap[];
extern const u16 gBgPalettes[];
extern const u32 gBgTiles[];

// BUG: 15-17 look like rows, but they are passed as the palette, so every
// message lands on row 1 over the previous one, and 16 and 17 overflow the
// 4-bit palette field (becoming palettes 0 and 1).
#if REVISION == 0
#define PRINT_PROGRESS(str, arg) PrintString(str, 0, 1, arg)
#else
#define PRINT_PROGRESS(str, arg)
#endif

void AgbMain(void)
{
    s32 i;
    u32 result;

    RegisterRamReset(RESET_IWRAM);
    InitIntrTable();
    DmaCopy32(3, IntrMain, IntrMain_Buffer, sizeof(IntrMain_Buffer));
    INTR_VECTOR = IntrMain_Buffer;

    DmaCopy32(3, gBgTiles, BG_CHAR_ADDR(2), 0x1000);
    DmaCopy32(3, gBgPalettes, BG_PLTT, BG_PLTT_SIZE);
    DmaCopy32(3, gBgPalettes, OBJ_PLTT, BG_PLTT_SIZE);
    DmaFill32(3, 0, gTilemap, 32 * 20 * sizeof(u16));
    DmaCopy32(3, gBgTilemap, BG_SCREEN_ADDR(4), 32 * 20 * sizeof(u16));

    for (i = 0; i < 128; i++)
        gOamBuffer[i].affineMode = ST_OAM_AFFINE_ERASE;
    DmaCopy32(3, gOamBuffer, OAM, OAM_SIZE);

    REG_BG0CNT = BGCNT_PRIORITY(0) | BGCNT_CHARBASE(2) | BGCNT_MOSAIC | BGCNT_SCREENBASE(0);
    REG_BG1CNT = BGCNT_PRIORITY(1) | BGCNT_CHARBASE(2) | BGCNT_MOSAIC | BGCNT_SCREENBASE(4);
    REG_DISPCNT = DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG1_ON | DISPCNT_OBJ_ON;

    REG_IE = INTR_FLAG_VBLANK;
    // Only take the cartridge interrupt if a valid "AGBJ" cartridge is inserted.
    if (*(u8 *)0x080000B2 == 0x96 && *(u32 *)0x080000AC == *(u32 *)"AGBJ")
        REG_IE |= INTR_FLAG_GAMEPAK;
    REG_DISPSTAT = DISPSTAT_VBLANK_INTR;
    REG_IME = 1;

    PRINT_PROGRESS("start handshake", 15);
    VBlankIntrWait();
    JoyBusHandshake();
    PRINT_PROGRESS("finish handshake", 16);
    VBlankIntrWait();
    result = DeliverJirachi();
    PRINT_PROGRESS("finish save", 17);
    VBlankIntrWait();

    switch (result)
    {
    case DELIVERY_SUCCESS:
        JoyBusSendResult(0x1000);
        break;
    case DELIVERY_ERROR_NO_SAVE_DATA_UNUSED:
        JoyBusSendResult(0x2);
        break;
    case DELIVERY_ERROR_SAVE_CORRUPTED:
        JoyBusSendResult(0x20);
        break;
    case DELIVERY_ERROR_NOT_POKEMON_CARTRIDGE:
        JoyBusSendResult(0x1);
        break;
    case DELIVERY_ERROR_NO_SAVE_DATA:
        JoyBusSendResult(0x4);
        break;
    case DELIVERY_ERROR_SAVE_FAILED:
        JoyBusSendResult(0x2000);
        break;
    case DELIVERY_ERROR_PARTY_FULL:
        JoyBusSendResult(0x10);
        break;
    case DELIVERY_ERROR_NO_FLASH:
        JoyBusSendResult(0x80);
        break;
    case DELIVERY_ERROR_ALREADY_DELIVERED:
        JoyBusSendResult(0x4000);
        break;
#if REVISION == 0
    // BUG: No case for DELIVERY_ERROR_PROCEED_GAME: the GBA shows its
    // message, but the GameCube is never told and gives up waiting.
    case DELIVERY_ERROR_UNSUPPORTED_VERSION:
        JoyBusSendResult(0x8000);
        break;
#else
    case DELIVERY_ERROR_PROCEED_GAME:
        JoyBusSendResult(0x8);
        break;
    case DELIVERY_ERROR_UNSUPPORTED_VERSION:
        JoyBusSendResult(0x1);
        break;
#endif
    }

#if REVISION == 1
    if (result == DELIVERY_SUCCESS)
    {
        // About 34 seconds of the save icon crossing the screen, then Jirachi.
        SlideSaveIcon(2062, 10);
        ShowReceivedJirachi();
    }
#endif

    SoftResetRom();
}

void VBlankIntr(void)
{
    // Only the 20 visible rows of the 32-tile-wide tilemap are copied.
    DmaCopy32(3, gTilemap, BG_VRAM, 32 * 20 * sizeof(u16));
    DmaCopy32(3, gOamBuffer, OAM, OAM_SIZE);
    INTR_CHECK = INTR_FLAG_VBLANK;
}

void IntrDummy(void)
{
}

void ReadKeys(void)
{
    u16 keyInput = REG_KEYINPUT ^ KEYS_MASK;

    gNewKeys = keyInput & ~gHeldKeys;
    gHeldKeys = keyInput;
}
