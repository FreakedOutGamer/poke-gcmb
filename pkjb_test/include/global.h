#ifndef GUARD_GLOBAL_H
#define GUARD_GLOBAL_H

#include "gba/gba.h"
#include "constants/global.h"

// pkjb_test.bin's screen is in Japanese.
#define GAME_LANGUAGE LANGUAGE_JAPANESE

// Which program this is, for the shared code in ../common that differs
// between programs.
#define PROGRAM_PKJB_TEST 1

#endif // GUARD_GLOBAL_H
