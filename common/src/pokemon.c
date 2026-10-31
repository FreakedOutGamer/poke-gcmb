#include "global.h"
#include "pokemon.h"
#include "box_crypto.h"
#include "libpmagb/random.h"


extern const struct SpeciesInfo gJirachiBaseStats;
extern const u32 gExperienceTables[][101];
extern const u8 gGameLanguage;

void ZeroBoxMonData(struct BoxPokemon *boxMon)
{
    u8 *raw = (u8 *)boxMon;
    u32 i;

    for (i = 0; i < sizeof(struct BoxPokemon); i++)
        raw[i] = 0;
}

void ZeroMonData(struct Pokemon *mon)
{
    u32 arg;

    ZeroBoxMonData(&mon->box);
    arg = 0;
    SetMonData(mon, MON_DATA_STATUS, &arg);
    SetMonData(mon, MON_DATA_LEVEL, &arg);
    SetMonData(mon, MON_DATA_HP, &arg);
    SetMonData(mon, MON_DATA_MAX_HP, &arg);
    SetMonData(mon, MON_DATA_ATK, &arg);
    SetMonData(mon, MON_DATA_DEF, &arg);
    SetMonData(mon, MON_DATA_SPEED, &arg);
    SetMonData(mon, MON_DATA_SPATK, &arg);
    SetMonData(mon, MON_DATA_SPDEF, &arg);
    arg = 0xFF;
    SetMonData(mon, MON_DATA_MAIL, &arg);
}

void CreateMon(struct Pokemon *mon, u16 species, u8 level, u8 fixedIV,
               u8 hasFixedPersonality, u32 fixedPersonality, u8 otIdType,
               u32 fixedOtId)
{
    u32 arg;

    ZeroMonData(mon);
    CreateBoxMon(&mon->box, species, level, fixedIV, hasFixedPersonality,
                 fixedPersonality, otIdType, fixedOtId);
    SetMonData(mon, MON_DATA_LEVEL, &level);
    arg = 0xFF;
    SetMonData(mon, MON_DATA_MAIL, &arg);
    CalculateMonStats(mon);
}

