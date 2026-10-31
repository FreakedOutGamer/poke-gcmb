#ifndef GUARD_LINK_MESSAGE_H
#define GUARD_LINK_MESSAGE_H

// The message screen shown while waiting on the GameCube.

void InitLinkMessageScreen(void);
void PrintSaveStatusMessage(void);
u32 RunLinkStandbyLoop(void);
void WarningPrint(u32 msgId);
void ErrorPrint(u32 msgId);

#endif // GUARD_LINK_MESSAGE_H
