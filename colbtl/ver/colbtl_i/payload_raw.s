@ Generated; do not edit by hand.
@ Still-raw ranges of this version's payload, and the addresses of
@ symbols the linked objects use but nothing linked defines yet.

	.global PayloadStart
	.set PayloadStart, 0x02000000

	.global gMaxLines
	.set gMaxLines, 0x00000000
	.global gNumMusicPlayers
	.set gNumMusicPlayers, 0x00000001
	.global SoundMainBTM
	.set SoundMainBTM, 0x02006921
	.global RealClearChain
	.set RealClearChain, 0x02006939
	.global ply_fine
	.set ply_fine, 0x02006959
	.global MPlayJumpTableCopy
	.set MPlayJumpTableCopy, 0x02006989
	.global ply_goto
	.set ply_goto, 0x020069C9
	.global ply_patt
	.set ply_patt, 0x020069E9
	.global ply_pend
	.set ply_pend, 0x02006A05
	.global ply_rept
	.set ply_rept, 0x02006A19
	.global ply_prio
	.set ply_prio, 0x02006A49
	.global ply_tempo
	.set ply_tempo, 0x02006A55
	.global ply_keysh
	.set ply_keysh, 0x02006A69
	.global ply_voice
	.set ply_voice, 0x02006A7D
	.global ply_vol
	.set ply_vol, 0x02006AAD
	.global ply_pan
	.set ply_pan, 0x02006AC1
	.global ply_bend
	.set ply_bend, 0x02006AD5
	.global ply_bendr
	.set ply_bendr, 0x02006AE9
	.global ply_lfodl
	.set ply_lfodl, 0x02006AFD
	.global ply_modt
	.set ply_modt, 0x02006B09
	.global ply_tune
	.set ply_tune, 0x02006B21
	.global ply_port
	.set ply_port, 0x02006B35
	.global m4aSoundVSync
	.set m4aSoundVSync, 0x02006B4D
	.global MPlayMain
	.set MPlayMain, 0x02006B85
	.global TrackStop
	.set TrackStop, 0x02006DF1
	.global ply_note
	.set ply_note, 0x02006E35
	.global ply_endtie
	.set ply_endtie, 0x0200704D
	.global ply_lfos
	.set ply_lfos, 0x020070B5
	.global ply_mod
	.set ply_mod, 0x020070C9
	.global Div
	.set Div, 0x0200FCF5
	.global Mod
	.set Mod, 0x0200FCF9
	.global HuffUnComp
	.set HuffUnComp, 0x0200FD01
	.global LZ77UnCompVram
	.set LZ77UnCompVram, 0x0200FD05
	.global LZ77UnCompWram
	.set LZ77UnCompWram, 0x0200FD09
	.global RLUnCompVram
	.set RLUnCompVram, 0x0200FD0D
	.global RegisterRamReset
	.set RegisterRamReset, 0x0200FD11
	.global SoftReset
	.set SoftReset, 0x0200FD15
	.global VBlankIntrWait
	.set VBlankIntrWait, 0x0200FD2D
	.global SoftResetRom
	.set SoftResetRom, 0x0200FD35
	.global gMPlayTable
	.set gMPlayTable, 0x0201DE9C
	.global bss_start
	.set bss_start, 0x02020A3C
	.global gScreenIsFadedOut
	.set gScreenIsFadedOut, 0x02020A40
	.global gNextWindowBaseBlock
	.set gNextWindowBaseBlock, 0x02020A44
	.global gBattleMenuMonId
	.set gBattleMenuMonId, 0x02020A48
	.global gMPlayTrack_BGM
	.set gMPlayTrack_BGM, 0x02020A50
	.global gSoundMainRAMBufferAddress
	.set gSoundMainRAMBufferAddress, 0x020213B4
	.global gSaveLoadResult
	.set gSaveLoadResult, 0x020217B4
	.global gBerryFixResult
	.set gBerryFixResult, 0x020217B8
	.global gStringBuffers
	.set gStringBuffers, 0x020217C0
	.global gPartyMenuState
	.set gPartyMenuState, 0x02021860
	.global gMonSummaryScreen
	.set gMonSummaryScreen, 0x02021990
	.global gMessageWindowPtr
	.set gMessageWindowPtr, 0x020219E4
	.global gBattleMenuInputState
	.set gBattleMenuInputState, 0x020219F0
	.global gSoundInfo
	.set gSoundInfo, 0x02021A40
	.global gMPlayJumpTable
	.set gMPlayJumpTable, 0x020229F0
	.global gCgbChans
	.set gCgbChans, 0x02022A80
	.global gMPlayInfo_BGM
	.set gMPlayInfo_BGM, 0x02022B80
	.global gMPlayMemAccArea
	.set gMPlayMemAccArea, 0x02022BC0
	.global gIntrTable
	.set gIntrTable, 0x02022BD0
	.global gVBlankCounter
	.set gVBlankCounter, 0x02022C08
	.global gVBlankCallback
	.set gVBlankCallback, 0x02022C0C
	.global gBgHofsBuffer
	.set gBgHofsBuffer, 0x02022C10
	.global gBgVofsBuffer
	.set gBgVofsBuffer, 0x02022C18
	.global gWindows
	.set gWindows, 0x02022C30
	.global gNewKeys
	.set gNewKeys, 0x02022EB0
	.global gNewAndRepeatedKeys
	.set gNewAndRepeatedKeys, 0x02022EB4
	.global gHeldKeys
	.set gHeldKeys, 0x02022EB8
	.global gPcItemsPtr
	.set gPcItemsPtr, 0x02022EBC
	.global gPokemonStoragePtr
	.set gPokemonStoragePtr, 0x02022EC0
	.global gSaveBlock1BackupPtr
	.set gSaveBlock1BackupPtr, 0x02022EC4
	.global gPokedexPtr
	.set gPokedexPtr, 0x02022EC8
	.global gSaveBlock1Ptr
	.set gSaveBlock1Ptr, 0x02022ECC
	.global gSaveRngValue
	.set gSaveRngValue, 0x02022ED0
	.global gFlagsPtr
	.set gFlagsPtr, 0x02022ED4
	.global gSaveBlock2Ptr
	.set gSaveBlock2Ptr, 0x02022ED8
	.global gGiftRibbonsPtr
	.set gGiftRibbonsPtr, 0x02022EDC
	.global gDexSeen3Ptr
	.set gDexSeen3Ptr, 0x02022EE0
	.global gVarsPtr
	.set gVarsPtr, 0x02022EE4
	.global gPlayerPartyBackupPtr
	.set gPlayerPartyBackupPtr, 0x02022EEC
	.global gPlayerPartyCountPtr
	.set gPlayerPartyCountPtr, 0x02022EF0
	.global gPlayerPartyPtr
	.set gPlayerPartyPtr, 0x02022EF4
	.global gDexSeen2Ptr
	.set gDexSeen2Ptr, 0x02022EF8
	.global gLastSaveBlockPtr
	.set gLastSaveBlockPtr, 0x02022EFC
	.global gLastWrittenSector
	.set gLastWrittenSector, 0x02022F00
	.global gLastSaveCounter
	.set gLastSaveCounter, 0x02022F04
	.global gSaveStatusFlags
	.set gSaveStatusFlags, 0x02022F10
	.global gLastKnownGoodSector
	.set gLastKnownGoodSector, 0x02022F24
	.global gDamagedSaveSectors
	.set gDamagedSaveSectors, 0x02022F28
	.global gSaveCounter
	.set gSaveCounter, 0x02022F2C
	.global gReadWriteSector
	.set gReadWriteSector, 0x02022F30
	.global gSaveReadBuffer
	.set gSaveReadBuffer, 0x02022F40
	.global gIncrementalSectorId
	.set gIncrementalSectorId, 0x02023F40
	.global gPlayerLinkInfo
	.set gPlayerLinkInfo, 0x02023F50
	.global gTextBuffer
	.set gTextBuffer, 0x020241D0
	.global gJoyBusState
	.set gJoyBusState, 0x020251F0
	.global gFlashTimeoutFlag
	.set gFlashTimeoutFlag, 0x02025224
	.global PollFlashStatus
	.set PollFlashStatus, 0x02025228
	.global WaitForFlashWrite
	.set WaitForFlashWrite, 0x0202522C
	.global ProgramFlashSector
	.set ProgramFlashSector, 0x02025230
	.global gFlash
	.set gFlash, 0x02025234
	.global ProgramFlashByte
	.set ProgramFlashByte, 0x02025238
	.global gFlashNumRemainingBytes
	.set gFlashNumRemainingBytes, 0x0202523C
	.global EraseFlashChip
	.set EraseFlashChip, 0x02025240
	.global EraseFlashSector
	.set EraseFlashSector, 0x02025244
	.global gFlashMaxTime
	.set gFlashMaxTime, 0x02025248
	.global gUpdateSuccessful
	.set gUpdateSuccessful, 0x0202524C
	.global gGameVersion
	.set gGameVersion, 0x02025250
	.global gTimeSinceBerryUpdate
	.set gTimeSinceBerryUpdate, 0x02025258
	.global gRtcUTCTime
	.set gRtcUTCTime, 0x02025260
	.global bss_end
	.set bss_end, 0x0202526C
	.global gSaveBlockLoadRegions
	.set gSaveBlockLoadRegions, 0x0202526C
	.global GPIOPortDirection
	.set GPIOPortDirection, 0x080000C6




	.global gLinkMonNameBuffer
	.set gLinkMonNameBuffer, 0x02021A00

	.global gMonLinkData
	.set gMonLinkData, 0x02024960


	.global CpuSet
	.thumb_set CpuSet, 0x0200FCF1

	.global gBgTilemapBufferTransferScheduled
	.set gBgTilemapBufferTransferScheduled, 0x02022C20

	.global gBattleMenuState
	.set gBattleMenuState, 0x02021A20

	.global gEnigmaBerryPtr
	.set gEnigmaBerryPtr, 0x02022EE8


	.global gLinkCommandReady
	.set gLinkCommandReady, 0x02024950

	.global gTargetIsRubySapphire
	.set gTargetIsRubySapphire, 0x020251EC

	.global gRomDetection_IsEnglishROM
	.set gRomDetection_IsEnglishROM, 0x020251E4

	.global gAgbPmRomParams
	.set gAgbPmRomParams, 0x020251E8

	.global gRomHeader
	.set gRomHeader, 0x02000004
