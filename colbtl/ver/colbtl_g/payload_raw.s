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
	.set SoundMainBTM, 0x0200692D
	.global RealClearChain
	.set RealClearChain, 0x02006945
	.global ply_fine
	.set ply_fine, 0x02006965
	.global MPlayJumpTableCopy
	.set MPlayJumpTableCopy, 0x02006995
	.global ply_goto
	.set ply_goto, 0x020069D5
	.global ply_patt
	.set ply_patt, 0x020069F5
	.global ply_pend
	.set ply_pend, 0x02006A11
	.global ply_rept
	.set ply_rept, 0x02006A25
	.global ply_prio
	.set ply_prio, 0x02006A55
	.global ply_tempo
	.set ply_tempo, 0x02006A61
	.global ply_keysh
	.set ply_keysh, 0x02006A75
	.global ply_voice
	.set ply_voice, 0x02006A89
	.global ply_vol
	.set ply_vol, 0x02006AB9
	.global ply_pan
	.set ply_pan, 0x02006ACD
	.global ply_bend
	.set ply_bend, 0x02006AE1
	.global ply_bendr
	.set ply_bendr, 0x02006AF5
	.global ply_lfodl
	.set ply_lfodl, 0x02006B09
	.global ply_modt
	.set ply_modt, 0x02006B15
	.global ply_tune
	.set ply_tune, 0x02006B2D
	.global ply_port
	.set ply_port, 0x02006B41
	.global m4aSoundVSync
	.set m4aSoundVSync, 0x02006B59
	.global MPlayMain
	.set MPlayMain, 0x02006B91
	.global TrackStop
	.set TrackStop, 0x02006DFD
	.global ply_note
	.set ply_note, 0x02006E41
	.global ply_endtie
	.set ply_endtie, 0x02007059
	.global ply_lfos
	.set ply_lfos, 0x020070C1
	.global ply_mod
	.set ply_mod, 0x020070D5
	.global Div
	.set Div, 0x0200FCFD
	.global Mod
	.set Mod, 0x0200FD01
	.global HuffUnComp
	.set HuffUnComp, 0x0200FD09
	.global LZ77UnCompVram
	.set LZ77UnCompVram, 0x0200FD0D
	.global LZ77UnCompWram
	.set LZ77UnCompWram, 0x0200FD11
	.global RLUnCompVram
	.set RLUnCompVram, 0x0200FD15
	.global RegisterRamReset
	.set RegisterRamReset, 0x0200FD19
	.global SoftReset
	.set SoftReset, 0x0200FD1D
	.global VBlankIntrWait
	.set VBlankIntrWait, 0x0200FD35
	.global SoftResetRom
	.set SoftResetRom, 0x0200FD3D
	.global gMPlayTable
	.set gMPlayTable, 0x0201DEC8
	.global bss_start
	.set bss_start, 0x02020AB8
	.global gScreenIsFadedOut
	.set gScreenIsFadedOut, 0x02020AC0
	.global gNextWindowBaseBlock
	.set gNextWindowBaseBlock, 0x02020AC4
	.global gBattleMenuMonId
	.set gBattleMenuMonId, 0x02020AC8
	.global gMPlayTrack_BGM
	.set gMPlayTrack_BGM, 0x02020AD0
	.global gSoundMainRAMBufferAddress
	.set gSoundMainRAMBufferAddress, 0x02021434
	.global gSaveLoadResult
	.set gSaveLoadResult, 0x02021834
	.global gBerryFixResult
	.set gBerryFixResult, 0x02021838
	.global gStringBuffers
	.set gStringBuffers, 0x02021840
	.global gPartyMenuState
	.set gPartyMenuState, 0x020218E0
	.global gMonSummaryScreen
	.set gMonSummaryScreen, 0x02021A10
	.global gMessageWindowPtr
	.set gMessageWindowPtr, 0x02021A64
	.global gBattleMenuInputState
	.set gBattleMenuInputState, 0x02021A70
	.global gSoundInfo
	.set gSoundInfo, 0x02021AC0
	.global gMPlayJumpTable
	.set gMPlayJumpTable, 0x02022A70
	.global gCgbChans
	.set gCgbChans, 0x02022B00
	.global gMPlayInfo_BGM
	.set gMPlayInfo_BGM, 0x02022C00
	.global gMPlayMemAccArea
	.set gMPlayMemAccArea, 0x02022C40
	.global gIntrTable
	.set gIntrTable, 0x02022C50
	.global gVBlankCounter
	.set gVBlankCounter, 0x02022C88
	.global gVBlankCallback
	.set gVBlankCallback, 0x02022C8C
	.global gBgHofsBuffer
	.set gBgHofsBuffer, 0x02022C90
	.global gBgVofsBuffer
	.set gBgVofsBuffer, 0x02022C98
	.global gWindows
	.set gWindows, 0x02022CB0
	.global gNewKeys
	.set gNewKeys, 0x02022F30
	.global gNewAndRepeatedKeys
	.set gNewAndRepeatedKeys, 0x02022F34
	.global gHeldKeys
	.set gHeldKeys, 0x02022F38
	.global gPcItemsPtr
	.set gPcItemsPtr, 0x02022F3C
	.global gPokemonStoragePtr
	.set gPokemonStoragePtr, 0x02022F40
	.global gSaveBlock1BackupPtr
	.set gSaveBlock1BackupPtr, 0x02022F44
	.global gPokedexPtr
	.set gPokedexPtr, 0x02022F48
	.global gSaveBlock1Ptr
	.set gSaveBlock1Ptr, 0x02022F4C
	.global gSaveRngValue
	.set gSaveRngValue, 0x02022F50
	.global gFlagsPtr
	.set gFlagsPtr, 0x02022F54
	.global gSaveBlock2Ptr
	.set gSaveBlock2Ptr, 0x02022F58
	.global gGiftRibbonsPtr
	.set gGiftRibbonsPtr, 0x02022F5C
	.global gDexSeen3Ptr
	.set gDexSeen3Ptr, 0x02022F60
	.global gVarsPtr
	.set gVarsPtr, 0x02022F64
	.global gEnigmaBerryPtr
	.set gEnigmaBerryPtr, 0x02022F68
	.global gPlayerPartyBackupPtr
	.set gPlayerPartyBackupPtr, 0x02022F6C
	.global gPlayerPartyCountPtr
	.set gPlayerPartyCountPtr, 0x02022F70
	.global gPlayerPartyPtr
	.set gPlayerPartyPtr, 0x02022F74
	.global gDexSeen2Ptr
	.set gDexSeen2Ptr, 0x02022F78
	.global gLastSaveBlockPtr
	.set gLastSaveBlockPtr, 0x02022F7C
	.global gLastWrittenSector
	.set gLastWrittenSector, 0x02022F80
	.global gLastSaveCounter
	.set gLastSaveCounter, 0x02022F84
	.global gSaveStatusFlags
	.set gSaveStatusFlags, 0x02022F90
	.global gLastKnownGoodSector
	.set gLastKnownGoodSector, 0x02022FA4
	.global gDamagedSaveSectors
	.set gDamagedSaveSectors, 0x02022FA8
	.global gSaveCounter
	.set gSaveCounter, 0x02022FAC
	.global gReadWriteSector
	.set gReadWriteSector, 0x02022FB0
	.global gSaveReadBuffer
	.set gSaveReadBuffer, 0x02022FC0
	.global gIncrementalSectorId
	.set gIncrementalSectorId, 0x02023FC0
	.global gPlayerLinkInfo
	.set gPlayerLinkInfo, 0x02023FD0
	.global gTextBuffer
	.set gTextBuffer, 0x02024250
	.global gMonLinkData
	.set gMonLinkData, 0x020249E0
	.global gJoyBusState
	.set gJoyBusState, 0x02025270
	.global gFlashTimeoutFlag
	.set gFlashTimeoutFlag, 0x020252A4
	.global PollFlashStatus
	.set PollFlashStatus, 0x020252A8
	.global WaitForFlashWrite
	.set WaitForFlashWrite, 0x020252AC
	.global ProgramFlashSector
	.set ProgramFlashSector, 0x020252B0
	.global gFlash
	.set gFlash, 0x020252B4
	.global ProgramFlashByte
	.set ProgramFlashByte, 0x020252B8
	.global gFlashNumRemainingBytes
	.set gFlashNumRemainingBytes, 0x020252BC
	.global EraseFlashChip
	.set EraseFlashChip, 0x020252C0
	.global EraseFlashSector
	.set EraseFlashSector, 0x020252C4
	.global gFlashMaxTime
	.set gFlashMaxTime, 0x020252C8
	.global gUpdateSuccessful
	.set gUpdateSuccessful, 0x020252CC
	.global gGameVersion
	.set gGameVersion, 0x020252D0
	.global gTimeSinceBerryUpdate
	.set gTimeSinceBerryUpdate, 0x020252D8
	.global gRtcUTCTime
	.set gRtcUTCTime, 0x020252E0
	.global bss_end
	.set bss_end, 0x020252EC
	.global gSaveBlockLoadRegions
	.set gSaveBlockLoadRegions, 0x020252EC
	.global GPIOPortDirection
	.set GPIOPortDirection, 0x080000C6


	.global CpuSet
	.thumb_set CpuSet, 0x0200FCF9

	.global gBgTilemapBufferTransferScheduled
	.set gBgTilemapBufferTransferScheduled, 0x02022CA0

	.global gBattleMenuState
	.set gBattleMenuState, 0x02021AA0

	.global gLinkCommandReady
	.set gLinkCommandReady, 0x020249D0

	.global gTargetIsRubySapphire
	.set gTargetIsRubySapphire, 0x0202526C

	.global gRomDetection_IsEnglishROM
	.set gRomDetection_IsEnglishROM, 0x02025264

	.global gAgbPmRomParams
	.set gAgbPmRomParams, 0x02025268

	.global gLinkMonNameBuffer
	.set gLinkMonNameBuffer, 0x02021A80

	.global gRomHeader
	.set gRomHeader, 0x02000004
