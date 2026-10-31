#include "global.h"
#include "pokemon.h"
#include "constants/global.h"
#include "constants/items.h"
#include "constants/pokemon.h"
#include "constants/species.h"
#include "save.h"
#include "target_game.h"
#include "delivery.h"

// The gift's fixed data. Its held item is overwritten with a random berry
// each time a Jirachi is made.
struct JirachiTemplate
{
    /*0x00*/ u8 level;
    /*0x02*/ u16 species;
    /*0x04*/ u16 moves[4];
    /*0x0C*/ u16 heldItem;
    /*0x0E*/ u8 unused[10];
};

struct JirachiOriginalTrainer
{
    /*0x00*/ const u8 *name;
    /*0x04*/ u32 id;
    /*0x08*/ u8 gender;
};

#define PARTY_SIZE 6

// Not const: CreateJirachi stores the random berry in it.
struct JirachiTemplate gJirachiTemplate =
{
    .level = 5,
    .species = SPECIES_JIRACHI,
    .heldItem = ITEM_GANLON_BERRY,
};

#if REVISION == 0
static const u8 sOriginalTrainerName[] = _("METEOR");

const struct JirachiOriginalTrainer gJirachiOriginalTrainer =
{
    .name = sOriginalTrainerName,
    .id = 30719,
    .gender = MALE,
};
#else
static const u8 sOriginalTrainerName[] = _("WISHMKR");

const struct JirachiOriginalTrainer gJirachiOriginalTrainer =
{
    .name = sOriginalTrainerName,
    .id = 20043,
    .gender = MALE,
};
#endif
extern u16 gFirstSectorChecksum;

// Random() is used here as if it returned int (no zero extension, signed
// division), most likely because the original called it without a
// prototype.
s32 Random(void);
void SeedRng(u16 seed);

void CreateJirachi(struct Pokemon *mon)
{
    u32 otId = gJirachiOriginalTrainer.id;
    u16 species;
    u32 personality;
    u8 i;
    u16 heldItem;
    u8 heldItemBytes[2];

    // Seeded from the save's first sector checksum, so each save gets its
    // own Jirachi.
    SeedRng(gFirstSectorChecksum);
    ZeroMonData(mon);
    species = SPECIES_JIRACHI;
    personality = (Random() << 16) | Random();
    CreateMon(mon, species, gJirachiTemplate.level, 32, TRUE, personality, 1, otId);

    if (gJirachiOriginalTrainer.name != NULL)
    {
        SetMonData(mon, MON_DATA_OT_NAME, gJirachiOriginalTrainer.name);
        SetMonData(mon, MON_DATA_OT_GENDER, &gJirachiOriginalTrainer.gender);
    }

    for (i = 0; i < 4; i++)
    {
        if (gJirachiTemplate.moves[i] != 0)
            GiveMoveToMon(mon, gJirachiTemplate.moves[i]);
    }

    if ((Random() / 3) & 1)
        gJirachiTemplate.heldItem = ITEM_GANLON_BERRY;
    else
        gJirachiTemplate.heldItem = ITEM_SALAC_BERRY;

    heldItem = gJirachiTemplate.heldItem;
    if (heldItem != 0)
    {
        heldItemBytes[0] = heldItem;
        heldItemBytes[1] = heldItem >> 8;
        SetMonData(mon, MON_DATA_HELD_ITEM, heldItemBytes);
    }
}

// Marks Jirachi as seen (in all three of the game's copies of the flag) and
// caught.
void SetJirachiPokedexFlags(void)
{
    u8 index = (NATIONAL_DEX_JIRACHI - 1) / 8;
    u8 mask = 1 << ((NATIONAL_DEX_JIRACHI - 1) % 8);

    ((struct Pokedex *)&gSaveBlock2Buffer[gPokedexOffset])->seen[index] |= mask;
    gSaveBlock1Buffer[gDexSeen2Offset + index] |= mask;
    gSaveBlock1Buffer[gDexSeen3Offset + index] |= mask;
    ((struct Pokedex *)&gSaveBlock2Buffer[gPokedexOffset])->owned[index] |= mask;
}

bool32 CanReceiveJirachi(void)
{
    if (((struct ExternalEventFlags *)&gSaveBlock1Buffer[gExternalEventFlagsOffset])->receivedGCNJirachi)
        return FALSE;
    return TRUE;
}

void SetReceivedJirachiFlag(void)
{
    ((struct ExternalEventFlags *)&gSaveBlock1Buffer[gExternalEventFlagsOffset])->receivedGCNJirachi = TRUE;
}

// Adds a Jirachi to the end of the party and saves. Returns DELIVERY_SUCCESS
// or an error for PrintErrorMessage.
s32 GiveJirachi(void)
{
    s32 partyCount = GetPartyCount();

    if ((u32)partyCount > PARTY_SIZE)
        return DELIVERY_ERROR_SAVE_CORRUPTED;
    if (partyCount == 0)
        return DELIVERY_ERROR_PROCEED_GAME;
    if (partyCount >= PARTY_SIZE)
        return DELIVERY_ERROR_PARTY_FULL;

    CreateJirachi((struct Pokemon *)&gSaveBlock1Buffer[gPartyOffset + partyCount * sizeof(struct Pokemon)]);
    gSaveBlock1Buffer[gPartyCountOffset]++;
    SetJirachiPokedexFlags();
    SetReceivedJirachiFlag();
    WriteSaveSectorOrSlot(gSaveBlockChunks);
    return DELIVERY_SUCCESS;
}

