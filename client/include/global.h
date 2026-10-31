#ifndef GUARD_GLOBAL_H
#define GUARD_GLOBAL_H

#include "gba/gba.h"
#include "constants/global.h"

// REVISION comes from the Makefile: 0 for client.2003_1112.bin (the
// client_2003_1112 build), 1 for client.bin (the client build).
#ifndef REVISION
#error "REVISION must be defined by the Makefile"
#endif

// The client's own text is English.
#define GAME_LANGUAGE LANGUAGE_ENGLISH

// Which program this is, for the shared code in ../common that differs
// between programs.
#define PROGRAM_CLIENT 1

#define ARRAY_COUNT(array) (sizeof(array) / sizeof((array)[0]))

#endif // GUARD_GLOBAL_H
