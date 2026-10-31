#include "global.h"

typedef void (*IntrFunc)(void);
extern IntrFunc gIntrTable[];

// Unused work area retained by the original image.
u8 gUnusedWorkArea[0x100] = {0};

// InitFlash installs the flash timer handler through interrupt slot 6.
IntrFunc *gSaveTimerIntrSlotPtr = &gIntrTable[6];
