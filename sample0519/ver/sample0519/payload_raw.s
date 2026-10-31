@ Generated; do not edit by hand.
@ Still-raw ranges of this version's payload, and the addresses of
@ symbols the linked objects use but nothing linked defines yet.

	.global PayloadStart
	.set PayloadStart, 0x02018000

	.global gRestartRequested
	.set gRestartRequested, 0x03000AA0
	.global gStartedOnce
	.set gStartedOnce, 0x03001008
	.global gCurrentTilemapFont
	.set gCurrentTilemapFont, 0x03000101
	.global gPrintfBuffer
	.set gPrintfBuffer, 0x03000000
	.global sErrorStatus
	.set sErrorStatus, 0x03000108
	.global sRtc
	.set sRtc, 0x03000110
	.global sProbeResult
	.set sProbeResult, 0x0300011C
	.global sRtcSavedIme
	.set sRtcSavedIme, 0x0300011E
	.global gDeliveryStateBuffer
	.set gDeliveryStateBuffer, 0x03000120
	.global gDeliveryState
	.set gDeliveryState, 0x030009A8
	.global gUnusedDeliveryByte
	.set gUnusedDeliveryByte, 0x030009AC
	.global sTimerNum
	.set sTimerNum, 0x030009B0
	.global sTimerCount
	.set sTimerCount, 0x030009B2
	.global sTimerReg
	.set sTimerReg, 0x030009B4
	.global sSavedIme
	.set sSavedIme, 0x030009B8
	.global sLocked
	.set sLocked, 0x030009BE
	.global gIntrTable
	.set gIntrTable, 0x03000AB0
	.global gHBlankIntrSlot
	.set gHBlankIntrSlot, 0x03000AEC
	.global gLagFrame
	.set gLagFrame, 0x03000AF4
	.global gNextOamIndex
	.set gNextOamIndex, 0x03000AF8
	.global gCopyRequestCount
	.set gCopyRequestCount, 0x03000AFC
	.global gKeys
	.set gKeys, 0x03000B10
	.global gIoRegs
	.set gIoRegs, 0x03000B70
	.global gVCountCallback
	.set gVCountCallback, 0x03000BC4
	.global gPaletteBuffer
	.set gPaletteBuffer, 0x03000BD0
	.global gHBlankCallback
	.set gHBlankCallback, 0x03000FD0
	.global gMainCallbacks
	.set gMainCallbacks, 0x03000FE0
	.global gNextAffineIndex
	.set gNextAffineIndex, 0x03000FFC
	.global gVBlankCallback
	.set gVBlankCallback, 0x03001000
	.global gPaletteBufferPtr
	.set gPaletteBufferPtr, 0x03001004
	.global gOamBuffer
	.set gOamBuffer, 0x03001010
	.global gVBlankWaiting
	.set gVBlankWaiting, 0x03001410
	.global gVBlankCounter
	.set gVBlankCounter, 0x03001414
	.global gSubCallbacks
	.set gSubCallbacks, 0x03001420
	.global gRng2High
	.set gRng2High, 0x0300143C
	.global gPendingVBlankCallback
	.set gPendingVBlankCallback, 0x03001540
	.global gRng2Low
	.set gRng2Low, 0x03001544
	.global gVCountIntrSlot
	.set gVCountIntrSlot, 0x03001548
	.global gCopyRequests
	.set gCopyRequests, 0x03001550
	.global gIntrMainBuffer
	.set gIntrMainBuffer, 0x030015B0
	.global gObjectEvents
	.set gObjectEvents, 0x03001B30
	.global gPlayerPartyCount
	.set gPlayerPartyCount, 0x03001D70
	.global gFlashMemoryPresent
	.set gFlashMemoryPresent, 0x03001D74
	.global gSaveBlock1
	.set gSaveBlock1, 0x03001D80
	.global gPlayerParty
	.set gPlayerParty, 0x03005840
	.global gSaveBlock2
	.set gSaveBlock2, 0x03005AA0
	.global gLocalTimeOffset
	.set gLocalTimeOffset, 0x03005B38
	.global gLevelUpHP
	.set gLevelUpHP, 0x03006330
	.global gSaveFileStatus
	.set gSaveFileStatus, 0x03006334
	.global gLastWrittenSector
	.set gLastWrittenSector, 0x0300633C
	.global gLastSaveCounter
	.set gLastSaveCounter, 0x03006340
	.global gLastKnownGoodSector
	.set gLastKnownGoodSector, 0x03006344
	.global gDamagedSaveSectors
	.set gDamagedSaveSectors, 0x03006348
	.global gSaveCounter
	.set gSaveCounter, 0x0300634C
	.global gReadWriteSector
	.set gReadWriteSector, 0x03006350
	.global gRngValue
	.set gRngValue, 0x03006358
	.global gLocalTime
	.set gLocalTime, 0x03006360
	.global gFlashTimeoutFlag
	.set gFlashTimeoutFlag, 0x03006368
	.global PollFlashStatus
	.set PollFlashStatus, 0x0300636C
	.global WaitForFlashWrite
	.set WaitForFlashWrite, 0x03006370
	.global ProgramFlashSector
	.set ProgramFlashSector, 0x03006374
	.global gFlash
	.set gFlash, 0x03006378
	.global gFlashNumRemainingBytes
	.set gFlashNumRemainingBytes, 0x0300637C
	.global EraseFlashChip
	.set EraseFlashChip, 0x03006380
	.global EraseFlashSector
	.set EraseFlashSector, 0x03006384
	.global gFlashMaxTime
	.set gFlashMaxTime, 0x03006388
	.global errno
	.set errno, 0x0300638C
	.global end
	.set end, 0x03006390
	.global GPIOPortDirection
	.set GPIOPortDirection, 0x080000C6
