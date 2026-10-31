#ifndef GUARD_MAIN_H
#define GUARD_MAIN_H

// main.c
void VBlankIntr(void);
void IntrDummy(void);

// intr.c
void InitIntrTable(void);

// gflib/reset.c
void SoftResetRom(void);

#endif // GUARD_MAIN_H
