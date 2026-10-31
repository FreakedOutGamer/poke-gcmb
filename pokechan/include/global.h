#ifndef GUARD_GLOBAL_H
#define GUARD_GLOBAL_H

#include "gba/gba.h"
#include "constants/global.h"

// pokechan.bin's own text is English.
#define GAME_LANGUAGE LANGUAGE_ENGLISH

// Which program this is, for the shared code in ../common that differs
// between programs.
#define PROGRAM_POKECHAN 1

#define ARRAY_COUNT(array) (sizeof(array) / sizeof((array)[0]))

#endif // GUARD_GLOBAL_H
