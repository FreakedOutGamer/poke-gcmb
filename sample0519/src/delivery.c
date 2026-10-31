#include <string.h>
#include "global.h"
#include "gift.h"
#include "joybus.h"
#include "main.h"
#include "pokemon.h"
#include "pokemon_data.h"
#include "rtc.h"
#include "save_blocks.h"
#include "constants/items.h"
#include "constants/pokemon.h"
#include "constants/species.h"
#include "delivery.h"
#include "pokedex_util.h"
#include "save.h"
#include "text.h"

// The delivery itself, which checks the cartridge and its save, gives
// Jirachi and saves. Each step is a callback on the sub-callback stack, and
// PrintMessage prints the message in the state's buffer.

#define HIHALF(n) (((n) & 0xFFFF0000) >> 16)
#define LOHALF(n) ((n) & 0xFFFF)
#define GET_SHINY_VALUE(otId, personality) (HIHALF(otId) ^ LOHALF(otId) ^ HIHALF(personality) ^ LOHALF(personality))

#define SAVE_STATUS_EMPTY    0
#define SAVE_STATUS_OK       1
#define SAVE_STATUS_CORRUPT  2
#define SAVE_STATUS_NO_FLASH 4
#define SAVE_STATUS_ERROR    0xFF

#define GAME_STAT_RECEIVED_RIBBONS 42

#define VARS_START 0x4000
#define SYSTEM_FLAGS 0x800
#define FLAG_SYS_NATIONAL_DEX (SYSTEM_FLAGS + 0x36)
#define FLAG_SYS_RIBBON_GET   (SYSTEM_FLAGS + 0x3B)

#define ROM_FIXED_VALUE (*(u8 *)0x080000B2)
#define ROM_GAME_CODE   ((u8 *)0x080000AC)

struct DeliveryState
{
    bool32 done;
    u16 angle;
    u8 message[0x82];
    u16 tilemap[0x400];
};

extern struct DeliveryState *gDeliveryState;
extern u8 gUnusedDeliveryByte;
extern struct DeliveryState gDeliveryStateBuffer;
extern struct MailStruct gGiftMail;

// Defined const, but written to (see CreateGiftJirachi).
extern struct GiftMon gJirachiGift;
extern const u8 gText_NoCartridge[];
extern const u8 gText_NotPokemonCartridge[];
extern const u8 gText_ReceivingJirachi[];
extern const u8 gGameCode_Ruby[];
extern const u8 gGameCode_Sapphire[];
extern const u8 gText_ErrorNoFlashRam[];
extern const u8 gText_ErrorNoSaveData[];
extern const u8 gText_ErrorDestroyedFlashData[];
extern const u8 gText_ErrorProceedGame[];
extern const u8 gText_Error6Pokemon[];
extern const u8 gText_ReceivedJirachi[];
extern const u8 gText_FailedToSave[];
extern const u8 gText_ReceivedJirachiTurnOff[];
extern const u16 gDeliveryBgPalette[];
extern const u16 gJirachiOamData[];
extern const u16 gJirachiPalette[];
extern const u8 gJirachiGfx[];
extern const u8 gFrameGfx[];
extern const u16 gFrameTilemap[];

void SoftResetRom(u32 resetFlags);

// The original calls these with only two arguments (no data pointer),
// which compiled through an unprototyped declaration.
#define GetBoxMonDataNoArg(boxMon, field) ((u32 (*)())GetBoxMonData)(boxMon, field)
#define GetMonDataNoArg(mon, field) ((u32 (*)())GetMonData)(mon, field)
// Declared without prototypes, as the original evidently did. Its callers
// use Random's result and pass SeedRng's argument as full 32-bit values,
// with none of the zero-extension libpmagb/random.h's u16 types would add.
// Random only returns 16 bits, so nothing changes at run time.
u32 Random();
void SeedRng();

