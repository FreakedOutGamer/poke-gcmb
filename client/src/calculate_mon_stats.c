#include "global.h"
#include "pokemon.h"
#include "constants/species.h"


extern const struct SpeciesInfo gJirachiBaseStats[];

#define CALC_STAT(base, iv, ev, statIndex, field)               \
{                                                               \
    u8 baseStat = gJirachiBaseStats[0].base;                           \
    s32 n = (((2 * baseStat + iv + ev / 4) * level) / 100) + 5; \
    u8 nature = GetNature(mon);                                 \
    n = ModifyStatByNature(nature, n, statIndex);                  \
    SetMonData(mon, field, &n);                                 \
}

void CalculateMonStats(struct Pokemon *mon)
{
    s32 oldMaxHP = GetMonData(mon, MON_DATA_MAX_HP, 0);
    s32 currentHP = GetMonData(mon, MON_DATA_HP, 0);
    s32 hpIV = GetMonData(mon, MON_DATA_HP_IV, 0);
    s32 hpEV = GetMonData(mon, MON_DATA_HP_EV, 0);
    s32 attackIV = GetMonData(mon, MON_DATA_ATK_IV, 0);
    s32 attackEV = GetMonData(mon, MON_DATA_ATK_EV, 0);
    s32 defenseIV = GetMonData(mon, MON_DATA_DEF_IV, 0);
    s32 defenseEV = GetMonData(mon, MON_DATA_DEF_EV, 0);
    s32 speedIV = GetMonData(mon, MON_DATA_SPEED_IV, 0);
    s32 speedEV = GetMonData(mon, MON_DATA_SPEED_EV, 0);
    s32 spAttackIV = GetMonData(mon, MON_DATA_SPATK_IV, 0);
    s32 spAttackEV = GetMonData(mon, MON_DATA_SPATK_EV, 0);
    s32 spDefenseIV = GetMonData(mon, MON_DATA_SPDEF_IV, 0);
    s32 spDefenseEV = GetMonData(mon, MON_DATA_SPDEF_EV, 0);
    u16 species = GetMonData(mon, MON_DATA_SPECIES, 0);
    s32 level = GetLevelFromMonExp(mon);
    s32 newMaxHP;

    SetMonData(mon, MON_DATA_LEVEL, &level);
    if (species == SPECIES_SHEDINJA)
        newMaxHP = 1;
    else
    {
        s32 n = 2 * gJirachiBaseStats[0].baseHP + hpIV;
        newMaxHP = (((n + hpEV / 4) * level) / 100) + level + 10;
    }

    SetMonData(mon, MON_DATA_MAX_HP, &newMaxHP);
    CALC_STAT(baseAttack, attackIV, attackEV, 1, MON_DATA_ATK)
    CALC_STAT(baseDefense, defenseIV, defenseEV, 2, MON_DATA_DEF)
    CALC_STAT(baseSpeed, speedIV, speedEV, 3, MON_DATA_SPEED)
    CALC_STAT(baseSpAttack, spAttackIV, spAttackEV, 4, MON_DATA_SPATK)
    CALC_STAT(baseSpDefense, spDefenseIV, spDefenseEV, 5, MON_DATA_SPDEF)

    if (species == SPECIES_SHEDINJA)
    {
        if (currentHP != 0 || oldMaxHP == 0)
            currentHP = 1;
        else
            return;
    }
    else
    {
        if (currentHP == 0 && oldMaxHP == 0)
            currentHP = newMaxHP;
        else if (currentHP != 0)
            currentHP += newMaxHP - oldMaxHP;
        else
            return;
    }
    SetMonData(mon, MON_DATA_HP, &currentHP);
}
