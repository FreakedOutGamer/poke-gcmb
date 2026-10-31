#include "global.h"
#include "pokemon.h"

u32 GetMonData(struct Pokemon *mon, s32 field, u8 *data)
{
    switch (field)
    {
    case 55: return mon->status;
    case 56: return mon->level;
    case 57: return mon->hp;
    case 58: return mon->maxHP;
    case 59: return mon->attack;
    case 60: return mon->defense;
    case 61: return mon->speed;
    case 62: return mon->spAttack;
    case 63: return mon->spDefense;
    case 64: return mon->mail;
    default: return GetBoxMonData(&mon->box, field, data);
    }
}

