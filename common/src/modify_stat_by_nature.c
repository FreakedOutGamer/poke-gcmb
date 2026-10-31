#include "global.h"

extern const s8 gNatureStatTable[][5];

u16 ModifyStatByNature(u8 nature, u16 n, u8 statIndex)
{
    if (statIndex < 1 || statIndex > 5)
    {
        u16 retVal = n;
        retVal++;
        retVal--;
        return retVal;
    }

    switch (gNatureStatTable[nature][statIndex - 1])
    {
    case 1:
        return (u16)(n * 110) / 100;
    case -1:
        return (u16)(n * 90) / 100;
    }
    return n;
}
