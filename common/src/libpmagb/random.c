#include "global.h"

extern u32 gRngValue;

u16 Random(void)
{
    gRngValue = 1103515245 * gRngValue + 24691;
    return gRngValue >> 16;
}

void SeedRng(u16 seed)
{
    gRngValue = seed;
}
