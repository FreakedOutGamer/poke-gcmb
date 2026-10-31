#ifndef GUARD_SAVE_H
#define GUARD_SAVE_H

// pokeruby's save.c and game stats.
void IncrementGameStat(u8 index);
void CheckForFlashMemory(void);
u8 TrySavingData(u8 saveType);
u8 LoadGameSave(u8 saveType);
void Save_ResetSaveCounters(void);

#endif // GUARD_SAVE_H
