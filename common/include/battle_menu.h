#ifndef GUARD_BATTLE_MENU_H
#define GUARD_BATTLE_MENU_H

bool32 ShowBattleMenu(u32 monId);
void RestoreBattleMenu(void);
bool32 IsMonFainted(u32 monId);
u32 GetMoveRestriction(s32 moveSlot);
u32 IsMoveSelectionLocked(s32 moveSlot);
u32 GetBattleMenuRestriction(void);
u32 GetForcedMoveSlot(void);

#endif // GUARD_BATTLE_MENU_H
