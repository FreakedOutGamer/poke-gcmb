#include "global.h"
#include "pokemon.h"
#include "constants/battle.h"

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

u32 CheckPartyPokerus(struct Pokemon *party, u8 selection)
{
    u32 retVal;
    u32 partyIndex = 0;
    u32 curBit = 1;

    retVal = 0;

    if (selection)
    {
        do
        {
            if ((selection & 1) && (GetMonDataInline(&party[partyIndex], MON_DATA_POKERUS, 0) & 0xF))
                retVal |= curBit;
            partyIndex++;
            curBit <<= 1;
            selection >>= 1;
        }
        while (selection);
    }
    else if (GetMonDataInline(&party[0], MON_DATA_POKERUS, 0) & 0xF)
    {
        retVal = 1;
    }

    return retVal;
}

static inline u32 GetAilmentFromStatus(u32 status)
{
    if (status & (STATUS1_POISON | STATUS1_TOXIC_POISON))
        return STATUS_PRIMARY_POISON;
    if (status & STATUS1_PARALYSIS)
        return STATUS_PRIMARY_PARALYSIS;
    if (status & STATUS1_SLEEP)
        return STATUS_PRIMARY_SLEEP;
    if (status & STATUS1_FREEZE)
        return STATUS_PRIMARY_FREEZE;
    if (status & STATUS1_BURN)
        return STATUS_PRIMARY_BURN;

    return STATUS_PRIMARY_NONE;
}

u32 GetMonAilment(struct Pokemon *mon)
{
    u32 statusAilment;

    if (GetMonDataInline(mon, MON_DATA_HP, NULL) == 0)
        return STATUS_PRIMARY_FAINTED;

    statusAilment = GetAilmentFromStatus(GetMonDataInline(mon, MON_DATA_STATUS, NULL));

    if (statusAilment == STATUS_PRIMARY_NONE)
    {
        if (!CheckPartyPokerus(mon, 0))
            return STATUS_PRIMARY_NONE;
        else
            return STATUS_PRIMARY_POKERUS;
    }

    return statusAilment;
}

u32 CheckPartyHasHadPokerus(struct Pokemon *party, u8 selection)
{
    u32 retVal;
    u32 partyIndex = 0;
    u32 curBit = 1;

    retVal = 0;

    if (selection)
    {
        do
        {
            if ((selection & 1) && GetMonDataInline(&party[partyIndex], MON_DATA_POKERUS, 0))
                retVal |= curBit;
            partyIndex++;
            curBit <<= 1;
            selection >>= 1;
        }
        while (selection);
    }
    else if (GetMonDataInline(&party[0], MON_DATA_POKERUS, 0))
    {
        retVal = 1;
    }

    return retVal;
}
