#include "global.h"
#include "test_screen.h"

void AgbMain(void)
{
    REG_IME = 0;
    // The program runs from EWRAM, so everything else is cleared.
    RegisterRamReset(RESET_ALL & ~RESET_EWRAM);
    REG_WAITCNT = WAITCNT_SRAM_4 | WAITCNT_WS0_N_3 | WAITCNT_WS0_S_1
                | WAITCNT_WS1_N_3 | WAITCNT_WS1_S_1 | WAITCNT_WS2_N_3 | WAITCNT_WS2_S_1
                | WAITCNT_PREFETCH_ENABLE;
    REG_IE = INTR_FLAG_VBLANK;
    REG_DISPSTAT = DISPSTAT_VBLANK_INTR;
    REG_IME = 1;

    RunTestScreen();
    SoftReset(RESET_ALL);
}
