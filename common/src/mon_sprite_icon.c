#include "global.h"
#include "gf_rom_header.h"
#include "constants/species.h"
#include "gflib/sprite.h"


















//Extracts the upper 16 bits of a 32-bit number
#define HIHALF(n) (((n) & 0xFFFF0000) >> 16)

//Extracts the lower 16 bits of a 32-bit number
#define LOHALF(n) ((n) & 0xFFFF)

const struct CompressedSpritePalette *GetMonSpritePalStructFromOtIdPersonality(u16 species, u32 otId, u32 personality)
{
    u32 shinyValue = 0;

    if (species > NUM_SPECIES)
        return &gAgbPmRomParams->monNormalPalettes[SPECIES_NONE];

    shinyValue = HIHALF(otId) ^ LOHALF(otId) ^ HIHALF(personality) ^ LOHALF(personality);

    if (shinyValue <= 7)
        return &gAgbPmRomParams->monShinyPalettes[species];
    else
        return &gAgbPmRomParams->monNormalPalettes[species];
}

// colbtl also has this out of line (a non-static inline); coupon and pokechan
// keep theirs in src/pokemon_icon.c.
#ifdef PROGRAM_COLBTL
inline u16 GetUnownLetterByPersonality(u32 personality)
#else
static inline u16 GetUnownLetterByPersonalityInline(u32 personality)
#endif
{
    return (((personality & 0x3000000) >> 18) | ((personality & 0x30000) >> 12) | ((personality & 0x300) >> 6) | (personality & 0x3)) % 0x1C;
}

u16 GetIconSpecies(u16 species, u32 personality)
{
    u16 result;

    if (species == SPECIES_UNOWN)
    {
#ifdef PROGRAM_COLBTL
        u16 letter = GetUnownLetterByPersonality(personality);
#else
        u16 letter = GetUnownLetterByPersonalityInline(personality);
#endif
        if (letter == 0)
            letter = SPECIES_UNOWN;
        else
            letter += (SPECIES_UNOWN_B - 1);
        result = letter;
    }
    else
    {
        if (species > SPECIES_EGG)
            result = INVALID_ICON_SPECIES;
        else
            result = species;
    }

    return result;
}