void PrintMessage(struct DeliveryState *state);
void GrowJirachiSprite(struct DeliveryState *state);
void CheckSaveData(struct DeliveryState *state);
void StartGivingJirachi(struct DeliveryState *state);
void GiveJirachi(struct DeliveryState *state);
void SaveJirachi(struct DeliveryState *state);
void ShowTurnOffMessage(struct DeliveryState *state);
void ResetGame(void);
void CheckCartridge(struct DeliveryState *state);

// Waits for one of the keys in the callback's argument.
void WaitForKeys(void)
{
    if (gKeys.newKeys & gSubCallbacks.args[gSubCallbacks.count - 1])
        PopSubCallback();
}

// Prints the message in the lower part of the screen, each line centered.
// Then waits for the keys in the callback's argument, if any.
void PrintMessage(struct DeliveryState *state)
{
    s32 len;
#ifdef NONMATCHING
    s32 width;
#else
    register s32 width asm("r4");
#endif
    u8 *line;
    s32 y;
    bool32 more;

    FillTilemapRect(state->tilemap, 0, 13, 32, 7, 0, 0);
    len = 0;
    width = 0;
    // FAKEMATCH: the loop around this assignment raises state's allocation
    // priority above len's, which puts state in r6 and len in r7.
    do
    {
        line = state->message;
    } while (0);
    y = 13;
    more = TRUE;
    do
    {
        if ((s8)line[len] >= 0)
        {
            switch (line[len])
            {
            default:
                len++;
                break;
            case 0:
                more = FALSE;
            case '\n':
                line[len] = 0;
                PrintfTilemap(state->tilemap, (28 - width) / 2 + 2, y, 0, line);
                line += len + 1;
                len = 0;
                y += 2;
                width = 0;
                continue;
            }
        }
        else
        {
            len += 2;
        }
        width++;
    } while (more);
    RequestCopy(BG_SCREEN_BASE(gIoRegs.bgcnt[0]), state->tilemap, 0x800);
    if (gSubCallbacks.args[gSubCallbacks.count - 1] != 0)
        ReplaceSubCallback(WaitForKeys, gSubCallbacks.args[gSubCallbacks.count - 1]);
    else
        PopSubCallback();
}

// Grows the Jirachi sprite until it is full size.
void GrowJirachiSprite(struct DeliveryState *state)
{
    s16 angle;

    angle = gSubCallbacks.args[gSubCallbacks.count - 1] + 4;
    gSubCallbacks.args[gSubCallbacks.count - 1] = angle;
    state->angle = angle;
    if (angle > 0xFF)
        PopSubCallback();
}

void CheckCartridge(struct DeliveryState *state)
{
    JoyBusHandshake();
    if (ROM_FIXED_VALUE != 0x96)
    {
        strcpy(state->message, gText_NoCartridge);
        JoyBusSendResult(2);
        ReplaceSubCallback(ResetGame, 0);
        PushSubCallback(PrintMessage, 1);
    }
    else if (memcmp(ROM_GAME_CODE, gGameCode_Ruby, 4) != 0 && memcmp(ROM_GAME_CODE, gGameCode_Sapphire, 4) != 0)
    {
        strcpy(state->message, gText_NotPokemonCartridge);
        JoyBusSendResult(1);
        ReplaceSubCallback(ResetGame, 0);
        PushSubCallback(PrintMessage, 1);
    }
    else
    {
        strcpy(state->message, gText_ReceivingJirachi);
        ReplaceSubCallback(CheckSaveData, 0);
        PushSubCallback(PrintMessage, 0);
    }
}

bool32 IsBadEgg(struct BoxPokemon *boxMon)
{
    bool8 isBadEgg = FALSE;

    if ((u16)GetBoxMonDataNoArg(boxMon, MON_DATA_SPECIES))
        isBadEgg = GetBoxMonDataNoArg(boxMon, MON_DATA_SANITY_IS_BAD_EGG) != 0;
    return isBadEgg;
}

