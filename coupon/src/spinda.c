#include "global.h"
#include "constants/species.h"

#define SPINDA_SPOT_HEIGHT 16
#define SPINDA_SPOT_WIDTH  16
#define TILE_SIZE_4BPP     32

#define FIRST_SPOT_COLOR       1
#define LAST_SPOT_COLOR        3
#define SPOT_COLOR_ADJUSTMENT  4

struct SpindaSpot
{
    u8 x;
    u8 y;
    u16 image[SPINDA_SPOT_HEIGHT];
    u16 unused;
};

extern const struct SpindaSpot gSpindaSpotGraphics[4];

#define TRY_DRAW_SPOT_PIXEL(pixels, shift)                                  \
    if (((*(pixels) & (0xF << (shift))) >= (FIRST_SPOT_COLOR << (shift)))  \
     && ((*(pixels) & (0xF << (shift))) <= (LAST_SPOT_COLOR << (shift))))  \
    {                                                                       \
        *(pixels) += (SPOT_COLOR_ADJUSTMENT << (shift));                    \
    }

void UNUSED DrawSpindaSpots(u16 species, u32 personality, u8 *dest)
{
    s32 i;

    if (species == SPECIES_SPINDA)
    {
        for (i = 0; i < (s32)ARRAY_COUNT(gSpindaSpotGraphics); i++)
        {
            s32 row;
            u8 x = gSpindaSpotGraphics[i].x + ((personality & 0x0F) - 8);
            u8 y = gSpindaSpotGraphics[i].y + (((personality & 0xF0) >> 4) - 8);

            for (row = 0; row < SPINDA_SPOT_HEIGHT; row++)
            {
                s32 column;
                s32 spotPixelRow = gSpindaSpotGraphics[i].image[row];

                for (column = x; column < x + SPINDA_SPOT_WIDTH; column++)
                {
                    u8 *destPixels = dest + ((column / 8) * TILE_SIZE_4BPP)
                                          + ((column % 8) / 2)
                                          + ((y / 8) * TILE_SIZE_4BPP * 8)
                                          + ((y % 8) * 4);

                    if (spotPixelRow & 1)
                    {
                        if (column & 1)
                        {
                            TRY_DRAW_SPOT_PIXEL(destPixels, 4)
                        }
                        else
                        {
                            TRY_DRAW_SPOT_PIXEL(destPixels, 0)
                        }
                    }

                    spotPixelRow >>= 1;
                }
                y++;
            }
            personality >>= 8;
        }
    }
}
