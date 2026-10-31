#include "global.h"
#include "pokemon.h"
#include "box_crypto.h"
#include "libpmagb/random.h"

#define Random32() (Random() | (Random() << 16))
#define HIHALF(n) (((n) & 0xFFFF0000) >> 16)
#define LOHALF(n) ((n) & 0xFFFF)

extern const struct SpeciesInfo gJirachiBaseStats[];
extern const u32 gExperienceTables[][101];
extern const u8 gGameLanguage;
extern const u8 gGameVersion;

void CreateBoxMon(struct BoxPokemon *boxMon, u16 species, u8 level,
                  u8 fixedIV, u8 hasFixedPersonality, u32 fixedPersonality,
                  u8 otIdType, u32 fixedOtId)
{
    u8 speciesName[11];
    u32 personality;
    u32 value;
    u32 growthRate;
    register u32 expOffset asm("r2");
    register u32 levelOffset asm("r0");
    u16 checksum;

    ZeroBoxMonData(boxMon);
    if (hasFixedPersonality)
        personality = fixedPersonality;
    else
        personality = Random32();
    SetBoxMonData(boxMon, MON_DATA_PERSONALITY, &personality);

    if (otIdType == 2)
    {
        u32 shinyValue;
        do
        {
            value = Random32();
            shinyValue = HIHALF(value) ^ LOHALF(value)
                       ^ HIHALF(personality) ^ LOHALF(personality);
        } while (shinyValue < 8);
    }
    else if (otIdType == 1)
        value = fixedOtId;
    else
        value = 0;

    SetBoxMonData(boxMon, MON_DATA_OT_ID, &value);
    checksum = CalculateBoxMonChecksum(boxMon);
    SetBoxMonData(boxMon, MON_DATA_CHECKSUM, &checksum);
    EncryptBoxMon(boxMon);
    GetSpeciesName(speciesName, species);
    SetBoxMonData(boxMon, MON_DATA_NICKNAME, speciesName);
    SetBoxMonData(boxMon, MON_DATA_LANGUAGE, &gGameLanguage);
    SetBoxMonData(boxMon, MON_DATA_SPECIES, &species);
    // TODO: remove the register pinning below. pokeruby's
    // &gExperienceTables[gJirachiBaseStats[0].growthRate][level] (with or without a
    // temporary) gets within a few instructions but allocates the level
    // load differently; the permuter has not found an asm-free version yet.
    growthRate = gJirachiBaseStats[0].growthRate;
    asm("" : : "r"(growthRate));
    expOffset = growthRate * 404;
    asm("" : "+r"(expOffset));
    levelOffset = *(volatile u8 *)&level;
    levelOffset <<= 2;
    asm("" : "+r"(levelOffset));
    SetBoxMonData(boxMon, MON_DATA_EXP,
                  (const u8 *)gExperienceTables + expOffset + levelOffset);
    SetBoxMonData(boxMon, MON_DATA_FRIENDSHIP, &gJirachiBaseStats[0].friendship);
    value = 0xFF;
    SetBoxMonData(boxMon, MON_DATA_MET_LOCATION, &value);
    SetBoxMonData(boxMon, MON_DATA_MET_LEVEL, &level);
#if REVISION == 1
    SetBoxMonData(boxMon, MON_DATA_MET_GAME, &gGameVersion);
#endif
    value = 4;
    SetBoxMonData(boxMon, MON_DATA_POKEBALL, &value);

    if (*(volatile u8 *)&fixedIV < 32)
    {
        SetBoxMonData(boxMon, MON_DATA_HP_IV, &fixedIV);
        SetBoxMonData(boxMon, MON_DATA_ATK_IV, &fixedIV);
        SetBoxMonData(boxMon, MON_DATA_DEF_IV, &fixedIV);
        SetBoxMonData(boxMon, MON_DATA_SPEED_IV, &fixedIV);
        SetBoxMonData(boxMon, MON_DATA_SPATK_IV, &fixedIV);
        SetBoxMonData(boxMon, MON_DATA_SPDEF_IV, &fixedIV);
    }
    else
    {
        u32 iv;
        value = Random();
        iv = value & 0x1F;
        SetBoxMonData(boxMon, MON_DATA_HP_IV, &iv);
        iv = (value & 0x3E0) >> 5;
        SetBoxMonData(boxMon, MON_DATA_ATK_IV, &iv);
        iv = (value & 0x7C00) >> 10;
        SetBoxMonData(boxMon, MON_DATA_DEF_IV, &iv);
        value = Random();
        iv = value & 0x1F;
        SetBoxMonData(boxMon, MON_DATA_SPEED_IV, &iv);
        iv = (value & 0x3E0) >> 5;
        SetBoxMonData(boxMon, MON_DATA_SPATK_IV, &iv);
        iv = (value & 0x7C00) >> 10;
        SetBoxMonData(boxMon, MON_DATA_SPDEF_IV, &iv);
    }

    if (gJirachiBaseStats[0].abilities[1])
    {
        value = personality & 1;
        SetBoxMonData(boxMon, MON_DATA_ABILITY_NUM, &value);
    }
    GiveBoxMonInitialMoveset(boxMon);
}

