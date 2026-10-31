#ifndef GUARD_PLAYER_RECORD_H
#define GUARD_PLAYER_RECORD_H

#include "global.h"
#include "pokemon.h"

// Japanese link packets reserve fewer bytes for each battle message.
#define LINK_TEXT_BUFFER_SIZE (GAME_LANGUAGE == LANGUAGE_JAPANESE ? 224 : 320)

// What the GBA reports about the connected cartridge and its save. This is
// everything the GameCube needs to show the player's party and decide what it
// may hand over. The status bits are only ever stored with strb.
struct PlayerLinkInfo
{
    /*0x000*/ u32 hasPokedex:1;
              u32 isInPokeCenter:1;
              u32 isFRLG:1;
              u32 isChampion:1;
              u32 language:4;
              u32 errorCode:2; // colbtl's Western builds: why the save is unusable
              u32 unused:15;
    /*0x004*/ u8 playerName[PLAYER_NAME_LENGTH + 1];
    /*0x00C*/ u32 playerGender;
    /*0x010*/ u8 playerTrainerId[TRAINER_ID_LENGTH];
    /*0x014*/ struct Pokemon party[PARTY_SIZE];
    /*0x26C*/ u8 giftRibbons[GIFT_RIBBONS_COUNT];
}; /*size = 0x278*/

extern struct PlayerLinkInfo gPlayerLinkInfo;
// The battle messages the GameCube sends, one per party slot.
extern u8 gTextBuffer[PARTY_SIZE][LINK_TEXT_BUFFER_SIZE];
// Cleared before a message transfer starts and set when it completes.
extern u8 gLinkCommandReady;

struct PlayerLinkInfo *SetPlayerLinkInfo(u8 *sav2, u8 *sav1, u32 saveStatus);
struct PlayerLinkInfo *GetPlayerLinkInfo(void);
u8 *GetTextBufferPointer(u32 id);

#endif // GUARD_PLAYER_RECORD_H
