#include "global.h"
#include "pokemon.h"

u8 GetNature(struct Pokemon *mon)
{
    return GetMonData(mon, MON_DATA_PERSONALITY, 0) % NUM_NATURES;
}

#ifdef PROGRAM_SAMPLE0519
u8 UNUSED GetNatureFromPersonality(u32 personality)
{
    return personality % NUM_NATURES;
}
#endif
