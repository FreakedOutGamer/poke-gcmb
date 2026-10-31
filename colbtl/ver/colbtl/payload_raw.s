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
	.set SoundMainBTM, 0x02006809
	.global RealClearChain
	.set RealClearChain, 0x02006821
	.global ply_fine
	.set ply_fine, 0x02006841
	.global MPlayJumpTableCopy
	.set MPlayJumpTableCopy, 0x02006871
	.global ply_goto
	.set ply_goto, 0x020068B1
	.global ply_patt
	.set ply_patt, 0x020068D1
	.global ply_pend
	.set ply_pend, 0x020068ED
	.global ply_rept
	.set ply_rept, 0x02006901
	.global ply_prio
	.set ply_prio, 0x02006931
	.global ply_tempo
	.set ply_tempo, 0x0200693D
	.global ply_keysh
	.set ply_keysh, 0x02006951
	.global ply_voice
	.set ply_voice, 0x02006965
	.global ply_vol
	.set ply_vol, 0x02006995
	.global ply_pan
	.set ply_pan, 0x020069A9
	.global ply_bend
	.set ply_bend, 0x020069BD
	.global ply_bendr
	.set ply_bendr, 0x020069D1
	.global ply_lfodl
	.set ply_lfodl, 0x020069E5
	.global ply_modt
	.set ply_modt, 0x020069F1
	.global ply_tune
	.set ply_tune, 0x02006A09
	.global ply_port
	.set ply_port, 0x02006A1D
	.global m4aSoundVSync
	.set m4aSoundVSync, 0x02006A35
	.global MPlayMain
	.set MPlayMain, 0x02006A6D
	.global TrackStop
	.set TrackStop, 0x02006CD9
	.global ply_note
	.set ply_note, 0x02006D1D
	.global ply_endtie
	.set ply_endtie, 0x02006F35
	.global ply_lfos
	.set ply_lfos, 0x02006F9D
	.global ply_mod
	.set ply_mod, 0x02006FB1
	.global CpuSet
	.set CpuSet, 0x0200FBC5
	.global Div
	.set Div, 0x0200FBC9
	.global Mod
	.set Mod, 0x0200FBCD
	.global HuffUnComp
	.set HuffUnComp, 0x0200FBD5
	.global LZ77UnCompWram
	.set LZ77UnCompWram, 0x0200FBDD
	.global RLUnCompVram
	.set RLUnCompVram, 0x0200FBE1
	.global RegisterRamReset
	.set RegisterRamReset, 0x0200FBE5
	.global SoftReset
	.set SoftReset, 0x0200FBE9
	.global VBlankIntrWait
	.set VBlankIntrWait, 0x0200FC01
	.global SoftResetRom
	.set SoftResetRom, 0x0200FC09
	.global gMPlayTable
	.set gMPlayTable, 0x0201DCB8
	.global bss_start
	.set bss_start, 0x020208D8
	.global gScreenIsFadedOut
	.set gScreenIsFadedOut, 0x020208E0
	.global gNextWindowBaseBlock
	.set gNextWindowBaseBlock, 0x020208E4
	.global gBattleMenuMonId
	.set gBattleMenuMonId, 0x020208E8
	.global gMPlayTrack_BGM
	.set gMPlayTrack_BGM, 0x020208F0
	.global gSoundMainRAMBufferAddress
	.set gSoundMainRAMBufferAddress, 0x02021254
	.global gSaveLoadResult
	.set gSaveLoadResult, 0x02021654
	.global gBerryFixResult
	.set gBerryFixResult, 0x02021658
	.global gPartyMenuState
	.set gPartyMenuState, 0x02021700
	.global gMessageWindowPtr
	.set gMessageWindowPtr, 0x02021884
	.global gBattleMenuInputState
	.set gBattleMenuInputState, 0x02021890
	.global gLinkMonNameBuffer
	.set gLinkMonNameBuffer, 0x020218A0
	.global gSoundInfo
	.set gSoundInfo, 0x020218E0
	.global gMPlayJumpTable
	.set gMPlayJumpTable, 0x02022890
	.global gCgbChans
	.set gCgbChans, 0x02022920
	.global gMPlayInfo_BGM
	.set gMPlayInfo_BGM, 0x02022A20
	.global gMPlayMemAccArea
	.set gMPlayMemAccArea, 0x02022A60
	.global gIntrTable
	.set gIntrTable, 0x02022A70
	.global gVBlankCounter
	.set gVBlankCounter, 0x02022AA8
	.global gVBlankCallback
	.set gVBlankCallback, 0x02022AAC
	.global gBgHofsBuffer
	.set gBgHofsBuffer, 0x02022AB0
	.global gBgVofsBuffer
	.set gBgVofsBuffer, 0x02022AB8
	.global gWindows
	.set gWindows, 0x02022AD0
	.global gNewAndRepeatedKeys
	.set gNewAndRepeatedKeys, 0x02022D54
	.global gHeldKeys
	.set gHeldKeys, 0x02022D58
	.global gPcItemsPtr
	.set gPcItemsPtr, 0x02022D5C
	.global gPokemonStoragePtr
	.set gPokemonStoragePtr, 0x02022D60
	.global gSaveBlock1BackupPtr
	.set gSaveBlock1BackupPtr, 0x02022D64
	.global gPokedexPtr
	.set gPokedexPtr, 0x02022D68
	.global gSaveBlock1Ptr
	.set gSaveBlock1Ptr, 0x02022D6C
	.global gSaveRngValue
	.set gSaveRngValue, 0x02022D70
	.global gFlagsPtr
	.set gFlagsPtr, 0x02022D74
	.global gSaveBlock2Ptr
	.set gSaveBlock2Ptr, 0x02022D78
	.global gGiftRibbonsPtr
	.set gGiftRibbonsPtr, 0x02022D7C
	.global gDexSeen3Ptr
	.set gDexSeen3Ptr, 0x02022D80
	.global gVarsPtr
	.set gVarsPtr, 0x02022D84
	.global gEnigmaBerryPtr
	.set gEnigmaBerryPtr, 0x02022D88
	.global gPlayerPartyBackupPtr
	.set gPlayerPartyBackupPtr, 0x02022D8C
	.global gPlayerPartyCountPtr
	.set gPlayerPartyCountPtr, 0x02022D90
	.global gDexSeen2Ptr
	.set gDexSeen2Ptr, 0x02022D98
	.global gLastSaveBlockPtr
	.set gLastSaveBlockPtr, 0x02022D9C
	.global gLastWrittenSector
	.set gLastWrittenSector, 0x02022DA0
	.global gLastSaveCounter
	.set gLastSaveCounter, 0x02022DA4
	.global gSaveStatusFlags
	.set gSaveStatusFlags, 0x02022DB0
	.global gLastKnownGoodSector
	.set gLastKnownGoodSector, 0x02022DC4
	.global gDamagedSaveSectors
	.set gDamagedSaveSectors, 0x02022DC8
	.global gSaveCounter
	.set gSaveCounter, 0x02022DCC
	.global gReadWriteSector
	.set gReadWriteSector, 0x02022DD0
	.global gSaveReadBuffer
	.set gSaveReadBuffer, 0x02022DE0
	.global gIncrementalSectorId
	.set gIncrementalSectorId, 0x02023DE0
	.global gPlayerLinkInfo
	.set gPlayerLinkInfo, 0x02023DF0
	.global gTextBuffer
	.set gTextBuffer, 0x02024070
	.global gMonLinkData
	.set gMonLinkData, 0x02024800
	.global gJoyBusState
	.set gJoyBusState, 0x02025090
	.global gFlashTimeoutFlag
	.set gFlashTimeoutFlag, 0x020250C4
	.global PollFlashStatus
	.set PollFlashStatus, 0x020250C8
	.global WaitForFlashWrite
	.set WaitForFlashWrite, 0x020250CC
	.global ProgramFlashSector
	.set ProgramFlashSector, 0x020250D0
	.global gFlash
	.set gFlash, 0x020250D4
	.global ProgramFlashByte
	.set ProgramFlashByte, 0x020250D8
	.global gFlashNumRemainingBytes
	.set gFlashNumRemainingBytes, 0x020250DC
	.global EraseFlashChip
	.set EraseFlashChip, 0x020250E0
	.global EraseFlashSector
	.set EraseFlashSector, 0x020250E4
	.global gFlashMaxTime
	.set gFlashMaxTime, 0x020250E8
	.global gUpdateSuccessful
	.set gUpdateSuccessful, 0x020250EC
	.global gGameVersion
	.set gGameVersion, 0x020250F0
	.global gTimeSinceBerryUpdate
	.set gTimeSinceBerryUpdate, 0x020250F8
	.global gRtcUTCTime
	.set gRtcUTCTime, 0x02025100
	.global bss_end
	.set bss_end, 0x0202510C
	.global gSaveBlockLoadRegions
	.set gSaveBlockLoadRegions, 0x0202510C
	.global GPIOPortDirection
	.set GPIOPortDirection, 0x080000C6


















	.global gMonSummaryScreen
	.set gMonSummaryScreen, 0x02021830


	.global LZ77UnCompVram
	.thumb_set LZ77UnCompVram, 0x0200FBD9





	.global gTargetIsRubySapphire
	.set gTargetIsRubySapphire, 0x0202508C

	.global gRomDetection_IsEnglishROM
	.set gRomDetection_IsEnglishROM, 0x02025084

	.global gAgbPmRomParams
	.set gAgbPmRomParams, 0x02025088


	.global gBattleMenuState
	.set gBattleMenuState, 0x020218C0


	.global gBgTilemapBufferTransferScheduled
	.set gBgTilemapBufferTransferScheduled, 0x02022AC0


	.global gPlayerPartyPtr
	.set gPlayerPartyPtr, 0x02022D94


	.global gLinkCommandReady
	.set gLinkCommandReady, 0x020247F0

	.global gStringBuffers
	.set gStringBuffers, 0x02021660

	.global gNewKeys
	.set gNewKeys, 0x02022D50

	.global gRomHeader
	.set gRomHeader, 0x02000004
