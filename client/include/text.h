#ifndef GUARD_TEXT_H
#define GUARD_TEXT_H

// The centered message box. The later revision rewords its messages and
// gives them a taller box.
#if REVISION == 0
#define MESSAGE_BOX_Y      3
#define MESSAGE_BOX_HEIGHT 6
#else
#define MESSAGE_BOX_Y      2
#define MESSAGE_BOX_HEIGHT 7
#endif

// The payload's strings are ASCII apart from this.
#define CHAR_e_ACUTE 0x1B

void PrintString(const u8 *str, s32 x, s32 y, u32 palette);
void PrintStringCentered(const u8 *str, s32 x, s32 y, s32 width, s32 height, u32 palette);
void PrintErrorMessage(u32 error);

// The earlier revision prints the valid-sector bits of a damaged slot.
void PrintHex(u32 value, s32 x, s32 y, u32 palette, s32 numBytes);
s32 SaveStatusToDeliveryError(s32 status);

#endif // GUARD_TEXT_H
