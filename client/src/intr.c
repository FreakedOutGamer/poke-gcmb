#include "main.h"

typedef void (*IntrFunc)(void);

extern IntrFunc gIntrTable[];

/*
 * Later client builds use the same operation and handler layout, but this
 * revision initializes 13 entries in reverse order. Keeping the intermediate
 * locals explicit reproduces the original agbcc register allocation.
 */
void InitIntrTable(void)
{
    IntrFunc *table = gIntrTable;
    IntrFunc handler = VBlankIntr;
    int *base = (int *)table;
    int dummy = (int)IntrDummy;
    int *intr = base;

    intr += 12;
    do
        *intr-- = dummy;
    while ((int)intr >= (int)base);

    table[1] = handler;
}
