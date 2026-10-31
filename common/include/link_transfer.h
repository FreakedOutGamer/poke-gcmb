#ifndef GUARD_LINK_TRANSFER_H
#define GUARD_LINK_TRANSFER_H

// Moving Pokémon and battle data between the cartridge and the GameCube.

void ApplyLinkBattlePartyData(void);
bool32 FinalizeLinkSave(void);
s32 FinalizeCouponTransfer(void);
s32 FinalizePokemonTransfer(void);
void BufferFirstPartyMon(void);
// The restriction code (1 can't switch, 2 Shadow Tag, 3 Magnet Pull,
// 4 Arena Trap) in the low 16 bits, the trapping mon in the high 16.
u32 GetSwitchOutRestriction(void);
u32 GetLinkSelectedMonId(void);
u32 ShouldUseLinkPartyMenu(void);
u32 GetNibbleByIndex(u32 value, s32 index);

#endif // GUARD_LINK_TRANSFER_H
