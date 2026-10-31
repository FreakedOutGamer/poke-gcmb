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
	.set SoundMainBTM, 0x02006925
	.global RealClearChain
	.set RealClearChain, 0x0200693D
	.global ply_fine
	.set ply_fine, 0x0200695D
	.global MPlayJumpTableCopy
	.set MPlayJumpTableCopy, 0x0200698D
	.global ply_goto
	.set ply_goto, 0x020069CD
	.global ply_patt
	.set ply_patt, 0x020069ED
	.global ply_pend
	.set ply_pend, 0x02006A09
	.global ply_rept
	.set ply_rept, 0x02006A1D
	.global ply_prio
	.set ply_prio, 0x02006A4D
	.global ply_tempo
	.set ply_tempo, 0x02006A59
	.global ply_keysh
	.set ply_keysh, 0x02006A6D
	.global ply_voice
	.set ply_voice, 0x02006A81
	.global ply_vol
	.set ply_vol, 0x02006AB1
	.global ply_pan
	.set ply_pan, 0x02006AC5
	.global ply_bend
	.set ply_bend, 0x02006AD9
	.global ply_bendr
	.set ply_bendr, 0x02006AED
	.global ply_lfodl
	.set ply_lfodl, 0x02006B01
	.global ply_modt
	.set ply_modt, 0x02006B0D
	.global ply_tune
	.set ply_tune, 0x02006B25
	.global ply_port
	.set ply_port, 0x02006B39
	.global m4aSoundVSync
	.set m4aSoundVSync, 0x02006B51
	.global MPlayMain
	.set MPlayMain, 0x02006B89
	.global TrackStop
	.set TrackStop, 0x02006DF5
	.global ply_note
	.set ply_note, 0x02006E39
	.global ply_endtie
	.set ply_endtie, 0x02007051
	.global ply_lfos
	.set ply_lfos, 0x020070B9
	.global ply_mod
	.set ply_mod, 0x020070CD
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
	.set gMPlayTable, 0x0201DECC
	.global bss_start
	.set bss_start, 0x02020A30
	.global gScreenIsFadedOut
	.set gScreenIsFadedOut, 0x02020A30
	.global gNextWindowBaseBlock
	.set gNextWindowBaseBlock, 0x02020A34
	.global gBattleMenuMonId
	.set gBattleMenuMonId, 0x02020A38
	.global gMPlayTrack_BGM
	.set gMPlayTrack_BGM, 0x02020A40
	.global gSoundMainRAMBufferAddress
	.set gSoundMainRAMBufferAddress, 0x020213A4
	.global gSaveLoadResult
	.set gSaveLoadResult, 0x020217A4
	.global gBerryFixResult
	.set gBerryFixResult, 0x020217A8
	.global gStringBuffers
	.set gStringBuffers, 0x020217B0
	.global gPartyMenuState
	.set gPartyMenuState, 0x02021850
	.global gMonSummaryScreen
	.set gMonSummaryScreen, 0x02021980
	.global gMessageWindowPtr
	.set gMessageWindowPtr, 0x020219D4
	.global gBattleMenuInputState
	.set gBattleMenuInputState, 0x020219E0
	.global gSoundInfo
	.set gSoundInfo, 0x02021A30
	.global gMPlayJumpTable
	.set gMPlayJumpTable, 0x020229E0
	.global gCgbChans
	.set gCgbChans, 0x02022A70
	.global gMPlayInfo_BGM
	.set gMPlayInfo_BGM, 0x02022B70
	.global gMPlayMemAccArea
	.set gMPlayMemAccArea, 0x02022BB0
	.global gIntrTable
	.set gIntrTable, 0x02022BC0
	.global gVBlankCounter
	.set gVBlankCounter, 0x02022BF8
	.global gVBlankCallback
	.set gVBlankCallback, 0x02022BFC
	.global gBgHofsBuffer
	.set gBgHofsBuffer, 0x02022C00
	.global gBgVofsBuffer
	.set gBgVofsBuffer, 0x02022C08
	.global gWindows
	.set gWindows, 0x02022C20
	.global gNewKeys
	.set gNewKeys, 0x02022EA0
	.global gNewAndRepeatedKeys
	.set gNewAndRepeatedKeys, 0x02022EA4
	.global gHeldKeys
	.set gHeldKeys, 0x02022EA8
	.global gPcItemsPtr
	.set gPcItemsPtr, 0x02022EAC
	.global gPokemonStoragePtr
	.set gPokemonStoragePtr, 0x02022EB0
	.global gSaveBlock1BackupPtr
	.set gSaveBlock1BackupPtr, 0x02022EB4
	.global gPokedexPtr
	.set gPokedexPtr, 0x02022EB8
	.global gSaveBlock1Ptr
	.set gSaveBlock1Ptr, 0x02022EBC
	.global gSaveRngValue
	.set gSaveRngValue, 0x02022EC0
	.global gFlagsPtr
	.set gFlagsPtr, 0x02022EC4
	.global gSaveBlock2Ptr
	.set gSaveBlock2Ptr, 0x02022EC8
	.global gGiftRibbonsPtr
	.set gGiftRibbonsPtr, 0x02022ECC
	.global gDexSeen3Ptr
	.set gDexSeen3Ptr, 0x02022ED0
	.global gVarsPtr
	.set gVarsPtr, 0x02022ED4
	.global gPlayerPartyBackupPtr
	.set gPlayerPartyBackupPtr, 0x02022EDC
	.global gPlayerPartyCountPtr
	.set gPlayerPartyCountPtr, 0x02022EE0
	.global gDexSeen2Ptr
	.set gDexSeen2Ptr, 0x02022EE8
	.global gLastSaveBlockPtr
	.set gLastSaveBlockPtr, 0x02022EEC
	.global gLastWrittenSector
	.set gLastWrittenSector, 0x02022EF0
	.global gLastSaveCounter
	.set gLastSaveCounter, 0x02022EF4
	.global gSaveStatusFlags
	.set gSaveStatusFlags, 0x02022F00
	.global gLastKnownGoodSector
	.set gLastKnownGoodSector, 0x02022F14
	.global gDamagedSaveSectors
	.set gDamagedSaveSectors, 0x02022F18
	.global gSaveCounter
	.set gSaveCounter, 0x02022F1C
	.global gReadWriteSector
	.set gReadWriteSector, 0x02022F20
	.global gSaveReadBuffer
	.set gSaveReadBuffer, 0x02022F30
	.global gIncrementalSectorId
	.set gIncrementalSectorId, 0x02023F30
	.global gPlayerLinkInfo
	.set gPlayerLinkInfo, 0x02023F40
	.global gTextBuffer
	.set gTextBuffer, 0x020241C0
	.global gJoyBusState
	.set gJoyBusState, 0x020251E0
	.global gFlashTimeoutFlag
	.set gFlashTimeoutFlag, 0x02025214
	.global PollFlashStatus
	.set PollFlashStatus, 0x02025218
	.global WaitForFlashWrite
	.set WaitForFlashWrite, 0x0202521C
	.global ProgramFlashSector
	.set ProgramFlashSector, 0x02025220
	.global gFlash
	.set gFlash, 0x02025224
	.global ProgramFlashByte
	.set ProgramFlashByte, 0x02025228
	.global gFlashNumRemainingBytes
	.set gFlashNumRemainingBytes, 0x0202522C
	.global EraseFlashChip
	.set EraseFlashChip, 0x02025230
	.global EraseFlashSector
	.set EraseFlashSector, 0x02025234
	.global gFlashMaxTime
	.set gFlashMaxTime, 0x02025238
	.global gUpdateSuccessful
	.set gUpdateSuccessful, 0x0202523C
	.global gGameVersion
	.set gGameVersion, 0x02025240
	.global gTimeSinceBerryUpdate
	.set gTimeSinceBerryUpdate, 0x02025248
	.global gRtcUTCTime
	.set gRtcUTCTime, 0x02025250
	.global bss_end
	.set bss_end, 0x0202525C
	.global gSaveBlockLoadRegions
	.set gSaveBlockLoadRegions, 0x0202525C
	.global GPIOPortDirection
	.set GPIOPortDirection, 0x080000C6




	.global gLinkMonNameBuffer
	.set gLinkMonNameBuffer, 0x020219F0

	.global gMonLinkData
	.set gMonLinkData, 0x02024950


	.global CpuSet
	.thumb_set CpuSet, 0x0200FCF1

	.global gBgTilemapBufferTransferScheduled
	.set gBgTilemapBufferTransferScheduled, 0x02022C10

	.global gBattleMenuState
	.set gBattleMenuState, 0x02021A10

	.global gEnigmaBerryPtr
	.set gEnigmaBerryPtr, 0x02022ED8


	.global gPlayerPartyPtr
	.set gPlayerPartyPtr, 0x02022EE4

	.global gLinkCommandReady
	.set gLinkCommandReady, 0x02024940

	.global gTargetIsRubySapphire
	.set gTargetIsRubySapphire, 0x020251DC

	.global gRomDetection_IsEnglishROM
	.set gRomDetection_IsEnglishROM, 0x020251D4

	.global gAgbPmRomParams
	.set gAgbPmRomParams, 0x020251D8

	.global gRomHeader
	.set gRomHeader, 0x02000004
