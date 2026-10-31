#include "global.h"
#include "berry_fix.h"
#include "rtc.h"
#include "libpmagb/save_sector.h"

// Unused. agbcc keeps all three because the file is built with no -O, which
// is also why each one stores its argument and branches to its own epilogue.
//
// Difference from pret/berry-fix: EmptyFunc1 takes an argument there it does
// not, and all three bodies are an explicit "return;" rather than empty.
static void UNUSED EmptyFunc1(u8 a)
{
    return;
}

// Unused
static void UNUSED EmptyFunc2(void)
{
    return;
}

// Unused
static void UNUSED EmptyFunc3(void)
{
    return;
}

static u16 *GetVarPointer(u16 id)
{
    if (id < VARS_START)
        return NULL;
    if (id < SPECIAL_VARS_START)
        return &gVarsPtr[id - VARS_START];
    return NULL;
}

bool32 BerryFix_IsPacifidlogTMCorrect(void)
{
    u8 year;
    u16 *tmReceivedDay = GetVarPointer(VAR_PACIFIDLOG_TM_RECEIVED_DAY);

    BerryFix_CalcTimeDifference(&year);
    if (*tmReceivedDay <= gRtcUTCTime.days)
        return TRUE;
    else
        return FALSE;
}

bool32 BerryFix_ResetPacifidlogTM(void)
{
    u16 *tmReceivedDay;
    u8 year;

    if (BerryFix_IsPacifidlogTMCorrect() == TRUE)
        return TRUE;

    BerryFix_CalcTimeDifference(&year);
    if (gRtcUTCTime.days < 0)
        return FALSE;

    tmReceivedDay = GetVarPointer(VAR_PACIFIDLOG_TM_RECEIVED_DAY);
    *tmReceivedDay = 1;
    // pret/berry-fix goes through BerryFix_TrySave(SAVE_NORMAL) and two
    // wrappers under it; this build calls the save routine directly.
    if (WriteSaveSlot() != SAVE_STATUS_OK)
        return FALSE;
    return TRUE;
}
