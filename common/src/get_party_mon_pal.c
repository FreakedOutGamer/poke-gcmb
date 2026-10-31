#include "global.h"
#include "libpmagb/save_block_pointers.h"
#include "constants/species.h"
#include "pokemon.h"
#include "gflib/sprite.h"

//Extracts the upper 16 bits of a 32-bit number
#define HIHALF(n) (((n) & 0xFFFF0000) >> 16)

//Extracts the lower 16 bits of a 32-bit number
#define LOHALF(n) ((n) & 0xFFFF)

static inline u32 GetMonDataInline(struct Pokemon *mon, s32 attr, u8 *strbuf)
{
    switch (attr)
    {
    case MON_DATA_STATUS:
        return mon->status;
    case MON_DATA_LEVEL:
        return mon->level;
    case MON_DATA_HP:
        return mon->hp;
    case MON_DATA_MAX_HP:
        return mon->maxHP;
    case MON_DATA_ATK:
        return mon->attack;
    case MON_DATA_DEF:
        return mon->defense;
    case MON_DATA_SPEED:
        return mon->speed;
    case MON_DATA_SPATK:
        return mon->spAttack;
    case MON_DATA_SPDEF:
        return mon->spDefense;
    case MON_DATA_MAIL:
        return mon->mail;
    default:
        return GetBoxMonData(&mon->box, attr, strbuf);
    }
}

const struct CompressedSpritePalette *GetPartyMonSpritePalStruct(u32 partyId)
{
    u32 shinyValue = 0;
    u16 species = GetMonDataInline(&gPlayerPartyPtr[partyId], MON_DATA_SPECIES_OR_EGG, NULL);
    u32 otId = GetMonDataInline(&gPlayerPartyPtr[partyId], MON_DATA_OT_ID, NULL);
    u32 personality = GetMonDataInline(&gPlayerPartyPtr[partyId], MON_DATA_PERSONALITY, NULL);

    if (species > NUM_SPECIES)
        return &gAgbPmRomParams->monNormalPalettes[SPECIES_NONE];

    shinyValue = HIHALF(otId) ^ LOHALF(otId) ^ HIHALF(personality) ^ LOHALF(personality);

    if (shinyValue > 7)
        return &gAgbPmRomParams->monNormalPalettes[species];
    else
        return &gAgbPmRomParams->monShinyPalettes[species];
}
