#include "global.h"
#include "save.h"

// The payload's initialized variables, which the image carries at their
// run-time addresses.

typedef void (*IntrFunc)(void);
union PokemonSubstruct;

extern IntrFunc gIntrTable[];

// Unused.
u8 gUnusedBuffer[0x100] = {0};

// GetSubstruct's working pointers, globals in the original.
union PokemonSubstruct *substructs0 = NULL;
union PokemonSubstruct *substructs1 = NULL;
union PokemonSubstruct *substructs2 = NULL;
union PokemonSubstruct *substructs3 = NULL;
union PokemonSubstruct *substructs4 = NULL;
union PokemonSubstruct *substructs5 = NULL;
union PokemonSubstruct *substructs6 = NULL;
union PokemonSubstruct *substructs7 = NULL;
union PokemonSubstruct *substructs8 = NULL;
union PokemonSubstruct *substructs9 = NULL;
union PokemonSubstruct *substructs10 = NULL;
union PokemonSubstruct *substructs11 = NULL;
union PokemonSubstruct *substructs12 = NULL;
union PokemonSubstruct *substructs13 = NULL;
union PokemonSubstruct *substructs14 = NULL;
union PokemonSubstruct *substructs15 = NULL;
union PokemonSubstruct *substructs16 = NULL;
union PokemonSubstruct *substructs17 = NULL;
union PokemonSubstruct *substructs18 = NULL;
union PokemonSubstruct *substructs19 = NULL;
union PokemonSubstruct *substructs20 = NULL;
union PokemonSubstruct *substructs21 = NULL;
union PokemonSubstruct *substructs22 = NULL;
union PokemonSubstruct *substructs23 = NULL;

// Where each save sector's data lives in RAM: SaveBlock2, then SaveBlock1
// across four sectors. The other sectors are read but not kept.
u8 *gSaveBlockChunkData[NUM_SECTORS_PER_SAVE_SLOT] =
{
    gSaveBlock2Buffer,
    gSaveBlock1Buffer,
    gSaveBlock1Buffer + SECTOR_DATA_SIZE,
    gSaveBlock1Buffer + SECTOR_DATA_SIZE * 2,
    gSaveBlock1Buffer + SECTOR_DATA_SIZE * 3,
};

// The flash timeout timer's slot in gIntrTable (timer 2).
IntrFunc *gSaveTimerIntrSlotPtr = &gIntrTable[9];
