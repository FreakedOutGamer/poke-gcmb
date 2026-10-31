#include "global.h"

// The program's messages, drawn by PrintString. They are plain ASCII, with
// "\033" (0x1B) for the font's é.

#if REVISION == 0
const u8 gText_ConnectCable[] = "Connect GBA Cable.\n Don't turn off the Power";
const u8 gText_ReceivedJirachi[] = "You receive jirachi\n Please love him.\n Turn off the power\nand remove the cable.";
const u8 gText_NotPokemonCartridge[] = "Not Pok\033mon Cardiage."; // sic
const u8 gText_ErrorNoSaveData[] = "Error! No savedata.";
const u8 gText_ErrorProceedGame[] = "Error! Proceed game.";
const u8 gText_Error6Pokemon[] = "Error! 6 Pok\033mons.";
const u8 gText_ErrorDestroyedFlashData[] = "Error! Destroyed flashdata.";
// Unused; see PrintErrorMessage.
const u8 gText_ErrorNoFlashRam[] = "Error! No Flash RAM.";
const u8 gText_FailedToSave[] = "Failed to save. Retry Again.";
const u8 gText_JirachiAlreadyDelivered[] = "Jirachi has already\nbeen delivered before.";
const u8 gText_UnsupportedVersion[] = "This system doesn't\n support this version.";
#else
const u8 gText_ConnectCable[] = "The Game Boy Advance system\nis communicating with the\nNintendo GameCube.\nPlease do not turn off the\npower or unplug the\nNintendo GameCube Game Boy\nAdvance cable.";
const u8 gText_ReceivedJirachi[] = "You got JIRACHI!\nPlease take good care of it.\nPlease turn off the power\nof your Game Boy Advance\nsystem and unplug the\nNintendo GameCube Game Boy\nAdvance cable.";
const u8 gText_NotPokemonCartridge[] = "Error: Unable to transfer\nJIRACHI to a Pok\033mon Ruby\nor Sapphire Game Pak that\nis not a North American\nversion.";
const u8 gText_ErrorNoSaveData[] = "Error:\n  There is no save file.";
const u8 gText_ErrorProceedGame[] = "Error:\n You don't have a POK\033DEX.\n You cannot receive JIRACHI\n as a gift right now.\n";
const u8 gText_Error6Pokemon[] = "Error:\n Your Pok\033mon party is full.";
const u8 gText_ErrorDestroyedFlashData[] = "Error:\n The save file is corrupted.";
// Unused; in place of the earlier revision's "No Flash RAM" message.
const u8 gText_ErrorSaveCorrupted_Unused[] = "Error:\n The save file is corrupted.";
const u8 gText_FailedToSave[] = "Error: Save failure.";
const u8 gText_JirachiAlreadyDelivered[] = "Error:\nYou already have been given\nJIRACHI as a gift. You can\nonly receive one JIRACHI\nper Game Pak.";
const u8 gText_UnsupportedVersion[] = "Error:\n This version is not\n    compatible.";
// Unused.
const u8 gText_LookAtTheTvScreen[] = "Look at the TV screen.";
const u8 gText_BerryProgramUpdated_Unused[] = "Bery Problem is updated"; // sic
const u8 gText_BerryProgramUpdated[] = "Your Berry Program\n       was updated.";
const u8 gText_BerryProgramUpToDate[] = "You don't need to update\n      your Berry Program.";
const u8 gText_BerryProgramUpdateFailed[] = "Unable to update\n          Berry Program.";
#endif
