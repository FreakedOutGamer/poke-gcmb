#include "global.h"

// Initialized variables the image carries between its tables.

typedef void (*IntrFunc)(void);

extern IntrFunc gIntrTable[];

// Unused.
u8 gUnusedBuffer[0x100] = {0};

// The flash timeout timer's slot in gIntrTable, for InitFlash.
IntrFunc *gSaveTimerIntrSlotPtr = &gIntrTable[6];
