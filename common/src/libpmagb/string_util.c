#include "global.h"
#include "string_util.h"

// Ruby/Sapphire's string helpers, cut down to the ones these programs use.
// StringLength and GetExtCtrlCodeLength are needed because a nickname or OT
// name can carry extended control codes, which are longer than one byte.
//
// GetExtCtrlCodeLength lives in Ruby/Sapphire's text.c rather than
// string_util.c, and its table is static there.

#if defined(PROGRAM_CLIENT) || defined(PROGRAM_SAMPLE0519)

// The client and sample0519 have only StringLength, in an older form that
// does not skip control codes.
u16 StringLength(const u8 *str)
{
    u16 length = 0;

    while (str[length] != EOS)
        length++;

    return length;
}

#else

// Bytes following each extended control code, indexed by the byte after
// EXT_CTRL_CODE_BEGIN. colbtl and coupon keep the table with their other
// data (src/text.c, src/pokemon_tables.c).
#if defined(PROGRAM_COUPON) || defined(PROGRAM_COLBTL)
extern const u8 gExtCtrlCodeLengths[23];
#else
const u8 gExtCtrlCodeLengths[23] =
{
    1, 2, 2, 2, 4, 2, 2, 1, 2, 1, 1, 3,
    2, 2, 2, 1, 3, 2, 2, 2, 2, 1, 1,
};
#endif

u16 StringLength(const u8 *str)
{
    u16 length = 0;

    while (str[length] != EOS)
    {
        u16 temp = length;
        length++;
        if (str[temp] == EXT_CTRL_CODE_BEGIN)
            length += GetExtCtrlCodeLength(str[length]);
    }

    return length;
}

static inline u8 *CopyString(u8 *dest, const u8 *src)
{
    while (*src != EOS)
    {
        *dest = *src;
        dest++;
        src++;
    }

    *dest = EOS;
    return dest;
}

u8 *StringCopy(u8 *dest, const u8 *src)
{
    return CopyString(dest, src);
}

u8 *StringAppend(u8 *dest, const u8 *src)
{
    while (*dest != EOS)
        dest++;

    return CopyString(dest, src);
}

#if defined(PROGRAM_POKECHAN) || defined(PROGRAM_COLBTL)
s32 StringCompare(const u8 *str1, const u8 *str2)
{
    while (*str1 == *str2)
    {
        if (*str1 == EOS)
            return 0;
        str1++;
        str2++;
    }

    return *str1 - *str2;
}
#endif

u8 GetExtCtrlCodeLength(u8 code)
{
    u8 length = 0;
    if (code <= 0x16)
        length = gExtCtrlCodeLengths[code];
    return length;
}

#endif