// Returns whether the Pokémon is one of the species in gGiftSpecies (Jirachi).
bool32 IsGiftSpecies(struct BoxPokemon *boxMon)
{
    bool32 found = FALSE;
    u16 species = GetBoxMonDataNoArg(boxMon, MON_DATA_SPECIES);

    if (species != 0)
    {
        u16 i;

        for (i = 0; gGiftSpecies[i] != 0; i++)
        {
            if (species == gGiftSpecies[i])
            {
                found = TRUE;
                break;
            }
        }
    }
    return found;
}

// Returns whether the National Pokédex has been unlocked, by any of the
// save's several signs of it.
// BUG: This reads the variable numbered 0x302, far outside the save
// block, rather than checking VAR_NATIONAL_DEX for the value 0x302.
bool32 IsNationalDexEnabled(void)
{
    bool32 national = FALSE;

    if (gSaveBlock2.pokedex.mode == 1
     || gSaveBlock2.pokedex.nationalMagic == 0xDA
     || gSaveBlock1.vars[0x302 - VARS_START] != 0
     || gSaveBlock1.flags[FLAG_SYS_NATIONAL_DEX / 8] & (1 << (FLAG_SYS_NATIONAL_DEX % 8)))
        national = TRUE;
    return national;
}

// Checks the party (bit 0) and boxes (bit 1) for a Bad Egg or a Jirachi,
// and the save for the National Pokédex (bit 2).
u32 CheckForTampering(void)
{
    u32 result = 0;
    u8 i, j;

    for (i = 0; i < gPlayerPartyCount; i++)
    {
        struct BoxPokemon *boxMon = &gPlayerParty[i].box;

        if (IsBadEgg(boxMon) || IsGiftSpecies(boxMon))
            result |= 1;
    }
    for (i = 0; i < 14; i++)
    {
        for (j = 0; j < 30; j++)
        {
            struct BoxPokemon *boxMon = &((struct BoxPokemon (*)[30])0x02008004)[i][j];

            if (IsBadEgg(boxMon) || IsGiftSpecies(boxMon))
                result |= 2;
        }
    }
    if (IsNationalDexEnabled() == TRUE)
        result |= 4;
    return result;
}

bool32 IsSaveTampered(void)
{
    return CheckForTampering() != 0;
}

void CheckSaveData(struct DeliveryState *state)
{
    u8 status;
    u32 seed;

    Save_ResetSaveCounters();
    status = LoadGameSave(0);
    seed = RtcGetMinuteCount();
    SeedRng((seed >> 16) ^ (seed & 0xFFFF));
    seed = ((u32)Random() << 16) | Random();
    if (!(seed & 0xFFFF0000))
        seed |= 0x20030000;
    if (!(seed & 0xFFFF))
        seed |= 0x327;
    SeedRng2(seed);
    switch (status)
    {
    case SAVE_STATUS_OK:
        switch (gPlayerPartyCount)
        {
        default:
            // NOTE: A tampered save is handled exactly like a clean one, so
            // "データかいぞうのうたがいがあります" (the data may have been
            // tampered with) is never shown.
            if (IsSaveTampered())
            {
                ReplaceSubCallback(StartGivingJirachi, 0);
                return;
            }
            break;
        case 0:
            strcpy(state->message, gText_ErrorProceedGame);
            JoyBusSendResult(8);
            goto error;
        case PARTY_SIZE:
            strcpy(state->message, gText_Error6Pokemon);
            JoyBusSendResult(16);
            goto error;
        }
        ReplaceSubCallback(StartGivingJirachi, 0);
        return;
    case SAVE_STATUS_CORRUPT:
    case SAVE_STATUS_ERROR:
        strcpy(state->message, gText_ErrorDestroyedFlashData);
        JoyBusSendResult(32);
        break;
    // BUG: Unreachable. With no flash chip LoadGameSave returns
    // SAVE_STATUS_ERROR and only records SAVE_STATUS_NO_FLASH in
    // gSaveFileStatus, so the player is told the save data is corrupted.
    case SAVE_STATUS_NO_FLASH:
        strcpy(state->message, gText_ErrorNoFlashRam);
        JoyBusSendResult(128);
        break;
    case SAVE_STATUS_EMPTY:
        strcpy(state->message, gText_ErrorNoSaveData);
        JoyBusSendResult(4);
        break;
    }
error:
    ReplaceSubCallback(ResetGame, 0);
    PushSubCallback(PrintMessage, 8);
}

