#include "global.h"
#include "pokemon.h"

u16 UNUSED GetUnownLetterByPersonality(u32 personality)
{
    return (((personality & 0x3000000) >> 18)
          | ((personality & 0x30000) >> 12)
          | ((personality & 0x300) >> 6)
          | (personality & 0x3)) % UNOWN_FORM_COUNT;
}
