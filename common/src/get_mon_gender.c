#include "global.h"
#include "libpmagb/save_block_pointers.h"
#include "pokemon.h"

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

// Same as GetBoxMonGender, except GetGenderFromSpeciesAndPersonality isn't called but copied.
static UNUSED u8 GetMonGender(struct Pokemon *mon)
{
    const struct SpeciesInfo *speciesInfo = gAgbPmRomParams->speciesInfo;
    u16 species = GetMonDataInline(mon, MON_DATA_SPECIES, NULL);
    u32 personality = GetMonDataInline(mon, MON_DATA_PERSONALITY, NULL);

    switch (speciesInfo[species].genderRatio)
    {
    case MON_MALE:
    case MON_FEMALE:
    case MON_GENDERLESS:
        return speciesInfo[species].genderRatio;
    }

    if (speciesInfo[species].genderRatio > (personality & 0xFF))
        return MON_FEMALE;
    else
        return MON_MALE;
}
