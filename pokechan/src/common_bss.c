#include "global.h"
#include "rtc_time.h"

// The flash library's and the Berry Program Update's globals, defined here
// together because the image keeps them in this order (the link places this
// file's .bss at 0x0201A798). The section attribute makes agbcc emit them
// in order as ordinary .bss rather than as COMMON symbols, which the linker
// would reorder. No header declaring them is included, because agbcc drops
// the attribute from a definition that follows an extern declaration.

struct FlashType;

#define ORDERED_BSS __attribute__((section(".bss.common")))

u8 gFlashTimeoutFlag ORDERED_BSS;
u8 (*PollFlashStatus)(u8 *) ORDERED_BSS;
u16 (*WaitForFlashWrite)(u8 phase, u8 *addr, u8 lastData) ORDERED_BSS;
u16 (*ProgramFlashSector)(u16 sectorNum, void *src) ORDERED_BSS;
const struct FlashType *gFlash ORDERED_BSS;
u16 (*ProgramFlashByte)(u16 sectorNum, u32 offset, u8 data) ORDERED_BSS;
u16 gFlashNumRemainingBytes ORDERED_BSS;
u16 (*EraseFlashChip)(void) ORDERED_BSS;
u16 (*EraseFlashSector)(u16 sectorNum) ORDERED_BSS;
const u16 *gFlashMaxTime ORDERED_BSS;

u32 gUpdateSuccessful ORDERED_BSS;
u32 gGameVersion ORDERED_BSS;
struct Time gTimeSinceBerryUpdate ORDERED_BSS;
struct Time gRtcUTCTime ORDERED_BSS;

// Unused, but inside the area AgbMain clears (it ends at __bss_end).
u32 gUnusedBssWord ORDERED_BSS;
