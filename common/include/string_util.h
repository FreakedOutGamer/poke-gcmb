#ifndef GUARD_STRING_UTIL_H
#define GUARD_STRING_UTIL_H

#include "global.h"

#define EOS 0xFF
#define EXT_CTRL_CODE_BEGIN 0xFC
#define EXT_CTRL_CODE_JPN 0x15

u8 *StringCopy(u8 *dest, const u8 *src);
u16 StringLength(const u8 *str);
u8 *StringAppend(u8 *dest, const u8 *src);
s32 StringCompare(const u8 *str1, const u8 *str2);
u8 GetExtCtrlCodeLength(u8 code);

#endif // GUARD_STRING_UTIL_H
