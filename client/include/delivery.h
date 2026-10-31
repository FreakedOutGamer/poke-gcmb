#ifndef GUARD_DELIVERY_H
#define GUARD_DELIVERY_H

// Results of giving the Jirachi, each named after the message
// PrintErrorMessage shows for it. AgbMain also reports them to the GameCube.
#define DELIVERY_SUCCESS                    1  // "You receive jirachi"
#define DELIVERY_ERROR_NO_SAVE_DATA_UNUSED  2  // "Error! No savedata." (nothing returns this)
#define DELIVERY_ERROR_SAVE_CORRUPTED       3  // "Error! Destroyed flashdata."
#define DELIVERY_ERROR_NOT_POKEMON_CARTRIDGE 4 // "Not a Pokémon Cartridge." (nothing returns this)
#define DELIVERY_ERROR_NO_SAVE_DATA         5  // "Error! No savedata."
#define DELIVERY_ERROR_SAVE_FAILED          6  // "Failed to save." (nothing returns this)
#define DELIVERY_ERROR_PROCEED_GAME         7  // "Error! Proceed game."
#define DELIVERY_ERROR_PARTY_FULL           8  // "Error! 6 Pokémon"
#define DELIVERY_ERROR_NO_FLASH             9  // "Error! Destroyed flashdata." (see PrintErrorMessage)
#define DELIVERY_ERROR_ALREADY_DELIVERED    10 // "Jirachi has already been delivered before."
#define DELIVERY_ERROR_UNSUPPORTED_VERSION  11 // "This system doesn't support this version."

#if REVISION == 1
void SlideSaveIcon(s32 numFrames, s32 numLaps);
void ShowReceivedJirachi(void);
#endif


// delivery.c
s32 DeliverJirachi(void);
#if REVISION == 0
void UpdateSaveIcon(void);
#else
// Takes a position in the later revision.
void UpdateSaveIcon(s32 xStep, s32 angleStep);
#endif

// give_jirachi.c
s32 TryGiveJirachi(void);

// jirachi.c
bool32 CanReceiveJirachi(void);
s32 GiveJirachi(void);

#endif // GUARD_DELIVERY_H