void StartGivingJirachi(struct DeliveryState *state)
{
    ReplaceSubCallback(GiveJirachi, 0);
}

// Like pokeruby's GiveMailToMon, but copies a fixed letter.
u8 GiveMailToMon(struct Pokemon *mon, u16 itemId)
{
    u8 heldItem[2];
    u8 id;
    u8 mailId = 0xFF;

    heldItem[0] = itemId;
    heldItem[1] = (itemId & 0xFF00) >> 8;
    for (id = 0; id < SAVE_MAIL_COUNT; id++)
    {
        if (gSaveBlock1.mail[id].itemId == 0)
        {
            gSaveBlock1.mail[id] = gGiftMail;
            SetMonData(mon, MON_DATA_MAIL, &id);
            SetMonData(mon, MON_DATA_HELD_ITEM, heldItem);
            mailId = id;
            break;
        }
    }
    return mailId;
}

// Unused. Creates an egg of a species picked at random, by weight, from
// gGiftEggs (Pichu with Teeter Dance or Wish, some shiny).
void UNUSED CreateGiftEgg(struct DeliveryState *state, struct Pokemon *mon)
{
    u32 otId = gSaveBlock2.playerTrainerId[0]
             | (gSaveBlock2.playerTrainerId[1] << 8)
             | (gSaveBlock2.playerTrainerId[2] << 16)
             | (gSaveBlock2.playerTrainerId[3] << 24);
    u32 personality = (Random() << 16) | Random();
    u16 value;
    u8 i;
    u8 data;
    u8 j;

    // BUG: The shiny test takes the personality's upper half where its
    // lower half belongs, so the value is 8 or more unless that upper half
    // is 0, and a shiny egg is almost never avoided.
    while ((HIHALF(otId) ^ LOHALF(otId) ^ HIHALF(personality) ^ (personality & 0xFFFF0000)) < 8)
        personality++;
    ZeroMonData(mon);
    value = 0;
    for (i = 0; gGiftEggs[i].weight != 0; i++)
        value += gGiftEggs[i].weight;
    value = Random2(value);
    for (i = 0; gGiftEggs[i].weight != 0; i++)
    {
        if (value < gGiftEggs[i].weight)
        {
            value = gGiftEggs[i].species;
            break;
        }
        value -= gGiftEggs[i].weight;
    }
    if (gGiftEggs[i].weight == 0)
        i = 0;
    if (gGiftEggs[i].shiny)
    {
        personality &= 0xFFFF0000;
        personality |= HIHALF(otId) ^ LOHALF(otId) ^ (personality >> 16);
        personality &= ~7;
        personality |= Random() & 7;
    }
    CreateMon(mon, value, gGiftEggs[i].level, 32, TRUE, personality, 1, otId);
    if (gGiftOT.name != NULL)
    {
        SetMonData(mon, MON_DATA_OT_NAME, gGiftOT.name);
        SetMonData(mon, MON_DATA_OT_GENDER, &gGiftOT.gender);
    }
    {
        u8 *buffer = &data;
        j = 0;
        *buffer = TRUE;
    }
    SetMonData(mon, MON_DATA_IS_EGG, &data);
    data = 0xFF;
    SetMonData(mon, MON_DATA_MET_LOCATION, &data);
    data = 0;
    SetMonData(mon, MON_DATA_MET_LEVEL, &data);
    data = gBaseStats[value].eggCycles;
    SetMonData(mon, MON_DATA_FRIENDSHIP, &data);
    data = 1;
    SetMonData(mon, MON_DATA_LANGUAGE, &data);
    {

        for (; j < 4; j++)
            if (gGiftEggs[i].moves[j] != 0)
                GiveMoveToMon(mon, gGiftEggs[i].moves[j]);
    }
}

