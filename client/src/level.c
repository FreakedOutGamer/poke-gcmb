#include "global.h"
#include "pokemon.h"

extern const struct SpeciesInfo gJirachiBaseStats[];
extern const u32 gExperienceTables[][101];

u8 GetLevelFromMonExp(struct Pokemon *mon)
{
    u16 species = GetMonData(mon, 11, 0);
    u32 exp = GetMonData(mon, 25, 0);
    s32 level = 1;
    if (gExperienceTables[gJirachiBaseStats[0].growthRate][level] <= exp)
    {
        do
        {
            level++;
            if (level > 100)
                break;
        } while (gExperienceTables[gJirachiBaseStats[0].growthRate][level] <= exp);
    }
    return level - 1;
}

u8 GetLevelFromBoxMonExp(struct BoxPokemon *boxMon)
{
    u16 species = GetBoxMonData(boxMon, 11, 0);
    u32 exp = GetBoxMonData(boxMon, 25, 0);
    s32 level = 1;
    if (gExperienceTables[gJirachiBaseStats[0].growthRate][level] <= exp)
    {
        do
        {
            level++;
            if (level > 100)
                break;
        } while (gExperienceTables[gJirachiBaseStats[0].growthRate][level] <= exp);
    }
    return level - 1;
}
