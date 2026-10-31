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
	.set SoundMainBTM, 0x020063CD
	.global RealClearChain
	.set RealClearChain, 0x020063E5
	.global ply_fine
	.set ply_fine, 0x02006405
	.global MPlayJumpTableCopy
	.set MPlayJumpTableCopy, 0x02006435
	.global ply_goto
	.set ply_goto, 0x02006475
	.global ply_patt
	.set ply_patt, 0x02006495
	.global ply_pend
	.set ply_pend, 0x020064B1
	.global ply_rept
	.set ply_rept, 0x020064C5
	.global ply_prio
	.set ply_prio, 0x020064F5
	.global ply_tempo
	.set ply_tempo, 0x02006501
	.global ply_keysh
	.set ply_keysh, 0x02006515
	.global ply_voice
	.set ply_voice, 0x02006529
	.global ply_vol
	.set ply_vol, 0x02006559
	.global ply_pan
	.set ply_pan, 0x0200656D
	.global ply_bend
	.set ply_bend, 0x02006581
	.global ply_bendr
	.set ply_bendr, 0x02006595
	.global ply_lfodl
	.set ply_lfodl, 0x020065A9
	.global ply_modt
	.set ply_modt, 0x020065B5
	.global ply_tune
	.set ply_tune, 0x020065CD
	.global ply_port
	.set ply_port, 0x020065E1
	.global m4aSoundVSync
	.set m4aSoundVSync, 0x020065F9
	.global MPlayMain
	.set MPlayMain, 0x02006631
	.global TrackStop
	.set TrackStop, 0x0200689D
	.global ply_note
	.set ply_note, 0x020068E1
	.global ply_endtie
	.set ply_endtie, 0x02006AF9
	.global ply_lfos
	.set ply_lfos, 0x02006B61
	.global ply_mod
	.set ply_mod, 0x02006B75
	.global GetPicUncompPtr
	.set GetPicUncompPtr, 0x0200935D
	.global Div
	.set Div, 0x0200DC5D
	.global Mod
	.set Mod, 0x0200DC61
	.global HuffUnComp
	.set HuffUnComp, 0x0200DC69
	.global LZ77UnCompWram
	.set LZ77UnCompWram, 0x0200DC71
	.global RLUnCompVram
	.set RLUnCompVram, 0x0200DC75
	.global RegisterRamReset
	.set RegisterRamReset, 0x0200DC79
	.global VBlankIntrWait
	.set VBlankIntrWait, 0x0200DC95
	.global gMPlayTable
	.set gMPlayTable, 0x0201724C
	.global bss_start
	.set bss_start, 0x02019E9C
	.global gMPlayTrack_BGM
	.set gMPlayTrack_BGM, 0x02019EB0
	.global gSoundMainRAMBufferAddress
	.set gSoundMainRAMBufferAddress, 0x0201A7E0
	.global gSoundInfo
	.set gSoundInfo, 0x0201AE70
	.global gMPlayJumpTable
	.set gMPlayJumpTable, 0x0201BE20
	.global gCgbChans
	.set gCgbChans, 0x0201BEB0
	.global gMPlayInfo_BGM
	.set gMPlayInfo_BGM, 0x0201BFB0
	.global gMPlayMemAccArea
	.set gMPlayMemAccArea, 0x0201BFF0
	.global gIntrTable
	.set gIntrTable, 0x0201C000
	.global gVBlankCounter
	.set gVBlankCounter, 0x0201C038
	.global gVBlankCallback
	.set gVBlankCallback, 0x0201C03C
	.global gNewAndRepeatedKeys
	.set gNewAndRepeatedKeys, 0x02020294
	.global gHeldKeys
	.set gHeldKeys, 0x02020298
	.global gLastWrittenSector
	.set gLastWrittenSector, 0x020202E0
	.global gLastSaveCounter
	.set gLastSaveCounter, 0x020202E4
	.global gSaveStatusFlags
	.set gSaveStatusFlags, 0x020202F0
	.global gLastKnownGoodSector
	.set gLastKnownGoodSector, 0x02020304
	.global gDamagedSaveSectors
	.set gDamagedSaveSectors, 0x02020308
	.global gSaveCounter
	.set gSaveCounter, 0x0202030C
	.global gReadWriteSector
	.set gReadWriteSector, 0x02020310
	.global gSaveReadBuffer
	.set gSaveReadBuffer, 0x02020320
	.global gIncrementalSectorId
	.set gIncrementalSectorId, 0x02021320
	.global gFlashTimeoutFlag
	.set gFlashTimeoutFlag, 0x02022184
	.global PollFlashStatus
	.set PollFlashStatus, 0x02022188
	.global WaitForFlashWrite
	.set WaitForFlashWrite, 0x0202218C
	.global ProgramFlashSector
	.set ProgramFlashSector, 0x02022190
	.global gFlash
	.set gFlash, 0x02022194
	.global ProgramFlashByte
	.set ProgramFlashByte, 0x02022198
	.global gFlashNumRemainingBytes
	.set gFlashNumRemainingBytes, 0x0202219C
	.global EraseFlashChip
	.set EraseFlashChip, 0x020221A0
	.global EraseFlashSector
	.set EraseFlashSector, 0x020221A4
	.global gFlashMaxTime
	.set gFlashMaxTime, 0x020221A8
	.global bss_end
	.set bss_end, 0x020221AC




	.global SoftResetRom
	.thumb_set SoftResetRom, 0x0200DC9D

























	.global gJoyBusState
	.set gJoyBusState, 0x02022150









	.global gTextBuffer
	.set gTextBuffer, 0x020215B0

















	.global gJapaneseSummaryTilemapBuffers
	.set gJapaneseSummaryTilemapBuffers, 0x0201E040







































	.global gRomHeader
	.set gRomHeader, 0x02000004





	.global gBgHofsBuffer
	.set gBgHofsBuffer, 0x02020040

	.global gBgVofsBuffer
	.set gBgVofsBuffer, 0x02020048











	.global gBattleMenuInputState
	.set gBattleMenuInputState, 0x0201AE20

	.global gBattleMenuMonId
	.set gBattleMenuMonId, 0x02019EA8




	.global ShowPartyMenu
	.thumb_set ShowPartyMenu, 0x0200272D










	.global gText_BattleType
	.set gText_BattleType, 0x02019879







	.global gStringBuffers
	.set gStringBuffers, 0x0201ABF0





	.global gSaveBlockLoadRegions
	.set gSaveBlockLoadRegions, 0x020221AC

	.global gSaveRngValue
	.set gSaveRngValue, 0x020202B0

	.global gSaveBlock1Ptr
	.set gSaveBlock1Ptr, 0x020202AC


	.global gSaveBlock2Ptr
	.set gSaveBlock2Ptr, 0x020202B8

	.global gPokemonStoragePtr
	.set gPokemonStoragePtr, 0x020202A0

	.global gSaveBlock1BackupPtr
	.set gSaveBlock1BackupPtr, 0x020202A4


	.global gPlayerPartyBackupPtr
	.set gPlayerPartyBackupPtr, 0x020202C8

	.global gPlayerPartyCountPtr
	.set gPlayerPartyCountPtr, 0x020202CC

	.global gPokedexPtr
	.set gPokedexPtr, 0x020202A8

	.global gDexSeen2Ptr
	.set gDexSeen2Ptr, 0x020202D4

	.global gDexSeen3Ptr
	.set gDexSeen3Ptr, 0x020202C0

	.global gFlagsPtr
	.set gFlagsPtr, 0x020202B4



	.global gPcItemsPtr
	.set gPcItemsPtr, 0x0202029C



	.global gWindows
	.set gWindows, 0x02020060



	.global sFontHalfrowLookupTablePtr
	.set sFontHalfrowLookupTablePtr, 0x0201A7BC



	.global gPlayerLinkInfo
	.set gPlayerLinkInfo, 0x02021330



	.global gGiftRibbonsPtr
	.set gGiftRibbonsPtr, 0x020202BC




	.global gPartyMenuState
	.set gPartyMenuState, 0x0201AC90




	.global gTargetIsRubySapphire
	.set gTargetIsRubySapphire, 0x02022148








	.global gEnigmaBerryPtr
	.set gEnigmaBerryPtr, 0x020202C4

	.global gScreenIsFadedOut
	.set gScreenIsFadedOut, 0x02019EA0

	.global gBgTilemapBuffers
	.set gBgTilemapBuffers, 0x0201C040

	.global gNextWindowBaseBlock
	.set gNextWindowBaseBlock, 0x02019EA4

	.global gFont1LatinGfx
	.set gFont1LatinGfx, 0x0200FF8C

	.global gFont1LatinWidths
	.set gFont1LatinWidths, 0x02010D4C

	.global gFont0LatinGfx
	.set gFont0LatinGfx, 0x0200DECC

	.global gFont0LatinWidths
	.set gFont0LatinWidths, 0x0200FE8C

	.global SoftReset
	.thumb_set SoftReset, 0x0200DC7D





	.global gSaveLoadResult
	.set gSaveLoadResult, 0x0201ABE0

	.global gMessageWindowPtr
	.set gMessageWindowPtr, 0x0201AE14

	.global gNewKeys
	.set gNewKeys, 0x02020290

	.global gLinkCommandReady
	.set gLinkCommandReady, 0x02021AF0

	.global gPlayerPartyPtr
	.set gPlayerPartyPtr, 0x020202D0

	.global gMonSummaryScreen
	.set gMonSummaryScreen, 0x0201ADC0

	.global LZ77UnCompVram
	.thumb_set LZ77UnCompVram, 0x0200DC6D

	.global gBgTilemapBufferTransferScheduled
	.set gBgTilemapBufferTransferScheduled, 0x02020050

	.global gBattleMenuState
	.set gBattleMenuState, 0x0201AE50

	.global CpuSet
	.thumb_set CpuSet, 0x0200DC59

	.global gLinkMonNameBuffer
	.set gLinkMonNameBuffer, 0x0201AE30

	.global gMonLinkData
	.set gMonLinkData, 0x02021B00

	.global gAgbPmRomParams
	.set gAgbPmRomParams, 0x02022144

	.global gLastSaveBlockPtr
	.set gLastSaveBlockPtr, 0x020202D8
