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

u32 GetMonType(struct Pokemon *mon, u32 which)
{
    const struct SpeciesInfo *speciesInfo = gAgbPmRomParams->speciesInfo;
    u32 species = GetMonDataInline(mon, MON_DATA_SPECIES, NULL);

    switch (which)
    {
    default:
        return 0;
    case 1:
        return speciesInfo[species].types[0];
    case 2:
        return speciesInfo[species].types[1];
    }
}

u32 GetTypePaletteNum(u32 id)
{
    return gTypeToPaletteNumber[id];
}

const u8 *GetAbilityName(u32 ability)
{
    const u8 (*abilityNames)[][(GAME_LANGUAGE == LANGUAGE_JAPANESE ? 7 : ABILITY_NAME_LENGTH) + 1] = (const u8 (*)[][(GAME_LANGUAGE == LANGUAGE_JAPANESE ? 7 : ABILITY_NAME_LENGTH) + 1])gAgbPmRomParams->abilityNames;

    return (*abilityNames)[ability];
}

const u8 *GetAbilityDescription(u32 ability)
{
    const u8 **desriptions = (const u8 **)gAgbPmRomParams->abilityDescriptions;

    if (GAME_LANGUAGE == LANGUAGE_JAPANESE)
        return (const u8 *)desriptions + 19 * ability;
    return desriptions[ability];
}

s32 CalculatePPWithBonus(u32 move, s32 ppBonuses, u32 moveIndex)
{
    const struct BattleMove *moves = gAgbPmRomParams->moves;
    s32 basePP = moves[move].pp;

    return basePP + ((basePP * 20 * ((gPPUpGetMask[moveIndex] & ppBonuses) >> (2 * moveIndex))) / 100);
}
