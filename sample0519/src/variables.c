#include "global.h"

// Initialized variables, as in pokechan.

extern IntrFunc gIntrTable[];

// Unused.
u8 gUnusedBuffer[0x100] = {0};

// The flash timeout timer's slot in gIntrTable, for InitFlashTimer.
IntrFunc *gFlashTimerIntrFunc = &gIntrTable[5];
