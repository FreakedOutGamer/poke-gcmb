/* The image is streamed over the link cable by the GameCube side and runs
   in place from EWRAM, so there is no ROM section. */

ENTRY(Start)

GPIOPortDirection = 0x080000C6;
gJoyBusExternalEventBuf = 0x02017CC8;   /* referenced by TransmitGiftData */
gAgbPmRomParams      = 0x02017CE4;   /* written by DetectROM */
gTargetIsRubySapphire = 0x02017CE8;
gJoyBusState         = 0x02017CF0;
gSaveBlock1Ptr       = 0x020159C0;
gSaveBlock1BackupPtr = 0x020159B8;
gSaveBlock2Ptr       = 0x020159CC;
gPokemonStoragePtr   = 0x020159B4;
gFlagsPtr            = 0x020159C8;
gGiftRibbonsPtr      = 0x020159D0;
gPlayerPartyCountPtr = 0x020159E4;   /* read by JoyBusHandleCmdAA; sits right before gPlayerPartyPtr, matching sb1PartyCountOffset before sb1PartyOffset */
gPlayerPartyPtr      = 0x020159E8;

/* ReceiveJirachi's own scratch, reached as fixed addresses rather than through
   &gJoyBusState + offset the way JOYBUS_EXTRA's callers do; numerically
   gReceivedGiftMonBuffer is gJoyBusState + 0x2A3C, one word into the same
   buffer ProcessReceiveCommand fills for its 0x99 command. */
gReceivedGiftMonBuffer    = 0x0201A72C;
gReceiveSuccessGraphicsBuffer    = 0x02019928;
gSaveBlockLoadRegions      = 0x0201A7DC;
gVarsPtr             = 0x020159D8;
gHeldKeys            = 0x020159AC;
gNewKeys             = 0x020159A4;
gNewAndRepeatedKeys  = 0x020159A8;
sKeyRepeatDelay = 0x020118F4;
sKeyRepeatRate = 0x020118F6;
sKeyRepeatTimer    = 0x020118F8;
gSaveSectorWipeAttempts    = 0x020118FC;
gIntrTable           = 0x02011950;
gBgTilemapBuffers    = 0x02011990;
gBgHofsBuffer        = 0x02015990;
gBgVofsBuffer        = 0x02015998;
gBgTilemapBufferTransferScheduled = 0x020159A0;
gVBlankCounter       = 0x02011988;
gVBlankCallback      = 0x0201198C;
gReceiveSceneSprite1    = 0x02011260;   /* InitReceiveScene's own sprite, set by AddSprite */
gReceiveSceneSprite2    = 0x02011264;   /* likewise */
sSprites             = 0x02011268;
sOamBuffer           = 0x020114F0;
gSaveFileStatus    = 0x02011900;
gPokedexPtr          = 0x020159BC;
gDexSeen2Ptr         = 0x020159EC;
gDexSeen3Ptr         = 0x020159D4;
gPcItemsPtr          = 0x020159B0;
gSaveRngValue            = 0x020159C4;
gPlayerPartyBackupPtr = 0x020159E0;
gEnigmaBerryPtr      = 0x020159DC;
gLastSaveBlockPtr   = 0x020159F0;

/* main.c's RunJirachiDistributor, the distributor's main loop. */
gTargetGameIdentificationResult    = 0x02011944;   /* DetectROM's result */
gSaveLoadResult    = 0x02011948;   /* ReadSaveBlockChunks's result */
gBerryFixResult    = 0x0201194C;   /* the BerryFix loop's summary result */
gMonLinkData    = 0x02017460;   /* zeroed before GetPtrToEmptyPartySlot runs */



/* save.c's globals. Ruby/Sapphire has these in IWRAM at 0x03005xxx; with no
   IWRAM data section here they sit in EWRAM past the sector buffer. */
gLastWrittenSector     = 0x02015A00;
gLastSaveCounter     = 0x02015A04;
gSaveStatusFlags    = 0x02015A10;
gLastKnownGoodSector = 0x02015A24;
gDamagedSaveSectors  = 0x02015A28;
gSaveCounter         = 0x02015A2C;
gReadWriteSector     = 0x02015A30;
gSaveReadBuffer         = 0x02015A40;
gIncrementalSectorId    = 0x02016A40;

/* player_record.c's scratch, above the save sector buffer. */
gPlayerLinkInfo    = 0x02016A50;
gTextBuffer      = 0x02016CD0;
gLinkCommandReady  = 0x02017450;

SECTIONS
{
    . = 0x02000000;

    .text : ALIGN(4)
    {
        *(.header)          /* 0x02000000  192-byte AGB header       */
        *(.crt0)            /* 0x020000C0  Init + IntrMain (ARM)     */
        INCLUDE ld_text.ld
        . = ALIGN(4);
    }

    .rodata : ALIGN(4)
    {
        build/src/graphics.o(.rodata)   /* 0x02007698 */
        build/src/spinda.o(.rodata)             /* 20101B8 */
        build/src/pokemon_tables.o(.rodata)        /* 2010248 */
        . = ALIGN(4);
        build/src/libpmagb/pokedex.o(.rodata)           /* 20102EC */
        . = ALIGN(4);
        build/src/libpmagb/string_util.o(.rodata)       /* 2010624 */
        build/src/libpmagb/map_check.o(.rodata)         /* 201063C */
        build/src/agb_flash.o(.rodata)    /* 2010688 */
        build/src/agb_flash_1m.o(.rodata) /* 20106F8  FLASH1M_V103 */
        build/src/agb_flash_mx.o(.rodata) /* 201073C */
        build/src/agb_flash_le.o(.rodata) /* 20107D8 */
        build/src/berry_fix.o(.rodata)             /* 2010820 */
        build/src/berry_fix_rtc.o(.rodata)              /* 201086C */
        build/src/rtc.o(.rodata)               /* 201088C */
        build/src/berry_fix_event_data.o(.rodata)       /* 20108F0 */
        build/src/siirtc.o(.rodata)       /* 20108FC  SIIRTC_V001 */
        build/src/variables.o(.data)               /* 2010934 */
        build/src/scene.o(.rodata)         /* 2010A38 */
        build/src/libpmagb/save_chunks.o(.data)         /* 2010B7C */
        build/src/libpmagb/save_chunks.o(.rodata)        /* 2010BEC */
        build/src/libpmagb/target_game.o(.rodata)       /* 2010C5C */
        *(.rodata)
        *(.rodata.*)
        . = ALIGN(4);
    }

    __image_end = .;    /* 201125C */

    /* Scratch above the image. Each block is pinned to the address its
       literal pools use. */
    . = 0x02011910;
    .bss.berry_fix_main (NOLOAD) : { build/src/berry_fix.o(.bss) }

    . = 0x02011918;
    .bss.berry_fix_rtc (NOLOAD) : { build/src/berry_fix_rtc.o(.bss) }

    . = 0x02011928;
    .bss.rtc (NOLOAD) : { build/src/rtc.o(.bss) }

    . = 0x02011904;
    .bss.flash (NOLOAD) : { build/src/agb_flash.o(.bss) }

    . = 0x02011940;
    .bss.siirtc (NOLOAD) : { build/src/siirtc.o(.bss) }

    . = 0x0201A798;
    .bss.common (NOLOAD) : { build/src/common_bss.o(.bss) *(COMMON) }

    .bss (NOLOAD) : ALIGN(4)
    {
        __bss_start = .;
        *(.bss)
        *(.bss.*)
        . = ALIGN(4);
        __bss_end = .;
    }

    /DISCARD/ : {
        *(.comment) *(.ARM.attributes) *(.note.*)
    }
}
