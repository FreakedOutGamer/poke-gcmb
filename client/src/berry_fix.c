#include "global.h"
#include "berry_fix.h"
#include "rtc.h"
#include "save.h"

// The Berry Program Update, pret/berry-fix's payload folded into a single
// call: BerryFix checks the connected Ruby/Sapphire, fixes its clock and the
// Pacifidlog TM day, and reports whether the save needs rewriting. Only the
// later revision has it.

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
    s32 languageCode = RomHeaderGameCode[3];
    s32 softwareVersion = *RomHeaderSoftwareVersion;
    s32 shouldUpdate = -1;
    s32 i;

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
            return NO_UPDATE_RUBY;
        else
            return UPDATE_RUBY;
    }
    if (CheckSameString(RomHeaderGameTitle, sSapphireTitleAndCode, ROM_GAME_TITLE_LEN) == TRUE)
    {
        if (shouldUpdate == FALSE)
            return NO_UPDATE_SAPPHIRE;
        else
            return UPDATE_SAPPHIRE;
    }
    return INVALID;
}

static s32 ValidateRomHeader(void)
{
    if (RomHeaderMakerCode[0] != '0')
        return INVALID;
    if (RomHeaderMakerCode[1] != '1')
        return INVALID;
    if (*RomHeaderMagic != ROM_HEADER_MAGIC)
        return INVALID;

    return ValidateGameVersion();
}

static bool32 NeedsBerryFix(void)
{
    s32 status = ValidateRomHeader();

    if (status == UPDATE_SAPPHIRE)
        return TRUE;
    if (status == UPDATE_RUBY)
        return TRUE;
    return FALSE;
}

// The Berry Glitch can leave a nonsense day count in this var, which stops
// the TM from ever being given again.
static bool32 IsPacifidlogTMDayInvalid(void)
{
    if (*(u16 *)(gSaveBlock1Ptr + PACIFIDLOG_TM_DAY_OFFSET) > 0x4FFE)
        return TRUE;
    else
        return FALSE;
}

// BUG: Declared to return a value, but never does (BerryFix ignores it).
static bool32 ResetPacifidlogTMDay(void)
{
    *(u16 *)(gSaveBlock1Ptr + PACIFIDLOG_TM_DAY_OFFSET) = 1;
}

// Returns a BERRY_FIX_* status.
s32 BerryFix(u8 *saveBlock2, u8 *saveBlock1)
{
    u8 year;
    bool32 resetTMDay = FALSE;

    if (!NeedsBerryFix())
        return BERRY_FIX_NOT_NEEDED;
    if (!BerryFix_TryInitRtc())
        return BERRY_FIX_FAILED;

    gSaveBlock2Ptr = saveBlock2;
    gSaveBlock1Ptr = saveBlock1;
    if (BerryFix_CalcTimeDifference(&year) == TRUE)
    {
        // Only fix the date if the Berry Glitch hasn't happened yet.
        if (year != 0)
            return BERRY_FIX_NOT_NEEDED;
    }
    else if (year == 0)
    {
        return BERRY_FIX_FAILED;
    }

    if (IsPacifidlogTMDayInvalid())
    {
        ResetPacifidlogTMDay();
        resetTMDay = TRUE;
    }
    BerryFix_TryFixDate();
    if (resetTMDay)
        return BERRY_FIX_SAVED;
    else
        return BERRY_FIX_UPDATED;
}
