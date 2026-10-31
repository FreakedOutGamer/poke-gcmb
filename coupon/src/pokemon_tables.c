#include "global.h"

struct BerryInfo
{
    u8 name[7];
    u8 firmness;
    u16 size;
    u8 maxYield;
    u8 minYield;
    const u8 *description1;
    const u8 *description2;
    u8 stageDuration;
    u8 spicy;
    u8 dry;
    u8 sweet;
    u8 bitter;
    u8 sour;
    u8 smoothness;
};

// Names use the Gen III Japanese character encoding. This utility
// omits the normal in-game Berry description strings.
const struct BerryInfo gBerryInfo[43] =
{
    {
        .name = {0x58, 0x77, 0x9A, 0xFF, 0xFF, 0xFF, 0xFF},
        .firmness = 2, .size = 20,
        .maxYield = 3, .minYield = 2,
        .description1 = 0, .description2 = 0,
        .stageDuration = 3, .spicy = 10, .dry = 0,
        .sweet = 0, .bitter = 0, .sour = 0,
        .smoothness = 25,
    }, // Berry 1
    {
        .name = {0x56, 0x8B, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        .firmness = 5, .size = 80,
        .maxYield = 3, .minYield = 2,
        .description1 = 0, .description2 = 0,
        .stageDuration = 3, .spicy = 0, .dry = 10,
        .sweet = 0, .bitter = 0, .sour = 0,
        .smoothness = 25,
    }, // Berry 2
    {
        .name = {0x73, 0x73, 0x7E, 0xFF, 0xFF, 0xFF, 0xFF},
        .firmness = 1, .size = 40,
        .maxYield = 3, .minYield = 2,
        .description1 = 0, .description2 = 0,
        .stageDuration = 3, .spicy = 0, .dry = 0,
        .sweet = 10, .bitter = 0, .sour = 0,
        .smoothness = 25,
    }, // Berry 3
    {
        .name = {0x61, 0xAE, 0x8B, 0xFF, 0xFF, 0xFF, 0xFF},
        .firmness = 3, .size = 32,
        .maxYield = 3, .minYield = 2,
        .description1 = 0, .description2 = 0,
        .stageDuration = 3, .spicy = 0, .dry = 0,
        .sweet = 0, .bitter = 10, .sour = 0,
        .smoothness = 25,
    }, // Berry 4
    {
        .name = {0x65, 0x65, 0x5C, 0xFF, 0xFF, 0xFF, 0xFF},
        .firmness = 5, .size = 50,
        .maxYield = 3, .minYield = 2,
        .description1 = 0, .description2 = 0,
        .stageDuration = 3, .spicy = 0, .dry = 0,
        .sweet = 0, .bitter = 0, .sour = 10,
        .smoothness = 25,
    }, // Berry 5
    {
        .name = {0x6B, 0x72, 0x78, 0xFF, 0xFF, 0xFF, 0xFF},
        .firmness = 4, .size = 28,
        .maxYield = 3, .minYield = 2,
        .description1 = 0, .description2 = 0,
        .stageDuration = 4, .spicy = 10, .dry = 0,
        .sweet = 10, .bitter = 10, .sour = 10,
        .smoothness = 20,
    }, // Berry 6
    {
        .name = {0x55, 0x7A, 0x7E, 0xFF, 0xFF, 0xFF, 0xFF},
        .firmness = 5, .size = 35,
        .maxYield = 3, .minYield = 2,
        .description1 = 0, .description2 = 0,
        .stageDuration = 3, .spicy = 10, .dry = 10,
        .sweet = 10, .bitter = 10, .sour = 10,
        .smoothness = 20,
    }, // Berry 7
    {
        .name = {0x57, 0xAE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        .firmness = 3, .size = 47,
        .maxYield = 3, .minYield = 2,
        .description1 = 0, .description2 = 0,
        .stageDuration = 3, .spicy = 10, .dry = 10,
        .sweet = 10, .bitter = 10, .sour = 10,
        .smoothness = 20,
    }, // Berry 8
    {
        .name = {0x77, 0x71, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        .firmness = 5, .size = 34,
        .maxYield = 2, .minYield = 1,
        .description1 = 0, .description2 = 0,
        .stageDuration = 12, .spicy = 10, .dry = 10,
        .sweet = 10, .bitter = 10, .sour = 10,
        .smoothness = 20,
    }, // Berry 9
    {
        .name = {0x55, 0x9A, 0x7E, 0xFF, 0xFF, 0xFF, 0xFF},
        .firmness = 4, .size = 95,
        .maxYield = 3, .minYield = 2,
        .description1 = 0, .description2 = 0,
        .stageDuration = 6, .spicy = 10, .dry = 10,
        .sweet = 10, .bitter = 10, .sour = 10,
        .smoothness = 20,
    }, // Berry 10
    {
        .name = {0x6C, 0x80, 0x77, 0xFF, 0xFF, 0xFF, 0xFF},
        .firmness = 2, .size = 100,
        .maxYield = 3, .minYield = 2,
        .description1 = 0, .description2 = 0,
        .stageDuration = 6, .spicy = 10, .dry = 0,
        .sweet = 0, .bitter = 0, .sour = 0,
        .smoothness = 25,
    }, // Berry 11
    {
        .name = {0x53, 0x52, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        .firmness = 3, .size = 115,
        .maxYield = 3, .minYield = 2,
        .description1 = 0, .description2 = 0,
        .stageDuration = 6, .spicy = 0, .dry = 10,
        .sweet = 0, .bitter = 0, .sour = 0,
        .smoothness = 25,
    }, // Berry 12
    {
        .name = {0x6F, 0x8B, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        .firmness = 3, .size = 126,
        .maxYield = 3, .minYield = 2,
        .description1 = 0, .description2 = 0,
        .stageDuration = 6, .spicy = 0, .dry = 0,
        .sweet = 10, .bitter = 0, .sour = 0,
        .smoothness = 25,
    }, // Berry 13
    {
        .name = {0x96, 0x7E, 0x8D, 0xFF, 0xFF, 0xFF, 0xFF},
        .firmness = 5, .size = 64,
        .maxYield = 3, .minYield = 2,
        .description1 = 0, .description2 = 0,
        .stageDuration = 6, .spicy = 0, .dry = 0,
        .sweet = 0, .bitter = 10, .sour = 0,
        .smoothness = 25,
    }, // Berry 14
    {
        .name = {0x52, 0x51, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        .firmness = 2, .size = 223,
        .maxYield = 3, .minYield = 2,
        .description1 = 0, .description2 = 0,
        .stageDuration = 6, .spicy = 0, .dry = 0,
        .sweet = 0, .bitter = 0, .sour = 10,
        .smoothness = 25,
    }, // Berry 15
    {
        .name = {0x8E, 0x78, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        .firmness = 4, .size = 120,
        .maxYield = 6, .minYield = 3,
        .description1 = 0, .description2 = 0,
        .stageDuration = 1, .spicy = 10, .dry = 10,
        .sweet = 0, .bitter = 0, .sour = 0,
        .smoothness = 20,
    }, // Berry 16
    {
        .name = {0x98, 0x78, 0xAE, 0xFF, 0xFF, 0xFF, 0xFF},
        .firmness = 2, .size = 108,
        .maxYield = 6, .minYield = 3,
        .description1 = 0, .description2 = 0,
        .stageDuration = 1, .spicy = 0, .dry = 10,
        .sweet = 10, .bitter = 0, .sour = 0,
        .smoothness = 20,
    }, // Berry 17
    {
        .name = {0x65, 0x65, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        .firmness = 4, .size = 77,
        .maxYield = 6, .minYield = 3,
        .description1 = 0, .description2 = 0,
        .stageDuration = 1, .spicy = 0, .dry = 0,
        .sweet = 10, .bitter = 10, .sour = 0,
        .smoothness = 20,
    }, // Berry 18
    {
        .name = {0x5E, 0x5C, 0x65, 0xFF, 0xFF, 0xFF, 0xFF},
        .firmness = 5, .size = 74,
        .maxYield = 6, .minYield = 3,
        .description1 = 0, .description2 = 0,
        .stageDuration = 1, .spicy = 0, .dry = 0,
        .sweet = 0, .bitter = 10, .sour = 10,
        .smoothness = 20,
    }, // Berry 19
    {
        .name = {0x9B, 0x52, 0x79, 0xFF, 0xFF, 0xFF, 0xFF},
        .firmness = 3, .size = 80,
        .maxYield = 6, .minYield = 3,
        .description1 = 0, .description2 = 0,
        .stageDuration = 1, .spicy = 10, .dry = 0,
        .sweet = 0, .bitter = 0, .sour = 10,
        .smoothness = 20,
    }, // Berry 20
    {
        .name = {0x8C, 0x7B, 0x58, 0xFF, 0xFF, 0xFF, 0xFF},
        .firmness = 4, .size = 135,
        .maxYield = 6, .minYield = 2,
        .description1 = 0, .description2 = 0,
        .stageDuration = 3, .spicy = 10, .dry = 0,
        .sweet = 10, .bitter = 10, .sour = 0,
        .smoothness = 20,
    }, // Berry 21
    {
        .name = {0x68, 0x5A, 0x98, 0xFF, 0xFF, 0xFF, 0xFF},
        .firmness = 3, .size = 150,
        .maxYield = 6, .minYield = 2,
        .description1 = 0, .description2 = 0,
        .stageDuration = 3, .spicy = 0, .dry = 10,
        .sweet = 0, .bitter = 10, .sour = 10,
        .smoothness = 20,
    }, // Berry 22
    {
        .name = {0x60, 0x9F, 0x79, 0xFF, 0xFF, 0xFF, 0xFF},
        .firmness = 3, .size = 110,
        .maxYield = 6, .minYield = 2,
        .description1 = 0, .description2 = 0,
        .stageDuration = 3, .spicy = 10, .dry = 0,
        .sweet = 10, .bitter = 0, .sour = 10,
        .smoothness = 20,
    }, // Berry 23
    {
        .name = {0x7B, 0x72, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        .firmness = 3, .size = 162,
        .maxYield = 6, .minYield = 2,
        .description1 = 0, .description2 = 0,
        .stageDuration = 3, .spicy = 10, .dry = 10,
        .sweet = 0, .bitter = 10, .sour = 0,
        .smoothness = 20,
    }, // Berry 24
    {
        .name = {0x53, 0x98, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        .firmness = 2, .size = 149,
        .maxYield = 6, .minYield = 2,
        .description1 = 0, .description2 = 0,
        .stageDuration = 3, .spicy = 0, .dry = 10,
        .sweet = 10, .bitter = 0, .sour = 10,
        .smoothness = 20,
    }, // Berry 25
    {
        .name = {0x6F, 0x64, 0x6F, 0xFF, 0xFF, 0xFF, 0xFF},
        .firmness = 2, .size = 200,
        .maxYield = 4, .minYield = 2,
        .description1 = 0, .description2 = 0,
        .stageDuration = 6, .spicy = 20, .dry = 10,
        .sweet = 0, .bitter = 0, .sour = 0,
        .smoothness = 30,
    }, // Berry 26
    {
        .name = {0x73, 0x5A, 0x5C, 0xFF, 0xFF, 0xFF, 0xFF},
        .firmness = 3, .size = 75,
        .maxYield = 4, .minYield = 2,
        .description1 = 0, .description2 = 0,
        .stageDuration = 6, .spicy = 0, .dry = 20,
        .sweet = 10, .bitter = 0, .sour = 0,
        .smoothness = 30,
    }, // Berry 27
    {
        .name = {0x8B, 0x5D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        .firmness = 3, .size = 140,
        .maxYield = 4, .minYield = 2,
        .description1 = 0, .description2 = 0,
        .stageDuration = 6, .spicy = 0, .dry = 0,
        .sweet = 20, .bitter = 10, .sour = 0,
        .smoothness = 30,
    }, // Berry 28
    {
        .name = {0x77, 0x98, 0x60, 0xFF, 0xFF, 0xFF, 0xFF},
        .firmness = 2, .size = 226,
        .maxYield = 4, .minYield = 2,
        .description1 = 0, .description2 = 0,
        .stageDuration = 6, .spicy = 0, .dry = 0,
        .sweet = 0, .bitter = 20, .sour = 10,
        .smoothness = 30,
    }, // Berry 29
    {
        .name = {0x69, 0x72, 0x79, 0xFF, 0xFF, 0xFF, 0xFF},
        .firmness = 5, .size = 285,
        .maxYield = 4, .minYield = 2,
        .description1 = 0, .description2 = 0,
        .stageDuration = 6, .spicy = 10, .dry = 0,
        .sweet = 0, .bitter = 0, .sour = 20,
        .smoothness = 30,
    }, // Berry 30
    {
        .name = {0x69, 0x7C, 0x57, 0xFF, 0xFF, 0xFF, 0xFF},
        .firmness = 2, .size = 133,
        .maxYield = 2, .minYield = 1,
        .description1 = 0, .description2 = 0,
        .stageDuration = 18, .spicy = 40, .dry = 10,
        .sweet = 0, .bitter = 0, .sour = 0,
        .smoothness = 70,
    }, // Berry 31
    {
        .name = {0x5C, 0xAE, 0x74, 0xFF, 0xFF, 0xFF, 0xFF},
        .firmness = 1, .size = 244,
        .maxYield = 2, .minYield = 1,
        .description1 = 0, .description2 = 0,
        .stageDuration = 18, .spicy = 0, .dry = 40,
        .sweet = 10, .bitter = 0, .sour = 0,
        .smoothness = 70,
    }, // Berry 32
    {
        .name = {0x56, 0x52, 0x5D, 0xFF, 0xFF, 0xFF, 0xFF},
        .firmness = 2, .size = 250,
        .maxYield = 2, .minYield = 1,
        .description1 = 0, .description2 = 0,
        .stageDuration = 18, .spicy = 0, .dry = 0,
        .sweet = 40, .bitter = 10, .sour = 0,
        .smoothness = 70,
    }, // Berry 33
    {
        .name = {0x95, 0x78, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        .firmness = 3, .size = 280,
        .maxYield = 2, .minYield = 1,
        .description1 = 0, .description2 = 0,
        .stageDuration = 18, .spicy = 0, .dry = 0,
        .sweet = 0, .bitter = 40, .sour = 10,
        .smoothness = 70,
    }, // Berry 34
    {
        .name = {0x99, 0x78, 0x98, 0xFF, 0xFF, 0xFF, 0xFF},
        .firmness = 1, .size = 300,
        .maxYield = 2, .minYield = 1,
        .description1 = 0, .description2 = 0,
        .stageDuration = 18, .spicy = 10, .dry = 0,
        .sweet = 0, .bitter = 0, .sour = 40,
        .smoothness = 70,
    }, // Berry 35
    {
        .name = {0x61, 0x52, 0x77, 0xFF, 0xFF, 0xFF, 0xFF},
        .firmness = 4, .size = 111,
        .maxYield = 2, .minYield = 1,
        .description1 = 0, .description2 = 0,
        .stageDuration = 24, .spicy = 40, .dry = 0,
        .sweet = 40, .bitter = 0, .sour = 10,
        .smoothness = 80,
    }, // Berry 36
    {
        .name = {0x78, 0x85, 0x87, 0xFF, 0xFF, 0xFF, 0xFF},
        .firmness = 4, .size = 33,
        .maxYield = 2, .minYield = 1,
        .description1 = 0, .description2 = 0,
        .stageDuration = 24, .spicy = 0, .dry = 40,
        .sweet = 0, .bitter = 40, .sour = 0,
        .smoothness = 80,
    }, // Berry 37
    {
        .name = {0x56, 0x71, 0x77, 0xFF, 0xFF, 0xFF, 0xFF},
        .firmness = 4, .size = 95,
        .maxYield = 2, .minYield = 1,
        .description1 = 0, .description2 = 0,
        .stageDuration = 24, .spicy = 0, .dry = 0,
        .sweet = 40, .bitter = 0, .sour = 40,
        .smoothness = 80,
    }, // Berry 38
    {
        .name = {0x74, 0x60, 0x9C, 0xFF, 0xFF, 0xFF, 0xFF},
        .firmness = 4, .size = 237,
        .maxYield = 2, .minYield = 1,
        .description1 = 0, .description2 = 0,
        .stageDuration = 24, .spicy = 40, .dry = 0,
        .sweet = 0, .bitter = 40, .sour = 0,
        .smoothness = 80,
    }, // Berry 39
    {
        .name = {0x8E, 0x51, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        .firmness = 3, .size = 75,
        .maxYield = 2, .minYield = 1,
        .description1 = 0, .description2 = 0,
        .stageDuration = 24, .spicy = 0, .dry = 40,
        .sweet = 0, .bitter = 0, .sour = 40,
        .smoothness = 80,
    }, // Berry 40
    {
        .name = {0x5B, 0x7E, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        .firmness = 2, .size = 97,
        .maxYield = 2, .minYield = 1,
        .description1 = 0, .description2 = 0,
        .stageDuration = 24, .spicy = 10, .dry = 10,
        .sweet = 10, .bitter = 10, .sour = 10,
        .smoothness = 30,
    }, // Berry 41
    {
        .name = {0x5D, 0x60, 0xAE, 0xFF, 0xFF, 0xFF, 0xFF},
        .firmness = 5, .size = 153,
        .maxYield = 2, .minYield = 1,
        .description1 = 0, .description2 = 0,
        .stageDuration = 24, .spicy = 10, .dry = 10,
        .sweet = 10, .bitter = 10, .sour = 10,
        .smoothness = 30,
    }, // Berry 42
    {
        .name = {0x65, 0x90, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
        .firmness = 0, .size = 0,
        .maxYield = 2, .minYield = 1,
        .description1 = 0, .description2 = 0,
        .stageDuration = 24, .spicy = 40, .dry = 40,
        .sweet = 40, .bitter = 40, .sour = 40,
        .smoothness = 40,
    }, // Berry 43
};

const u32 gBitTable[32] =
{
    0x00000001, 0x00000002, 0x00000004, 0x00000008,
    0x00000010, 0x00000020, 0x00000040, 0x00000080,
    0x00000100, 0x00000200, 0x00000400, 0x00000800,
    0x00001000, 0x00002000, 0x00004000, 0x00008000,
    0x00010000, 0x00020000, 0x00040000, 0x00080000,
    0x00100000, 0x00200000, 0x00400000, 0x00800000,
    0x01000000, 0x02000000, 0x04000000, 0x08000000,
    0x10000000, 0x20000000, 0x40000000, 0x80000000,
};

const u8 gTypeToPaletteNumber[23] =
{
    0x0D, 0x0D, 0x0E, 0x0E, 0x0D, 0x0D, 0x0F, 0x0E, 0x0D, 0x0F, 0x0D, 0x0E, 0x0F, 0x0D, 0x0E, 0x0E, 0x0F, 0x0D, 0x0D, 0x0E, 0x0E, 0x0F, 0x0D
};

const u8 gPPUpGetMask[4] = {0x03, 0x0C, 0x30, 0xC0};

const u8 gGiftRibbonMonDataIds[7] = {0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E};
const u16 gGiftRibbonPadding = 0;

const u16 gSpeciesToNationalPokedexNum[411] =
{
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
    13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
    25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36,
    37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
    49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60,
    61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72,
    73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84,
    85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96,
    97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108,
    109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120,
    121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132,
    133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144,
    145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156,
    157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168,
    169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180,
    181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192,
    193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204,
    205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216,
    217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228,
    229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240,
    241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 387,
    388, 389, 390, 391, 392, 393, 394, 395, 396, 397, 398, 399,
    400, 401, 402, 403, 404, 405, 406, 407, 408, 409, 410, 411,
    252, 253, 254, 255, 256, 257, 258, 259, 260, 261, 262, 263,
    264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274, 275,
    290, 291, 292, 276, 277, 285, 286, 327, 278, 279, 283, 284,
    320, 321, 300, 301, 352, 343, 344, 299, 324, 302, 339, 340,
    370, 341, 342, 349, 350, 318, 319, 328, 329, 330, 296, 297,
    309, 310, 322, 323, 363, 364, 365, 331, 332, 361, 362, 337,
    338, 298, 325, 326, 311, 312, 303, 307, 308, 333, 334, 360,
    355, 356, 315, 287, 288, 289, 316, 317, 357, 293, 294, 295,
    366, 367, 368, 359, 353, 354, 336, 335, 369, 304, 305, 306,
    351, 313, 314, 345, 346, 347, 348, 280, 281, 282, 371, 372,
    373, 374, 375, 376, 377, 378, 379, 382, 383, 384, 380, 381,
    385, 386, 358,
};

const u16 gPokemonTablesPadding = 0;

// Ruby/Sapphire icon graphics pointers used for the linked cartridge.
const void *const gMonIconTable[440] =
{
    (const void *)0x086AD128, (const void *)0x086AD128, (const void *)0x086ADC98, (const void *)0x086AEB64,
    (const void *)0x086AF518, (const void *)0x086B0010, (const void *)0x086B0DEC, (const void *)0x086B17B8,
    (const void *)0x086B22D8, (const void *)0x086B3090, (const void *)0x086B3A10, (const void *)0x086B42C8,
    (const void *)0x086B4E64, (const void *)0x086B577C, (const void *)0x086B60B4, (const void *)0x086B6CA8,
    (const void *)0x086B7758, (const void *)0x086B8288, (const void *)0x086B8F7C, (const void *)0x086B9914,
    (const void *)0x086BA43C, (const void *)0x086BAE10, (const void *)0x086BBA0C, (const void *)0x086BC46C,
    (const void *)0x086BCFC8, (const void *)0x086BD9E0, (const void *)0x086BE51C, (const void *)0x086BEF74,
    (const void *)0x086BFBEC, (const void *)0x086C05AC, (const void *)0x086C10DC, (const void *)0x086C1D9C,
    (const void *)0x086C2810, (const void *)0x086C33D0, (const void *)0x086C4174, (const void *)0x086C4B3C,
    (const void *)0x086C5620, (const void *)0x086C60C8, (const void *)0x086C6CE4, (const void *)0x086C7628,
    (const void *)0x086C8094, (const void *)0x086C8A98, (const void *)0x086C9560, (const void *)0x086C9E7C,
    (const void *)0x086CA9CC, (const void *)0x086CB5D0, (const void *)0x086CBF4C, (const void *)0x086CCA7C,
    (const void *)0x086CD594, (const void *)0x086CE26C, (const void *)0x086CEAF4, (const void *)0x086CF534,
    (const void *)0x086CFF74, (const void *)0x086D0AFC, (const void *)0x086D14E0, (const void *)0x086D2054,
    (const void *)0x086D2B44, (const void *)0x086D36D4, (const void *)0x086D41D8, (const void *)0x086D4F5C,
    (const void *)0x086D58C4, (const void *)0x086D63C8, (const void *)0x086D6F40, (const void *)0x086D79A0,
    (const void *)0x086D8650, (const void *)0x086D9300, (const void *)0x086D9CF4, (const void *)0x086DA864,
    (const void *)0x086DB680, (const void *)0x086DC00C, (const void *)0x086DCA9C, (const void *)0x086DD6D0,
    (const void *)0x086DE0AC, (const void *)0x086DED54, (const void *)0x086DF6AC, (const void *)0x086E0190,
    (const void *)0x086E0D3C, (const void *)0x086E1890, (const void *)0x086E25E0, (const void *)0x086E2FC0,
    (const void *)0x086E3C8C, (const void *)0x086E44B8, (const void *)0x086E50AC, (const void *)0x086E5C04,
    (const void *)0x086E66D8, (const void *)0x086E7488, (const void *)0x086E7EEC, (const void *)0x086E89A8,
    (const void *)0x086E9410, (const void *)0x086EA01C, (const void *)0x086EAA00, (const void *)0x086EB6AC,
    (const void *)0x086EC1F8, (const void *)0x086ECD88, (const void *)0x086ED874, (const void *)0x086EE4C0,
    (const void *)0x086EEEFC, (const void *)0x086EFA80, (const void *)0x086F0534, (const void *)0x086F1254,
    (const void *)0x086F1B38, (const void *)0x086F247C, (const void *)0x086F2FF0, (const void *)0x086F3E00,
    (const void *)0x086F4880, (const void *)0x086F53B0, (const void *)0x086F5EC4, (const void *)0x086F69B8,
    (const void *)0x086F74A0, (const void *)0x086F7F6C, (const void *)0x086F8B94, (const void *)0x086F9740,
    (const void *)0x086FA508, (const void *)0x086FAF98, (const void *)0x086FBB80, (const void *)0x086FC918,
    (const void *)0x086FD2E0, (const void *)0x086FDEB0, (const void *)0x086FE9F8, (const void *)0x086FF62C,
    (const void *)0x086FFFFC, (const void *)0x08700B38, (const void *)0x0870168C, (const void *)0x08702350,
    (const void *)0x08702F54, (const void *)0x08703B50, (const void *)0x087047B0, (const void *)0x08705394,
    (const void *)0x08705F88, (const void *)0x08706A78, (const void *)0x08707924, (const void *)0x08708498,
    (const void *)0x08708CE8, (const void *)0x0870971C, (const void *)0x0870A1E8, (const void *)0x0870AD88,
    (const void *)0x0870B904, (const void *)0x0870C2E0, (const void *)0x0870CD44, (const void *)0x0870D8B4,
    (const void *)0x0870E22C, (const void *)0x0870EE38, (const void *)0x0870FA2C, (const void *)0x087104FC,
    (const void *)0x087110C0, (const void *)0x08711CE0, (const void *)0x08712A3C, (const void *)0x087133EC,
    (const void *)0x08713ECC, (const void *)0x08714BA8, (const void *)0x08715818, (const void *)0x087161CC,
    (const void *)0x08716B68, (const void *)0x0871773C, (const void *)0x08718458, (const void *)0x08718E80,
    (const void *)0x08719980, (const void *)0x0871A61C, (const void *)0x0871B010, (const void *)0x0871BB38,
    (const void *)0x0871C9E0, (const void *)0x0871D3D0, (const void *)0x0871DE9C, (const void *)0x0871E898,
    (const void *)0x0871F3B8, (const void *)0x0871FE4C, (const void *)0x0872099C, (const void *)0x087212E4,
    (const void *)0x08721E8C, (const void *)0x087229B4, (const void *)0x087233F4, (const void *)0x08723ED4,
    (const void *)0x087247BC, (const void *)0x08725048, (const void *)0x087258DC, (const void *)0x08726194,
    (const void *)0x08726BA4, (const void *)0x087273F4, (const void *)0x08727EB8, (const void *)0x087288D0,
    (const void *)0x0872935C, (const void *)0x08729EA0, (const void *)0x0872A8A4, (const void *)0x0872B284,
    (const void *)0x0872BCB0, (const void *)0x0872C794, (const void *)0x0872D1FC, (const void *)0x0872DC00,
    (const void *)0x0872E5D8, (const void *)0x0872F1AC, (const void *)0x0872FC0C, (const void *)0x08730578,
    (const void *)0x087310E0, (const void *)0x08731C88, (const void *)0x087325C4, (const void *)0x087330A4,
    (const void *)0x08733B28, (const void *)0x087345C0, (const void *)0x08735028, (const void *)0x08735BD8,
    (const void *)0x0873662C, (const void *)0x08736E60, (const void *)0x08737864, (const void *)0x08738484,
    (const void *)0x08738F04, (const void *)0x08739A50, (const void *)0x0873A4CC, (const void *)0x0873B13C,
    (const void *)0x0873BE78, (const void *)0x0873C8E4, (const void *)0x0873D404, (const void *)0x0873DE14,
    (const void *)0x0873EAEC, (const void *)0x0873F47C, (const void *)0x0874004C, (const void *)0x08740B24,
    (const void *)0x08741478, (const void *)0x087420A8, (const void *)0x08742A6C, (const void *)0x0874363C,
    (const void *)0x08743EE4, (const void *)0x087448C8, (const void *)0x087452E0, (const void *)0x08745CCC,
    (const void *)0x08746728, (const void *)0x087472C8, (const void *)0x08747E74, (const void *)0x08748AC8,
    (const void *)0x08749470, (const void *)0x0874A004, (const void *)0x0874AC48, (const void *)0x0874B5D8,
    (const void *)0x0874C1B8, (const void *)0x0874CBCC, (const void *)0x0874D79C, (const void *)0x0874E2C4,
    (const void *)0x0874ECDC, (const void *)0x0874F900, (const void *)0x08750240, (const void *)0x08750D10,
    (const void *)0x087516B0, (const void *)0x0875226C, (const void *)0x08752D90, (const void *)0x08753B24,
    (const void *)0x087549B4, (const void *)0x08755884, (const void *)0x087561FC, (const void *)0x08756C94,
    (const void *)0x08757A2C, (const void *)0x08758734, (const void *)0x08759608, (const void *)0x0875A09C,
    (const void *)0x087DA3A4, (const void *)0x087DA3A4, (const void *)0x087DA3A4, (const void *)0x087DA3A4,
    (const void *)0x087DA3A4, (const void *)0x087DA3A4, (const void *)0x087DA3A4, (const void *)0x087DA3A4,
    (const void *)0x087DA3A4, (const void *)0x087DA3A4, (const void *)0x087DA3A4, (const void *)0x087DA3A4,
    (const void *)0x087DA3A4, (const void *)0x087DA3A4, (const void *)0x087DA3A4, (const void *)0x087DA3A4,
    (const void *)0x087DA3A4, (const void *)0x087DA3A4, (const void *)0x087DA3A4, (const void *)0x087DA3A4,
    (const void *)0x087DA3A4, (const void *)0x087DA3A4, (const void *)0x087DA3A4, (const void *)0x087DA3A4,
    (const void *)0x087DA3A4, (const void *)0x0875AE40, (const void *)0x0875BA78, (const void *)0x0875C7E8,
    (const void *)0x0875D1CC, (const void *)0x0875DDF8, (const void *)0x0875EABC, (const void *)0x0875F4C4,
    (const void *)0x0876006C, (const void *)0x08760E0C, (const void *)0x08761870, (const void *)0x08762490,
    (const void *)0x08763010, (const void *)0x08763AC4, (const void *)0x08764408, (const void *)0x08764D14,
    (const void *)0x0876588C, (const void *)0x08766178, (const void *)0x08766BA8, (const void *)0x08767560,
    (const void *)0x08767FE4, (const void *)0x08768C34, (const void *)0x08769618, (const void *)0x0876A03C,
    (const void *)0x0876AC2C, (const void *)0x0876B5F4, (const void *)0x0876C1A0, (const void *)0x0876CCD0,
    (const void *)0x0876D5B8, (const void *)0x0876E124, (const void *)0x0876EADC, (const void *)0x0876F714,
    (const void *)0x087701B4, (const void *)0x08770BB0, (const void *)0x08771738, (const void *)0x08771FEC,
    (const void *)0x08772B50, (const void *)0x08773484, (const void *)0x08773ED0, (const void *)0x08774988,
    (const void *)0x08775520, (const void *)0x087760E8, (const void *)0x08776A14, (const void *)0x08777610,
    (const void *)0x08777FEC, (const void *)0x08778BD0, (const void *)0x0877963C, (const void *)0x08779FCC,
    (const void *)0x0877AB00, (const void *)0x0877B2FC, (const void *)0x0877BDE4, (const void *)0x0877CAC8,
    (const void *)0x0877D4FC, (const void *)0x0877E0F0, (const void *)0x0877EB9C, (const void *)0x0877F6F4,
    (const void *)0x0877FFA0, (const void *)0x087809F8, (const void *)0x08781794, (const void *)0x08782208,
    (const void *)0x08782F08, (const void *)0x08783894, (const void *)0x08784260, (const void *)0x08784C5C,
    (const void *)0x087857A8, (const void *)0x08786088, (const void *)0x08786AF4, (const void *)0x0878778C,
    (const void *)0x0878826C, (const void *)0x08788E14, (const void *)0x0878982C, (const void *)0x0878A414,
    (const void *)0x0878AF04, (const void *)0x0878BBAC, (const void *)0x0878C5FC, (const void *)0x0878CF18,
    (const void *)0x0878DAA4, (const void *)0x0878E458, (const void *)0x0878EDF8, (const void *)0x0878FA24,
    (const void *)0x08790410, (const void *)0x08790EE8, (const void *)0x087919E8, (const void *)0x087925AC,
    (const void *)0x08792F44, (const void *)0x08793954, (const void *)0x08794450, (const void *)0x08794FF4,
    (const void *)0x08795A34, (const void *)0x08796568, (const void *)0x08797270, (const void *)0x08797B7C,
    (const void *)0x08798678, (const void *)0x08799384, (const void *)0x08799CF8, (const void *)0x0879A950,
    (const void *)0x0879B734, (const void *)0x0879C110, (const void *)0x0879CCA0, (const void *)0x0879D708,
    (const void *)0x0879E308, (const void *)0x0879EC48, (const void *)0x0879F5E8, (const void *)0x087A0304,
    (const void *)0x087A0EB4, (const void *)0x087A1A10, (const void *)0x087A2290, (const void *)0x087A2DB8,
    (const void *)0x087A3BCC, (const void *)0x087A552C, (const void *)0x087A608C, (const void *)0x087A6B8C,
    (const void *)0x087A7684, (const void *)0x087A830C, (const void *)0x087A8CE8, (const void *)0x087A9B10,
    (const void *)0x087AA3EC, (const void *)0x087AAECC, (const void *)0x087AB9EC, (const void *)0x087AC3BC,
    (const void *)0x087ACE18, (const void *)0x087AD9D0, (const void *)0x087AE3A0, (const void *)0x087AEF90,
    (const void *)0x087AFB30, (const void *)0x087B08A8, (const void *)0x087B1450, (const void *)0x087B2080,
    (const void *)0x087B2BD8, (const void *)0x087B3A44, (const void *)0x087B46FC, (const void *)0x087B528C,
    (const void *)0x087B5EB0, (const void *)0x087B69CC, (const void *)0x087B756C, (const void *)0x087B7E9C,
    (const void *)0x087E74C0, (const void *)0x087B886C, (const void *)0x087B912C, (const void *)0x087B997C,
    (const void *)0x087BA134, (const void *)0x087BA944, (const void *)0x087BB16C, (const void *)0x087BBA74,
    (const void *)0x087BC1B4, (const void *)0x087BC960, (const void *)0x087BD128, (const void *)0x087BD8AC,
    (const void *)0x087BE178, (const void *)0x087BE9DC, (const void *)0x087BF2D4, (const void *)0x087BFA38,
    (const void *)0x087C01B8, (const void *)0x087C0924, (const void *)0x087C1160, (const void *)0x087C18C4,
    (const void *)0x087C2130, (const void *)0x087C2944, (const void *)0x087C311C, (const void *)0x087C38D0,
    (const void *)0x087C409C, (const void *)0x087C4828, (const void *)0x087C4F80, (const void *)0x087C573C,
};

const u8 gMonIconPaletteIndices[440] =
{
    0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x01, 0x01, 0x00, 0x01, 0x02, 0x02, 0x00, 0x00, 0x00, 0x02,
    0x01, 0x00, 0x00, 0x02, 0x02, 0x02, 0x00, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x00, 0x00, 0x02, 0x01, 0x00,
    0x00, 0x02, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01, 0x02, 0x01, 0x02, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x02, 0x02, 0x02, 0x00, 0x02, 0x00, 0x01, 0x01, 0x01, 0x02, 0x02, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x01, 0x02, 0x02,
    0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x02, 0x02, 0x01, 0x02, 0x02, 0x01, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x02, 0x02, 0x00, 0x01, 0x02, 0x01, 0x00, 0x02, 0x02, 0x00, 0x00, 0x02, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x02, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02, 0x02, 0x00, 0x00, 0x01, 0x00, 0x02, 0x02, 0x00, 0x00, 0x00, 0x01, 0x02, 0x02, 0x00, 0x00, 0x02,
    0x00, 0x00, 0x01, 0x02, 0x02, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x01, 0x01, 0x01, 0x00, 0x00, 0x02, 0x02, 0x02, 0x00,
    0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x02, 0x02, 0x00, 0x00, 0x02, 0x00, 0x00, 0x01, 0x02, 0x00, 0x00, 0x02, 0x00, 0x00,
    0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x01, 0x02, 0x02, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x00, 0x02, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x02, 0x02, 0x00, 0x02, 0x00, 0x02, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x00, 0x01, 0x01, 0x01, 0x02, 0x02, 0x01, 0x01, 0x01, 0x00, 0x00, 0x02, 0x00, 0x02, 0x00, 0x00, 0x02, 0x01, 0x01, 0x00,
    0x00, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x02, 0x01, 0x01, 0x00, 0x01,
    0x00, 0x02, 0x02, 0x00, 0x01, 0x01, 0x02, 0x00, 0x01, 0x00, 0x02, 0x00, 0x02, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x01, 0x01, 0x02, 0x01, 0x00, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
    0x00, 0x01, 0x02, 0x02, 0x02, 0x00, 0x00, 0x02, 0x02, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x02, 0x02, 0x00, 0x00, 0x00,
    0x00, 0x02, 0x02, 0x02, 0x02, 0x00, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

// Number of parameter bytes following each extended control code.
const u8 gExtCtrlCodeLengths[23] =
{
    0x01, 0x02, 0x02, 0x02, 0x04, 0x02, 0x02, 0x01, 0x02, 0x01, 0x01, 0x03, 0x02, 0x02, 0x02, 0x01, 0x03, 0x02, 0x02, 0x02, 0x02, 0x01, 0x01
};

// Map ids where coupon communication is allowed.
const u16 gRSPokemonCenterMaps[36] =
{
    0x0202, 0x0203, 0x0301, 0x0302, 0x0405, 0x0406, 0x0503, 0x0504, 0x0603, 0x0604, 0x0700, 0x0701,
    0x0804, 0x0805, 0x090A, 0x090B, 0x0A05, 0x0A06, 0x0B05, 0x0B06, 0x0C02, 0x0C03, 0x0D06, 0x0D07,
    0x0E03, 0x0E04, 0x0F02, 0x0F03, 0x100C, 0x100D, 0x100A, 0x1918, 0x1919, 0x191A, 0x191B, 0xFFFF,
};

const u16 gRSLobbyMaps[2] = {0x1A05, 0xFFFF};