// Creates the gift Jirachi, holding a Ganlon or Salac Berry. A personality
// that would be shiny for the OT is stepped until it isn't.
void CreateGiftJirachi(struct DeliveryState *state, struct Pokemon *mon)
{
    u16 species;
    u32 otId;
    u32 personality;
    u8 i;
    u8 data[2];
    u16 minute;
    u32 heldItem;
    register struct GiftMon *gift;

    if (gGiftOT.name == NULL)
        otId = gSaveBlock2.playerTrainerId[0]
             | (gSaveBlock2.playerTrainerId[1] << 8)
             | (gSaveBlock2.playerTrainerId[2] << 16)
             | (gSaveBlock2.playerTrainerId[3] << 24);
    else
        otId = gGiftOT.id;
    personality = ((u32)Random() << 16) | Random();
    while (GET_SHINY_VALUE(otId, personality) < 8)
        personality++;
    ZeroMonData(mon);
    species = SPECIES_JIRACHI;
    if (gJirachiGift.shiny)
    {
        personality &= 0xFFFF0000;
        personality |= HIHALF(otId) ^ LOHALF(otId) ^ (personality >> 16);
        personality &= ~7;
        personality |= Random() & 7;
    }
    CreateMon(mon, species, gJirachiGift.level, 32, TRUE, personality, 1, otId);
    if (gGiftOT.name != NULL)
    {
        SetMonData(mon, MON_DATA_OT_NAME, gGiftOT.name);
        SetMonData(mon, MON_DATA_OT_GENDER, &gGiftOT.gender);
    }
    for (i = 0; i < 4; i++)
        if (gJirachiGift.moves[i] != 0)
            GiveMoveToMon(mon, gJirachiGift.moves[i]);
    minute = (u16)RtcGetMinuteCount() % 4;
    if (!(minute & 1))
        gJirachiGift.heldItem = ITEM_GANLON_BERRY;
    else
        gJirachiGift.heldItem = ITEM_SALAC_BERRY;
    gift = &gJirachiGift;
    heldItem = gift->heldItem;
    if ((u16)heldItem != 0)
    {
        if ((u16)heldItem == ITEM_DREAM_MAIL)
            // Preserve the original reload of the held item.
            GiveMailToMon(mon, ((volatile struct GiftMon *)gift)->heldItem);
        else
        {
            data[0] = heldItem;
            data[1] = heldItem >> 8;
            SetMonData(mon, MON_DATA_HELD_ITEM, data);
        }
    }
    if (gJirachiGift.ribbons != NULL)
    {
        const u32 *ribbon;

        data[0] = TRUE;
        data[1] = 0;
        for (ribbon = gJirachiGift.ribbons; *ribbon != 0; data[1]++)
        {
            SetMonData(mon, *ribbon++, data);
            IncrementGameStat(GAME_STAT_RECEIVED_RIBBONS);
        }
        if (data[1] != 0)
            gSaveBlock1.flags[FLAG_SYS_RIBBON_GET / 8] |= 1 << (FLAG_SYS_RIBBON_GET % 8);
    }
    GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_SET_SEEN);
    GetSetPokedexFlag(SpeciesToNationalPokedexNum(species), FLAG_SET_CAUGHT);
}

void GiveJirachi(struct DeliveryState *state)
{
    u8 i;

    for (i = 0; i < PARTY_SIZE && GetMonDataNoArg(&gPlayerParty[i], MON_DATA_SPECIES) != 0; i++)
        ;
    if (i != PARTY_SIZE)
    {
        CreateGiftJirachi(state, &gPlayerParty[i]);
        gPlayerPartyCount++;
        CheckForTampering();
    }
    ReplaceSubCallback(SaveJirachi, 0);
}

