#include "global.h"
#include "save.h"
#include "target_game.h"

int strcmp(const char *s1, const char *s2);

// Identifies the inserted cartridge from its game code: 0x10-0x12 for
// Ruby/Sapphire, 0x20-0x22 for FireRed/LeafGreen (Japanese, English, other
// languages), 0x30-0x32 for anything else.
s32 GetTargetGameType(void)
{
    char gameCode[5];
    char gameTitle[4];
    s32 i;

    DmaCopy32(3, (void *)0x080000AC, gameCode, 4);
    gameCode[4] = '\0';
    for (i = 0; i < 3; i++)
        gameTitle[i] = gameCode[i];
    gameTitle[3] = '\0';

    if ((strcmp(gameTitle, "AXP") == 0) | (strcmp(gameTitle, "AXV") == 0))
    {
        if ((strcmp(gameCode, "AXPJ") == 0) | (strcmp(gameCode, "AXVJ") == 0))
            return TARGET_GAME_RS_JAPANESE;
        if ((strcmp(gameCode, "AXPE") == 0) | (strcmp(gameCode, "AXVE") == 0))
            return TARGET_GAME_RS_ENGLISH;
        return TARGET_GAME_RS_OTHER;
    }
    if ((strcmp(gameTitle, "BPR") == 0) | (strcmp(gameTitle, "BPG") == 0))
    {
        if ((strcmp(gameCode, "BPRJ") == 0) | (strcmp(gameCode, "BPGJ") == 0))
            return TARGET_GAME_FRLG_JAPANESE;
        if ((strcmp(gameCode, "BPRE") == 0) | (strcmp(gameCode, "BPGE") == 0))
            return TARGET_GAME_FRLG_ENGLISH;
        return TARGET_GAME_FRLG_OTHER;
    }

    // BUG: this reads the terminator written above rather than the
    // language letter (gameCode[3]), so it always returns 0x32.
    if (gameCode[4] == 'J')
        return 0x30;
    if (gameCode[4] == 'E')
        return 0x31;
    return 0x32;
}

// Reads the connected game's save layout, from its GFRomHeader or, for
// Ruby/Sapphire, from values built in here, then sets the chunk sizes.
// Declared to return a value but only returns one on failure.
bool32 InitSaveBlockChunks(void)
{
    s32 i;
    u8 flags[4];

    if (gTargetGameType == TARGET_GAME_NONE)
        return FALSE;

    if (gTargetGameType == TARGET_GAME_RS_JAPANESE || gTargetGameType == TARGET_GAME_RS_ENGLISH)
    {
        gPartyCountOffset = 0x234;
        gPartyOffset = 0x238;
        gSaveBlock2Size = 0x890;
        gSaveBlock1Size = 0x3AC0;
        gPokedexOffset = 0x18;
        gDexSeen2Offset = 0x938;
        gDexSeen3Offset = 0x3A8C;
        gGFRomHeaderFlags = 0;
        gExternalEventFlagsOffset = 0x312F;
    }
    else
    {
        DmaCopy32(3, &CART_GF_ROM_HEADER->saveBlock2Size, &gSaveBlock2Size, 4);
        DmaCopy32(3, &CART_GF_ROM_HEADER->saveBlock1Size, &gSaveBlock1Size, 4);
        DmaCopy32(3, &CART_GF_ROM_HEADER->partyCountOffset, &gPartyCountOffset, 4);
        DmaCopy32(3, &CART_GF_ROM_HEADER->partyOffset, &gPartyOffset, 4);
        DmaCopy32(3, &CART_GF_ROM_HEADER->pokedexOffset, &gPokedexOffset, 4);
        DmaCopy32(3, &CART_GF_ROM_HEADER->seen1Offset, &gDexSeen2Offset, 4);
        DmaCopy32(3, &CART_GF_ROM_HEADER->seen2Offset, &gDexSeen3Offset, 4);
        DmaCopy32(3, &CART_GF_ROM_HEADER->seen2Offset, &gDexSeen3Offset, 4);
        // The flags are bitfields, so copy their whole word.
        DmaCopy32(3, (u32 *)CART_GF_ROM_HEADER + 0xB8 / 4, flags, 4);
        gGFRomHeaderFlags = flags[0];
        DmaCopy32(3, &CART_GF_ROM_HEADER->warpFlagsOffset, &gWarpFlagsOffset, 4);
        DmaCopy32(3, &CART_GF_ROM_HEADER->externalEventFlagsOffset, &gExternalEventFlagsOffset, 4);
    }

    // SaveBlock2 fills sector 0 and SaveBlock1 sectors 1-4; the rest are full.
    for (i = 0; i < NUM_SECTORS_PER_SAVE_SLOT; i++)
    {
        if (i == 0)
            gSaveBlockChunks[0].size = gSaveBlock2Size;
        else if (i == 4)
            gSaveBlockChunks[4].size = gSaveBlock1Size - SECTOR_DATA_SIZE * 3;
        else
            gSaveBlockChunks[i].size = SECTOR_DATA_SIZE;
    }
}
