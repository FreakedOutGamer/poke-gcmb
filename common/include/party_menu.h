#ifndef GUARD_PARTY_MENU_H
#define GUARD_PARTY_MENU_H

// mode 0 picks one Pokémon, mode 1 several (gMonLinkData.numMonsToSelect).
void ShowPartyMenu(s32 mode, u32 opponentMonId, u32 cancelDisabled);
void DrawPartyMonHealthBar(int bgNum, int x, int y, u32 monId);
void ResetPartyMenuState(void);
void ResetPartyMenuCursor(void);

#endif // GUARD_PARTY_MENU_H
