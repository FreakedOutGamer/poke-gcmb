@ The SDK flash library's (FLASH1M_V102) read-only data, the same in both
@ revisions. Each of its objects' .rodata ends with an unreferenced copy of
@ that object's literal pool (the code uses the pools in .text), which C
@ cannot express, so the data is kept here rather than in the agb_flash
@ sources. This older library has no programFlashByte in FlashSetupInfo, and
@ drives the LE26FV10N1TS with the MX29L010's routines.

	.section .rodata
	.align 2, 0

@ agb_flash.c: pool copy.
	.4byte sTimerCount, gFlashTimeoutFlag, sTimerNum, sTimerReg
	.4byte FlashTimerIntr + 1, gFlashMaxTime, sSavedIme, sTimerReg
	.4byte sTimerNum, gFlashTimeoutFlag, sTimerCount, sTimerReg
	.4byte sTimerNum, sSavedIme, PollFlashStatus, ReadFlash1 + 1
	.4byte SetReadFlash1 + 1, gFlash, ReadFlash_Core + 1, ReadFlash + 1
	.4byte gFlash, VerifyFlashSector_Core + 1, VerifyFlashSector + 1, gFlash
	.4byte VerifyFlashSector_Core + 1, VerifyFlashSector + 1, ProgramFlashSector, ProgramFlashSector

@ agb_flash_1m.c
AgbLibFlashVersion:
	.asciz "FLASH1M_V102"
	.align 2, 0

	.global sSetupInfos
sSetupInfos:
	.4byte MX29L010, LE26FV10N1TS, DefaultFlash

@ agb_flash_1m.c: pool copy.
	.4byte sSetupInfos, ProgramFlashSector, EraseFlashChip, EraseFlashSector
	.4byte WaitForFlashWrite, gFlashMaxTime, gFlash, PollFlashStatus
	.4byte gFlashTimeoutFlag

@ agb_flash_mx.c
	.global mxMaxTime
mxMaxTime:
	.2byte 10, 65469, 194
	.2byte 10, 65469, 194
	.2byte 2000, 65469, 194
	.2byte 2000, 65469, 194

@ struct FlashSetupInfo (V102): programFlashSector, eraseFlashChip,
@ eraseFlashSector, WaitForFlashWrite, maxTime, then struct FlashType:
@ romSize, sector {size, shift, count, top}, wait[2], makerId, deviceId.
	.global MX29L010
MX29L010:
	.4byte ProgramFlashSector_MX + 1, EraseFlashChip_MX + 1, EraseFlashSector_MX + 1, WaitForFlashWrite_Common + 1
	.4byte mxMaxTime
	.4byte 0x20000 @ 1 Mbit
	.4byte 0x1000
	.byte 12, 0
	.2byte 32, 0
	.2byte 0
	.2byte 3, 1
	.byte 0xC2, 0x09 @ Macronix MX29L010
	.2byte 0

	.global DefaultFlash
DefaultFlash:
	.4byte ProgramFlashSector_MX + 1, EraseFlashChip_MX + 1, EraseFlashSector_MX + 1, WaitForFlashWrite_Common + 1
	.4byte mxMaxTime
	.4byte 0x20000
	.4byte 0x1000
	.byte 12, 0
	.2byte 32, 0
	.2byte 0
	.2byte 3, 1
	.byte 0x00, 0x00 @ any other chip
	.2byte 0

@ agb_flash_mx.c: pool copy.
	.4byte gFlash, WaitForFlashWrite, gFlash, WaitForFlashWrite
	.4byte WaitForFlashWrite, gFlash, gFlashNumRemainingBytes

@ agb_flash_le.c
	.global leMaxTime
leMaxTime:
	.2byte 10, 65469, 194
	.2byte 10, 65469, 194
	.2byte 2000, 65469, 194
	.2byte 2000, 65469, 194

	.global LE26FV10N1TS
LE26FV10N1TS:
	.4byte ProgramFlashSector_MX + 1, EraseFlashChip_MX + 1, EraseFlashSector_MX + 1, WaitForFlashWrite_Common + 1
	.4byte leMaxTime
	.4byte 0x20000
	.4byte 0x1000
	.byte 12, 0
	.2byte 32, 0
	.2byte 0
	.2byte 3, 1
	.byte 0x62, 0x13 @ Sanyo LE26FV10N1TS
	.2byte 0
