#include "global.h"
#include "pokemon.h"
#include "constants/pokemon.h"

#define SET8(lhs) (lhs) = *data
#define SET16(lhs) (lhs) = data[0] + (data[1] << 8)
#define SET32(lhs) (lhs) = data[0] + (data[1] << 8) + (data[2] << 16) + (data[3] << 24)

void SetMonData(struct Pokemon *mon, s32 field, const void *dataArg)
{
    const u8 *data = dataArg;

    switch (field)
    {
    case MON_DATA_STATUS:
        SET32(mon->status);
        break;
    case MON_DATA_LEVEL:
        SET8(mon->level);
        break;
    case MON_DATA_HP:
        SET16(mon->hp);
        break;
    case MON_DATA_MAX_HP:
        SET16(mon->maxHP);
        break;
    case MON_DATA_ATK:
        SET16(mon->attack);
        break;
    case MON_DATA_DEF:
        SET16(mon->defense);
        break;
    case MON_DATA_SPEED:
        SET16(mon->speed);
        break;
    case MON_DATA_SPATK:
        SET16(mon->spAttack);
        break;
    case MON_DATA_SPDEF:
        SET16(mon->spDefense);
        break;
    case MON_DATA_MAIL:
        SET8(mon->mail);
        break;
    case MON_DATA_SPECIES_OR_EGG:
        break;
    default:
        SetBoxMonData(&mon->box, field, data);
        break;
    }
}
