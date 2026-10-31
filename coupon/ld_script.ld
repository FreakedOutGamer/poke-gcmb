/* The image is streamed over the link cable by the GameCube side and runs
   in place from EWRAM, so there is no ROM section. */

ENTRY(Start)

/* Work RAM the program uses at fixed addresses. */
gIntrTable = 0x0200F5B0;
gVBlankCounter = 0x0200F5E8;
gVBlankCallback = 0x0200F5EC;
gBgTilemapBuffers = 0x0200F5F0;
gBgHofsBuffer = 0x020135F0;
gBgVofsBuffer = 0x020135F8;
gBgTilemapBufferTransferScheduled = 0x02013600;
gWindows = 0x02013610;
sFontHalfrowLookupTablePtr = 0x0200F0D4;
gScreenIsFadedOut = 0x0200EA40;
gStringBuffers = 0x0200F500;
gLinkCommandMessageId = 0x0200F5A0;
gMessageWindowPtr = 0x0200F5A4;
sSprites = 0x0200EA48;
sOamBuffer = 0x0200ECD0;
sKeyRepeatDelay = 0x0200F0D8;
sKeyRepeatRate = 0x0200F0DA;
sKeyRepeatTimer = 0x0200F0DC;
gNewKeys = 0x02013840;
gNewAndRepeatedKeys = 0x02013844;
gHeldKeys = 0x02013848;
bss_start = 0x0200EA3C;
bss_end = 0x02015770;
/* The save blocks are placed at random offsets past the end of bss. */
gSaveBlockLoadRegions = 0x02015770;
gSaveSectorWipeAttempts = 0x0200F0E0;
gSaveLoadResult = 0x0200F4F4;
gSaveFileStatus = 0x0200F0E4;
gPokemonStoragePtr = 0x02013850;
gSaveBlock1BackupPtr = 0x02013854;
gLastWrittenSector = 0x02013890;
gLastSaveCounter = 0x02013894;
gSaveStatusFlags = 0x020138A0;
gLastKnownGoodSector = 0x020138B4;
gDamagedSaveSectors = 0x020138B8;
gSaveCounter = 0x020138BC;
gReadWriteSector = 0x020138C0;
gSaveReadBuffer = 0x020138D0;
gIncrementalSectorId = 0x020148D0;
gPlayerPartyPtr = 0x02013880;
gPlayerPartyCountPtr = 0x0201387C;
gPlayerPartyBackupPtr = 0x02013878;
gAgbPmRomParams = 0x020156EC;
gTargetIsRubySapphire = 0x020156F0;
gSaveBlock1Ptr = 0x0201385C;
gSaveRngValue = 0x02013860;
gLastSaveBlockPtr = 0x02013888;
gSaveBlock2Ptr = 0x02013868;
gGiftRibbonsPtr = 0x0201386C;
gJoyBusState = 0x02015710;
gMonLinkData = 0x020150B0;
gCouponLinkData = 0x020150B0;
gCouponTransferBuffer = 0x0201568C;
gPcItemsPtr = 0x0201384C;
gEnigmaBerryPtr = 0x02013874;
gBerries = gBerryInfo - 0x1C;
gPokedexPtr = 0x02013858;
gDexSeen2Ptr = 0x02013884;
gDexSeen3Ptr = 0x02013870;
gFlagsPtr = 0x02013864;
gPlayerLinkInfo = 0x020148E0;
gTextBuffer = 0x02014B60;
gLinkCommandReady = 0x020150A0;
gCrc32Table = 0x0200F0E8;
sTimerNum = 0x0200F4E8;
sTimerCount = 0x0200F4EA;
sTimerReg = 0x0200F4EC;
sSavedIme = 0x0200F4F0;
gFlashTimeoutFlag = 0x02015748;
PollFlashStatus = 0x0201574C;
WaitForFlashWrite = 0x02015750;
ProgramFlashSector = 0x02015754;
gFlash = 0x02015758;
ProgramFlashByte = 0x0201575C;
gFlashNumRemainingBytes = 0x02015760;
EraseFlashChip = 0x02015764;
EraseFlashSector = 0x02015768;
gFlashMaxTime = 0x0201576C;

SECTIONS
{
    . = 0x02000000;

    .text : ALIGN(4)
    {
        build/asm/rom_header.o(.header)     /* 0x02000000  192-byte AGB header */
        build/asm/crt0.o(.crt0)             /* 0x020000C0  Init + IntrMain (ARM) */
        INCLUDE ld_text.ld
        build/data/font_data.o(.rodata.fonts)
        build/data/display_data.o(.rodata.main_display)
        build/data/spinda_spots.o(.rodata.spinda_spots)
        build/src/pokemon_tables.o(.rodata)
        build/src/agb_flash.o(.rodata)
        build/src/agb_flash_1m.o(.rodata)
        build/src/agb_flash_mx.o(.rodata)
        build/src/agb_flash_le.o(.rodata)
        build/src/variables.o(.data)
        build/src/scene_data.o(.rodata)
        build/src/libpmagb/save_chunks.o(.data)
        build/src/libpmagb/save_chunks.o(.rodata)
        build/src/bad_egg.o(.rodata)
        build/src/libpmagb/target_game.o(.rodata)
    }
    __image_end = .;
    ASSERT(__image_end == 0x0200EA3C, "image size changed")

    /DISCARD/ : {
        *(.comment) *(.ARM.attributes) *(.note.*)
    }
}
