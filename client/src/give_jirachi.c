#include "global.h"
#include "save.h"
#include "target_game.h"
#include "delivery.h"
#include "berry_fix.h"
#include "text.h"

#if REVISION == 1
// The later revision also runs the Berry Program update on the save.
extern u8 gBerryFixStatus;
#endif

// Checks that the connected game can take a Jirachi, then gives it. Returns
// DELIVERY_SUCCESS or an error for PrintErrorMessage.
s32 TryGiveJirachi(void)
{
    s32 type;
    s32 status;

    // Only English versions are supported.
    gTargetGameType = type = GetTargetGameType();
#if REVISION == 1
    gBerryFixStatus = BERRY_FIX_NOT_RUN;
#endif
    if ((type & 0xF) != 1)
        return DELIVERY_ERROR_UNSUPPORTED_VERSION;
#if REVISION == 1
    // The later revision also drops FireRed/LeafGreen, although the checks
    // for them below are still there.
    if ((type & 0xF0) != 0x10)
        return DELIVERY_ERROR_UNSUPPORTED_VERSION;
#endif
    InitSaveBlockChunks();
    if (gGFRomHeaderFlags & GF_HEADER_BLOCK_JIRACHI_DISTRIBUTION)
        return DELIVERY_ERROR_UNSUPPORTED_VERSION;
    if (CheckForFlashMemory() != TRUE)
        return DELIVERY_ERROR_NO_FLASH;
    status = LoadGameSave();
    if (status != SAVE_STATUS_OK)
        return SaveStatusToDeliveryError(status);
#if REVISION == 1
    gBerryFixStatus = BerryFix(gSaveBlock2Buffer, gSaveBlock1Buffer);
#endif
    if (!CanReceiveJirachi())
        return DELIVERY_ERROR_ALREADY_DELIVERED;

    // The player must have progressed far enough in the game.
    if ((gTargetGameType & 0xF0) == 0x20)
    {
        if (!IsNationalPokedexEnabled())
            return DELIVERY_ERROR_PROCEED_GAME;
        if (!(gSaveBlock2Buffer[gWarpFlagsOffset] & 0x80)) // CHAMPION_SAVEWARP
            return DELIVERY_ERROR_PROCEED_GAME;
    }
    else if ((gTargetGameType & 0xF0) == 0x10)
    {
        if (!HasPokedex())
            return DELIVERY_ERROR_PROCEED_GAME;
    }
    else if (!HasHallOfFameRecord())
    {
        return DELIVERY_ERROR_PROCEED_GAME;
    }
    return GiveJirachi();
}
