#ifndef GUARD_CONSTANTS_BATTLE_H
#define GUARD_CONSTANTS_BATTLE_H

#define STATUS1_SLEEP        (1 << 0 | 1 << 1 | 1 << 2)
#define STATUS1_POISON       (1 << 3)
#define STATUS1_BURN         (1 << 4)
#define STATUS1_FREEZE       (1 << 5)
#define STATUS1_PARALYSIS    (1 << 6)
#define STATUS1_TOXIC_POISON (1 << 7)
#define STATUS1_PSN_ANY      (STATUS1_POISON | STATUS1_TOXIC_POISON)

#define AILMENT_NONE 0
#define AILMENT_PSN  1
#define AILMENT_PRZ  2
#define AILMENT_SLP  3
#define AILMENT_FRZ  4
#define AILMENT_BRN  5
#define AILMENT_PKRS 6
#define AILMENT_FNT  7

#define STATUS1_NONE             0
#define STATUS1_SLEEP_TURN(num)  ((num) << 0) // Just for readability (or if rearranging statuses)
#define STATUS1_TOXIC_COUNTER    (1 << 8 | 1 << 9 | 1 << 10 | 1 << 11)
#define STATUS1_TOXIC_TURN(num)  ((num) << 8)
#define STATUS1_ANY              (STATUS1_SLEEP | STATUS1_POISON | STATUS1_BURN | STATUS1_FREEZE | STATUS1_PARALYSIS | STATUS1_TOXIC_POISON)

#endif // GUARD_CONSTANTS_BATTLE_H
