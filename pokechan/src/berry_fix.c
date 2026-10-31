#include "global.h"
#include "berry_fix.h"
#include "rtc.h"
#include "target_game.h"

static const char sVersionData[][2] =
{
    {'J', 1},
    {'E', 2},
    {'D', 1},
    {'F', 1},
    {'I', 1},
    {'S', 1},
};

static const char sRubyTitleAndCode[ROM_GAME_TITLE_LEN + 1] = "POKEMON RUBYAXV";
static const char sSapphireTitleAndCode[ROM_GAME_TITLE_LEN + 1] = "POKEMON SAPPAXP";

// Never incremented in this build; see BerryFix below.
static s32 sInitialWaitTimer;

static bool32 CheckSameString(const char *a, const char *b, u32 size)
{
    s32 i;

    for (i = 0; i < size; i++)
    {
        if (a[i] != b[i])
            return FALSE;
    }

    return TRUE;
}

static s32 ValidateGameVersion(void)
{
    // Declared in this order for the stack slots the original uses;
    // pret/berry-fix initializes them where they are declared.
    s32 i;
    s32 shouldUpdate;
    s32 languageCode;
    s32 softwareVersion;

    languageCode = RomHeaderGameCode[3];
    softwareVersion = *RomHeaderSoftwareVersion;
    shouldUpdate = -1;

    // Check rom header data to see if games of this
    // language and revision need the berry fix.
    for (i = 0; i < ARRAY_COUNT(sVersionData); i++)
    {
        if (languageCode != sVersionData[i][0])
            continue;

        if (softwareVersion >= sVersionData[i][1])
            shouldUpdate = FALSE;
        else
            shouldUpdate = TRUE;
        break;
    }
    if (shouldUpdate == -1)
        return INVALID;

    // A valid language/revision was found, check game title
    // and code to see if it's Ruby or Sapphire

    if (CheckSameString(RomHeaderGameTitle, sRubyTitleAndCode, ROM_GAME_TITLE_LEN) == TRUE)
    {
        if (shouldUpdate == FALSE)
        {
            return NO_UPDATE_RUBY;
        }
        else
        {
            gGameVersion = VERSION_RUBY;
            return UPDATE_RUBY;
        }
    }
    if (CheckSameString(RomHeaderGameTitle, sSapphireTitleAndCode, ROM_GAME_TITLE_LEN) == TRUE)
    {
        if (shouldUpdate == FALSE)
        {
            return NO_UPDATE_SAPPHIRE;
        }
        else
        {
            gGameVersion = VERSION_SAPPHIRE;
            return UPDATE_SAPPHIRE;
        }
    }
    return INVALID;
}

s32 ValidateRomHeader(void)
{
    if (RomHeaderMakerCode[0] != '0')
        return INVALID;
    if (RomHeaderMakerCode[1] != '1')
        return INVALID;
    if (*RomHeaderMagic != ROM_HEADER_MAGIC)
        return INVALID;

    return ValidateGameVersion();
}

// Differences from pret/berry-fix:
//  - There is no message box, because the GameCube side shows the progress.
//  - The 180-frame wait before the header check is gone, though
//    sInitialWaitTimer is still cleared.
//  - STATE_CHECK_FLASH and STATE_READ_SAVE do nothing at all, because the
//    Jirachi half has already identified the flash and loaded the save by
//    the time this runs.
//
// NOTE: STATE_NO_NEED_TO_FIX and STATE_ERROR_YEAR are written as two cases
// with a break each, but both end up pointing at the same branch in the jump
// table, because agbcc redirects the second one onto the first. The branch
// it left behind is dead.
void BerryFix(u32 *state, void *unused1, void *unused2)
{
    u8 year;
    s32 headerStatus;

    switch (*state)
    {
    case STATE_INIT:
        sInitialWaitTimer = 0;
        gUpdateSuccessful = 0;
        headerStatus = ValidateRomHeader();
        switch (headerStatus)
        {
        case INVALID:
            *state = STATE_ERROR;
            break;
        case NO_UPDATE_SAPPHIRE:
        case NO_UPDATE_RUBY:
            *state = STATE_NO_NEED_TO_FIX;
            break;
        case UPDATE_SAPPHIRE:
        case UPDATE_RUBY:
            ++(*state); // STATE_CHECK_RTC
            break;
        }
        break;
    case STATE_CHECK_RTC:
        if (!BerryFix_TryInitRtc())
            *state = STATE_ERROR;
        else
            ++(*state); // STATE_CHECK_FLASH
        break;
    case STATE_CHECK_FLASH:
        ++(*state); // STATE_READ_SAVE
        break;
    case STATE_READ_SAVE:
        ++(*state); // STATE_CHECK_TIME
        break;
    case STATE_CHECK_TIME:
        if (BerryFix_CalcTimeDifference(&year) == TRUE)
        {
            // Time difference is okay, only fix the date if
            // the Berry Glitch hasn't happened yet (if year is 2000)
            if (year == 0)
                ++(*state); // STATE_FIX_DATE
            else
                *state = STATE_CHECK_PACIFIDLOG_TM;
        }
        else
        {
            // Time difference is incorrect, if the year is 2001
            // then the Berry Glitch is occurring. If the year is
            // not 2001 then some error has occurred.
            if (year != 1)
                *state = STATE_ERROR_YEAR;
            else
                ++(*state); // STATE_FIX_DATE
        }
        break;
    case STATE_FIX_DATE:
        // Set the clock forward to fix the Berry Glitch
        BerryFix_TryFixDate();
        gUpdateSuccessful |= 1;
        *state = STATE_CHECK_PACIFIDLOG_TM;
        break;
    case STATE_CHECK_PACIFIDLOG_TM:
        if (BerryFix_IsPacifidlogTMCorrect() == TRUE)
            *state = STATE_FINISHED;
        else
            *state = STATE_FIX_PACIFIDLOG_TM;
        break;
    case STATE_FIX_PACIFIDLOG_TM:
        if (BerryFix_ResetPacifidlogTM() == TRUE)
        {
            gUpdateSuccessful |= 1;
            *state = STATE_FINISHED;
        }
        else
        {
            *state = STATE_ERROR;
        }
        break;
    case STATE_FINISHED:
        *state = STATE_NO_NEED_TO_FIX;
        break;
    case STATE_NO_NEED_TO_FIX:
        break;
    case STATE_ERROR_YEAR:
        break;
    case STATE_ERROR:
        break;
    }
}
