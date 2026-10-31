#ifndef GUARD_MOVE_PP_H
#define GUARD_MOVE_PP_H

// pokeemerald's u8 (u16, u8, u8) CalculatePPWithBonus; colbtl's is in colbtl.h.
u8 CalculatePPWithBonus(u16 move, u8 ppBonuses, u8 moveIndex);
void MonRestorePP(struct Pokemon *mon);
void BoxMonRestorePP(struct BoxPokemon *boxMon);

#endif // GUARD_MOVE_PP_H
