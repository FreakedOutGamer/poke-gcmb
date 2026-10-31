#include "global.h"

struct FlashSector
{
    u32 size;
    u8 shift;
    u16 count;
    u16 top;
};

struct FlashType
{
    u32 romSize;
    struct FlashSector sector;
    u16 wait[2];
    union
    {
        struct
        {
            u8 makerId;
            u8 deviceId;
        } separate;
        u16 joined;
    } ids;
};

/* This 2003 SDK layout predates the ProgramFlashByte member. */
struct FlashSetupInfo
{
    u16 (*programFlashSector)(u16, void *);
    u16 (*eraseFlashChip)(void);
    u16 (*eraseFlashSector)(u16);
    u16 (*waitForFlashWrite)(u8, u8 *, u8);
    const u16 *maxTime;
    struct FlashType type;
};

#define FLASH_BASE ((u8 *)0x0E000000)
#define FLASH_WRITE(address, value) \
    (*(volatile u8 *)(FLASH_BASE + (address)) = (value))

extern const struct FlashSetupInfo *const sSetupInfos[];
extern u16 ReadFlashId(void);
extern u16 (*ProgramFlashSector)(u16, void *);
extern u16 (*EraseFlashChip)(void);
extern u16 (*EraseFlashSector)(u16);
extern u16 (*WaitForFlashWrite)(u8, u8 *, u8);
extern const u16 *gFlashMaxTime;
extern const struct FlashType *gFlash;
extern u8 (*PollFlashStatus)(u8 *);
extern u8 gFlashTimeoutFlag;
extern void StartFlashTimer(u8 phase);
extern void StopFlashTimer(void);

u16 IdentifyFlash(void)
{
    u16 result;
    u16 flashId;
    const struct FlashSetupInfo *const *setupInfo;

    REG_WAITCNT = (REG_WAITCNT & ~WAITCNT_SRAM_MASK) | WAITCNT_SRAM_8;
    flashId = ReadFlashId();
    setupInfo = sSetupInfos;
    result = 1;

    for (;;)
    {
        if ((*setupInfo)->type.ids.separate.makerId == 0)
            break;
        if (flashId == (*setupInfo)->type.ids.joined)
        {
            result = 0;
            break;
        }
        setupInfo++;
    }

    ProgramFlashSector = (*setupInfo)->programFlashSector;
    EraseFlashChip = (*setupInfo)->eraseFlashChip;
    EraseFlashSector = (*setupInfo)->eraseFlashSector;
    WaitForFlashWrite = (*setupInfo)->waitForFlashWrite;
    gFlashMaxTime = (*setupInfo)->maxTime;
    gFlash = &(*setupInfo)->type;
    return result;
}

u16 WaitForFlashWrite_Common(u8 phase, u8 *addr, u8 lastData)
{
    u16 result = 0;
    u8 status;

    StartFlashTimer(phase);

    while ((status = PollFlashStatus(addr)) != lastData)
    {
        if (status & 0x20)
        {
            if (PollFlashStatus(addr) == lastData)
                break;

            FLASH_WRITE(0x5555, 0xF0);
            result = phase | 0xA000u;
            break;
        }

        if (gFlashTimeoutFlag)
        {
            if (PollFlashStatus(addr) == lastData)
                break;

            FLASH_WRITE(0x5555, 0xF0);
            result = phase | 0xC000u;
            break;
        }
    }

    StopFlashTimer();
    return result;
}

