#ifndef GUARD_COLBTL_H
#define GUARD_COLBTL_H

// The part of the save's ExternalEventData the GameCube exchanges. coupon.bin
// has only the Poké Coupon words, without the two before them.
struct ExternalEventData2
{
#ifndef PROGRAM_COUPON
    u32 unknownExternalDataFields1;
    u32 unknownExternalDataFields2;
#endif
    u32 currentPokeCoupons:24;
    u32 gotGoldPokeCouponTitleReward:1;
    u32 gotSilverPokeCouponTitleReward:1;
    u32 gotBronzePokeCouponTitleReward:1;
    u32 receivedAgetoCelebi:1;
    u32 unknownExternalDataFields3:4;
    u32 totalEarnedPokeCoupons:24;
    u32 unknownExternalDataFields4:8;
};

#include "player_record.h"

struct MonName
{
    u8 str[10];
#if GAME_LANGUAGE == LANGUAGE_JAPANESE
    u32 playerId:4;
#else
    u32 playerId:3;
    u32 isJapanese:1;
#endif
    u32 isNidoranF:1;
    u32 isNidoranM:1;
    u32 gender:2;
#if GAME_LANGUAGE == LANGUAGE_JAPANESE
    u32 level:8;
#endif
};

struct MonData
{
    u16 hp; // 0x24
    u32 status:12; // 0x26
    u32 giveMaxPokerusDays:1;
    u32 giveMaxPokerusStrain:1;
    u32 setMailTo1:1;
    u32 preserveHeldItem:1;
    u32 heldItem:16; // 0x28
    u16 unused;
    u16 moves[MAX_MON_MOVES]; // 0x2C , 0x2E , 0x30, 0x32
    u8 pps[MAX_MON_MOVES]; // 0x34-0x37
    u8 textBuffer[LINK_TEXT_BUFFER_SIZE];
};

struct MonLinkData
{
    u32 partyCountNibblefield:PARTY_SIZE * 4; // Like a bitfield, but every party member is represented by 4 bits.  For example, a party of 4 would be represented by 0x00FFFF
    u32 selectedMonIdPlusOne:7;
    u32 usePartyMenu:1;
    u32 monSwitchStatus:24; // a nibble per party slot: 0 can switch in, 1 can't switch with the foe, 2 already in battle, 3 already selected
    u32 monCount:8;
    u32 personality;
    u32 otId;
    u16 species;
    u16 castformForm;
    u16 moves[MAX_MON_MOVES];
    u8 pps[MAX_MON_MOVES];
    u16 moveRestrictions; // a nibble per move: bits 0-2 the restriction, bit 3 set when the move is locked in
#ifdef PROGRAM_COUPON
    u8 battleRestriction; // coupon.bin's older layout: one byte for both menu and switching
#else
    u8 battleMenuRestriction:4;
    u8 forcedMoveIndex:4;
#endif
    u8 switchOutRestriction:4; // 1 can't switch, 2 Shadow Tag, 3 Magnet Pull, 4 Arena Trap
    u8 trappingMonId:4;
    struct MonData monData[PARTY_SIZE];
    struct MonName monName[4];
    u8 partyMonIncoming:1; // set by LINK_CMD_RECV_PARTY_MON, cleared once the save is written
    u8 transferComplete:1;
    // LINK_CMD_0x44 sets both. FinalizeLinkSave waits for cmd44Received before
    // writing the second save slot, then for the command to finish (cmd44Active).
    u8 cmd44Received:1;
    u8 cmd44Active:1;
    u8 monId:4;
    u32 species2:16;
    u32 speciesLowerByte:8;
    u32 speciesUpperByte:8;
    u32 unk850_2:24;
    u32 *monPtr;
    u8 partyMenuLayout;
    u8 numMonsToSelect;
    u8 pokemonTransferPending;
    u8 species3; // Only 1 byte, so can not contain a Hoenn species
    u8 ALIGNED(4) giftRibbons[GIFT_RIBBONS_COUNT + 1]; // Aligned for u32 copy
    struct ExternalEventData2 externalEventData;
    u8 partySelectionReceived;
    u8 partyCount;
    u8 linkSessionComplete;
    u8 battleAction;
    u8 battleTarget;
    u8 filler_87D;
    u8 partyActionMonId;
    u8 responsePending;
    u32 response;
};

extern volatile struct MonLinkData gMonLinkData;

// colbtl's version (sample0519 has pokeemerald's u8 (u16, u8, u8) one).
s32 CalculatePPWithBonus(u32 move, s32 ppBonuses, u32 moveIndex);

#endif // GUARD_COLBTL_H
