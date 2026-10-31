#ifndef GUARD_GLOBAL_H
#define GUARD_GLOBAL_H

#include "gba/gba.h"
#include "constants/global.h"

// sample0519.bin's text is Japanese.
#define GAME_LANGUAGE LANGUAGE_JAPANESE

// Which program this is, for the shared code in ../common that differs
// between programs.
#define PROGRAM_SAMPLE0519 1

#define ARRAY_COUNT(array) (sizeof(array) / sizeof((array)[0]))

typedef void (*IntrFunc)(void);

#endif // GUARD_GLOBAL_H
