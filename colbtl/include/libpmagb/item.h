#ifndef PMAGB_ITEM_H
#define PMAGB_ITEM_H

u32 UNUSED GetEnigmaBerryChecksum(struct EnigmaBerry * enigmaBerry);
bool32 UNUSED IsEnigmaBerryValid(void);
const struct Berry * UNUSED GetBerryInfo(u8 berry);
const u8 * UNUSED GetItemName(u16 itemId);
void CopyItemName(u32 itemId, u8 *string, const u8 * berryString);

#endif //PMAGB_ITEM_H
