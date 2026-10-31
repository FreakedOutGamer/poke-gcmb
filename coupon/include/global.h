#ifndef GUARD_GLOBAL_H
#define GUARD_GLOBAL_H

#include "gba/gba.h"
#include "constants/global.h"

// coupon.bin's text is Japanese on every disc.
#define GAME_LANGUAGE LANGUAGE_JAPANESE

// Which program this is, for the shared code in ../common that differs
// between programs.
#define PROGRAM_COUPON 1

#define ARRAY_COUNT(array) (sizeof(array) / sizeof((array)[0]))

#endif // GUARD_GLOBAL_H