void SaveJirachi(struct DeliveryState *state)
{
    if (TrySavingData(0) == SAVE_STATUS_OK)
    {
        strcpy(state->message, gText_ReceivedJirachi);
        ReplaceSubCallback(ShowTurnOffMessage, 0);
        PushSubCallback(GrowJirachiSprite, 0);
        PushSubCallback(PrintMessage, 0);
        JoyBusSendResult(0x1000);
    }
    else
    {
        strcpy(state->message, gText_FailedToSave);
        ReplaceSubCallback(ResetGame, 0);
        JoyBusSendResult(0x2000);
        PushSubCallback(PrintMessage, 8);
    }
}

void ShowTurnOffMessage(struct DeliveryState *state)
{
    strcpy(state->message, gText_ReceivedJirachiTurnOff);
    ReplaceSubCallback(ResetGame, 0);
    PushSubCallback(PrintMessage, 8);
}

void ResetGame(void)
{
    SoftResetRom(RESET_SIO_REGS | RESET_SOUND_REGS | RESET_REGS);
}

void InitDeliveryScene(struct DeliveryState *state)
{
    u16 unused;

    ExitForcedBlank();
    gDeliveryState->done = FALSE;
    InitInterrupts();
    EnterForcedBlank();
    InitIoRegs();
    gIoRegs.dispcnt &= 0xE07F;
    unused = 0;
    gIoRegs.dispcnt |= DISPCNT_OBJ_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON;
    gIoRegs.bgcnt[0] = BGCNT_SCREENBASE(31);
    gIoRegs.bgcnt[1] = BGCNT_SCREENBASE(28) | BGCNT_CHARBASE(1);
    FlushIoRegs();
    CheckForFlashMemory();
    LoadPalette(gDeliveryBgPalette, 0, 16);
    LoadPalette(gJirachiPalette, 0x100, 16);
    RequestCopy(BG_SCREEN_BASE(gIoRegs.bgcnt[1]), gFrameTilemap, 0x500);
    RequestCopy(BG_CHAR_BASE(gIoRegs.bgcnt[1]), gFrameGfx, 0x200);
    FillTilemapRect(state->tilemap, 0, 0, 32, 32, 0, 0);
    LoadTilemapFont(BG_CHAR_BASE(gIoRegs.bgcnt[0]), 0, 12, 13);
    RequestCopy((void *)(VRAM + 0x10000), gJirachiGfx, 0x800);
    RequestCopy(BG_SCREEN_BASE(gIoRegs.bgcnt[0]), state->tilemap, 0x800);
    ResetOam(0, 0);
    DoOamBufferTransfer();
    state->angle = 0;
    gUnusedDeliveryByte = unused;
    ExitForcedBlank();
    ClearSubCallbacks();
    PushSubCallback(CheckCartridge, 0);
}

void ReinitDeliveryScene(struct DeliveryState *state)
{
    InitDeliveryScene(state);
}

void UpdateDeliveryScene(struct DeliveryState *state)
{
    ResetOam(0, 0);
    if (state->angle != 0)
    {
        SetAffineParams(0, state->angle, state->angle, state->angle, 0);
        AddSprite(88, 24, gJirachiOamData, 0, 0);
    }
    if (gSubCallbacks.count != 0)
        gSubCallbacks.funcs[gSubCallbacks.count - 1](state);
}

void DeliveryMainCallback(void)
{
    switch (gMainCallbacks.args[gMainCallbacks.count - 1])
    {
    default:
        UpdateDeliveryScene(gDeliveryState);
        if (gDeliveryState->done)
        {
            ReplaceMainCallback(DeliveryMainCallback, 0);
            ClearSubCallbacks();
        }
        break;
    case 0:
        gMainCallbacks.args[gMainCallbacks.count - 1] = 2;
        gDeliveryState = &gDeliveryStateBuffer;
        InitDeliveryScene(gDeliveryState);
        break;
    case 1:
        gMainCallbacks.args[gMainCallbacks.count - 1] = 2;
        gDeliveryState = &gDeliveryStateBuffer;
        ReinitDeliveryScene(gDeliveryState);
        break;
    }
}
