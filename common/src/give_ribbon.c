#include "global.h"
#include "flag.h"
#include "libpmagb/save_block_pointers.h"
#include "pokemon.h"

extern const u8 gGiftRibbonMonDataIds[7];

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

void GiveGiftRibbonToParty(s32 index_, s32 ribbonId_)
{
    s32 i;
    u8 arr[7];
    // Callers pass full words without narrowing them, so the parameters are
    // s32; the function narrows them to u8 itself.
    u8 index = index_;
    u8 ribbonId = ribbonId_;
    bool32 gotRibbon = FALSE;
#ifdef PROGRAM_COUPON
    u8 hasRibbon = TRUE;
#endif

    memcpy(arr, gGiftRibbonMonDataIds, 7);
    /*
    u8 arr[] = {
        MON_DATA_MARINE_RIBBON,
        MON_DATA_LAND_RIBBON,
        MON_DATA_SKY_RIBBON,
        MON_DATA_COUNTRY_RIBBON,
        MON_DATA_NATIONAL_RIBBON,
        MON_DATA_EARTH_RIBBON,
        MON_DATA_WORLD_RIBBON,
    };
    */

    if (index < 11 && ribbonId < 65 && index < 7)
    {
        gGiftRibbonsPtr[index] = ribbonId;

        for (i = 0; i < PARTY_SIZE; i++)
        {
            if (GetMonDataInline(&gPlayerPartyPtr[i], MON_DATA_SPECIES, NULL) != 0 && GetMonDataInline(&gPlayerPartyPtr[i], MON_DATA_SANITY_IS_EGG, NULL) == 0)
            {
                // Only coupon.bin puts the ribbon on the Pokémon; the others
                // just record it and set the flag.
#ifdef PROGRAM_COUPON
                SetMonData(&gPlayerPartyPtr[i], arr[index], &hasRibbon);
#endif
                gotRibbon = TRUE;
            }
        }
        if (gotRibbon)
            FlagSet(gAgbPmRomParams->giftRibbonsOffset);
    }
}
