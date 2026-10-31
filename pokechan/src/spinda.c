#include "global.h"
#include "pokemon.h"
#include "target_game.h"
#include "constants/species.h"
const struct SpindaSpot gSpindaSpotGraphics[4] =
{
    {16, 14, INCBIN_U16("graphics/spinda_spots/spot_0.bin")},
    {40, 15, INCBIN_U16("graphics/spinda_spots/spot_1.bin")},
    {22, 32, INCBIN_U16("graphics/spinda_spots/spot_2.bin")},
    {34, 33, INCBIN_U16("graphics/spinda_spots/spot_3.bin")},
};
#define SPINDA_SPOT_HEIGHT 16
#define SPINDA_SPOT_WIDTH 16
#define TILE_SIZE_4BPP 32
#define FIRST_SPOT_COLOR 1
#define LAST_SPOT_COLOR 3
#define SPOT_COLOR_ADJUSTMENT 4
void UNUSED DrawSpindaSpots(u16 species, u32 personality, u8 *dest)
{
    if (species == SPECIES_SPINDA)
    {
        s32 i, row, column;
        u32 yOffset;

        if (gTargetIsRubySapphire != FALSE)
            yOffset = 0;
        else
            yOffset = 7;

        for (i = 0; i < 4; i++)
        {
            u8 x = gSpindaSpotGraphics[i].x + ((personality & 0x0F) - 8);
            u8 y = gSpindaSpotGraphics[i].y + (((personality & 0xF0) >> 4) - 8) - yOffset;

            for (row = 0; row < SPINDA_SPOT_HEIGHT; row++)
            {
                s32 spotPixelRow = gSpindaSpotGraphics[i].image[row];

                for (column = x; column < x + SPINDA_SPOT_WIDTH; column++)
                {
                    u8 *destPixels  = dest + ((column / 8) * TILE_SIZE_4BPP) +
                                             ((column % 8) / 2) +
                                                ((y / 8) * TILE_SIZE_4BPP * 8) +
                                                ((y % 8) * 4);

                    if (spotPixelRow & 1)
                    {
                        if (column & 1)
                        {
                            if ((u8)((*destPixels & 0xF0) - (FIRST_SPOT_COLOR << 4)) <= ((LAST_SPOT_COLOR - FIRST_SPOT_COLOR) << 4))
                                *destPixels  += (SPOT_COLOR_ADJUSTMENT << 4);
                        }
                        else
                        {
                            if ((u8)((*destPixels & 0xF) - FIRST_SPOT_COLOR) <= (LAST_SPOT_COLOR - FIRST_SPOT_COLOR))
                                *destPixels  += SPOT_COLOR_ADJUSTMENT;
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
