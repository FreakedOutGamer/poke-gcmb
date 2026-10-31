@ Generated; do not edit by hand.
@ Still-raw ranges of this version's payload, and the addresses of
@ symbols the linked objects use but nothing linked defines yet.

	.global PayloadStart
	.set PayloadStart, 0x02010000

	.global sRtcInfoWork
	.set sRtcInfoWork, 0x03000000
	.global sErrorStatus
	.set sErrorStatus, 0x03000010
	.global sRtc
	.set sRtc, 0x03000018
	.global sProbeResult
	.set sProbeResult, 0x03000024
	.global sRtcSavedIme
	.set sRtcSavedIme, 0x03000026
	.global sTimerNum
	.set sTimerNum, 0x03000028
	.global sTimerCount
	.set sTimerCount, 0x0300002A
	.global sTimerReg
	.set sTimerReg, 0x0300002C
	.global sSavedIme
	.set sSavedIme, 0x03000030
	.global sLocked
	.set sLocked, 0x03000036
	.global gIntrTable
	.set gIntrTable, 0x03000040
	.global gHeldKeys
	.set gHeldKeys, 0x03000078
	.global gNewKeys
	.set gNewKeys, 0x03000080
	.global IntrMain_Buffer
	.set IntrMain_Buffer, 0x030000E0
	.global gOamBuffer
	.set gOamBuffer, 0x030002F0
	.global gRngValue
	.set gRngValue, 0x030006F4
	.global gTilemap
	.set gTilemap, 0x03000960
	.global gLastWrittenSector
	.set gLastWrittenSector, 0x03000E60
	.global gTargetGameType
	.set gTargetGameType, 0x03000E64
	.global gPokedexOffset
	.set gPokedexOffset, 0x03000E68
	.global gExternalEventFlagsOffset
	.set gExternalEventFlagsOffset, 0x03000E6C
	.global gWarpFlagsOffset
	.set gWarpFlagsOffset, 0x03000E70
	.global gSaveIconAngle
	.set gSaveIconAngle, 0x03000E74
	.global gSaveSlotFirstSector
	.set gSaveSlotFirstSector, 0x03000E78
	.global gSlot2ValidSectors
	.set gSlot2ValidSectors, 0x03000E7C
	.global gDamagedSaveSectors
	.set gDamagedSaveSectors, 0x03000E80
	.global gSaveCounter
	.set gSaveCounter, 0x03000E84
	.global gSaveBlock1Buffer
	.set gSaveBlock1Buffer, 0x03000E90
	.global gDexSeen3Offset
	.set gDexSeen3Offset, 0x03004C90
	.global gPartyCountOffset
	.set gPartyCountOffset, 0x03004C94
	.global gSaveBlock1Size
	.set gSaveBlock1Size, 0x03004C98
	.global gGFRomHeaderFlags
	.set gGFRomHeaderFlags, 0x03004C9C
	.global gDexSeen2Offset
	.set gDexSeen2Offset, 0x03004CA0
	.global gSaveBlock2Buffer
	.set gSaveBlock2Buffer, 0x03004CB0
	.global gSaveBlock2Size
	.set gSaveBlock2Size, 0x03005C30
	.global gPartyOffset
	.set gPartyOffset, 0x03005C34
	.global gSaveBlockChunks
	.set gSaveBlockChunks, 0x03005C40
	.global gBerryFixStatus
	.set gBerryFixStatus, 0x03005C78
	.global gSlot1ValidSectors
	.set gSlot1ValidSectors, 0x03005C7C
	.global gCurrentSaveSlot
	.set gCurrentSaveSlot, 0x03005C80
	.global gFirstSectorChecksum
	.set gFirstSectorChecksum, 0x03005C84
	.global gSaveSectorBuffer
	.set gSaveSectorBuffer, 0x03005C90
	.global gMultiSioIntrFuncBuf
	.set gMultiSioIntrFuncBuf, 0x03006C90
	.global gMultiSioArea
	.set gMultiSioArea, 0x03006DD0
	.global gMultiSioRecvFuncBuf
	.set gMultiSioRecvFuncBuf, 0x03006F80
	.global gSaveBlock2Ptr
	.set gSaveBlock2Ptr, 0x03006FC0
	.global gSaveBlock1Ptr
	.set gSaveBlock1Ptr, 0x03006FC4
	.global gTimeSinceBerryUpdate
	.set gTimeSinceBerryUpdate, 0x03006FC8
	.global gRtcUTCTime
	.set gRtcUTCTime, 0x03006FD0
	.global gFlashTimeoutFlag
	.set gFlashTimeoutFlag, 0x03006FD8
	.global PollFlashStatus
	.set PollFlashStatus, 0x03006FDC
	.global WaitForFlashWrite
	.set WaitForFlashWrite, 0x03006FE0
	.global ProgramFlashSector
	.set ProgramFlashSector, 0x03006FE4
	.global gFlash
	.set gFlash, 0x03006FE8
	.global gFlashNumRemainingBytes
	.set gFlashNumRemainingBytes, 0x03006FEC
	.global EraseFlashChip
	.set EraseFlashChip, 0x03006FF0
	.global EraseFlashSector
	.set EraseFlashSector, 0x03006FF4
	.global gFlashMaxTime
	.set gFlashMaxTime, 0x03006FF8
	.global GPIOPortDirection
	.set GPIOPortDirection, 0x080000C6
