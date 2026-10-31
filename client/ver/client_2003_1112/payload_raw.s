@ Generated; do not edit by hand.
@ Still-raw ranges of this version's payload, and the addresses of
@ symbols the linked objects use but nothing linked defines yet.

	.global PayloadStart
	.set PayloadStart, 0x02010000

	.global sTimerNum
	.set sTimerNum, 0x03000000
	.global sTimerCount
	.set sTimerCount, 0x03000002
	.global sTimerReg
	.set sTimerReg, 0x03000004
	.global sSavedIme
	.set sSavedIme, 0x03000008
	.global gIntrTable
	.set gIntrTable, 0x03000010
	.global gHeldKeys
	.set gHeldKeys, 0x03000048
	.global gNewKeys
	.set gNewKeys, 0x03000050
	.global IntrMain_Buffer
	.set IntrMain_Buffer, 0x030000B0
	.global gOamBuffer
	.set gOamBuffer, 0x030002C0
	.global gRngValue
	.set gRngValue, 0x030006C4
	.global gTilemap
	.set gTilemap, 0x03000930
	.global gLastWrittenSector
	.set gLastWrittenSector, 0x03000E30
	.global gTargetGameType
	.set gTargetGameType, 0x03000E34
	.global gPokedexOffset
	.set gPokedexOffset, 0x03000E38
	.global gExternalEventFlagsOffset
	.set gExternalEventFlagsOffset, 0x03000E3C
	.global gWarpFlagsOffset
	.set gWarpFlagsOffset, 0x03000E40
	.global gSaveIconAngle
	.set gSaveIconAngle, 0x03000E44
	.global gSaveSlotFirstSector
	.set gSaveSlotFirstSector, 0x03000E48
	.global gSlot2ValidSectors
	.set gSlot2ValidSectors, 0x03000E4C
	.global gDamagedSaveSectors
	.set gDamagedSaveSectors, 0x03000E50
	.global gSaveCounter
	.set gSaveCounter, 0x03000E54
	.global gSaveBlock1Buffer
	.set gSaveBlock1Buffer, 0x03000E60
	.global gDexSeen3Offset
	.set gDexSeen3Offset, 0x03004C60
	.global gPartyCountOffset
	.set gPartyCountOffset, 0x03004C64
	.global gSaveBlock1Size
	.set gSaveBlock1Size, 0x03004C68
	.global gGFRomHeaderFlags
	.set gGFRomHeaderFlags, 0x03004C6C
	.global gDexSeen2Offset
	.set gDexSeen2Offset, 0x03004C70
	.global gSaveBlock2Buffer
	.set gSaveBlock2Buffer, 0x03004C80
	.global gSaveBlock2Size
	.set gSaveBlock2Size, 0x03005C00
	.global gPartyOffset
	.set gPartyOffset, 0x03005C04
	.global gSaveBlockChunks
	.set gSaveBlockChunks, 0x03005C10
	.global gSlot1ValidSectors
	.set gSlot1ValidSectors, 0x03005C48
	.global gCurrentSaveSlot
	.set gCurrentSaveSlot, 0x03005C4C
	.global gFirstSectorChecksum
	.set gFirstSectorChecksum, 0x03005C50
	.global gSaveSectorBuffer
	.set gSaveSectorBuffer, 0x03005C60
	.global gMultiSioIntrFuncBuf
	.set gMultiSioIntrFuncBuf, 0x03006C60
	.global gMultiSioArea
	.set gMultiSioArea, 0x03006DA0
	.global gMultiSioRecvFuncBuf
	.set gMultiSioRecvFuncBuf, 0x03006F50
	.global gFlashTimeoutFlag
	.set gFlashTimeoutFlag, 0x03006F90
	.global PollFlashStatus
	.set PollFlashStatus, 0x03006F94
	.global WaitForFlashWrite
	.set WaitForFlashWrite, 0x03006F98
	.global ProgramFlashSector
	.set ProgramFlashSector, 0x03006F9C
	.global gFlash
	.set gFlash, 0x03006FA0
	.global gFlashNumRemainingBytes
	.set gFlashNumRemainingBytes, 0x03006FA4
	.global EraseFlashChip
	.set EraseFlashChip, 0x03006FA8
	.global EraseFlashSector
	.set EraseFlashSector, 0x03006FAC
	.global gFlashMaxTime
	.set gFlashMaxTime, 0x03006FB0
