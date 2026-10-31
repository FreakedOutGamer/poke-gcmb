# Pokemon Colosseum battle multiboot image (colbtl.bin), in every language:
#
#   colbtl colbtl_e colbtl_f colbtl_g colbtl_i colbtl_jp colbtl_s
#
# Everything under src/, include/ and asm/ is shared; C code branches on
# COLBTL_VERSION (VERSION_COLBTL, VERSION_COLBTL_E, ...; see
# include/global.h) where versions differ. The build lives in
# ../multiboot.mk (see there for targets); per-version files are in
# ver/<version>/.

VERSIONS := colbtl colbtl_e colbtl_f colbtl_g colbtl_i colbtl_jp colbtl_s
# The payload is itself a complete image linked at 0x02000000; the IWRAM
# bootstrap decompresses it over the outer image and jumps to it.
PAYLOAD_BASE := 0x02000000
# gbagfx's default search reproduces every original LZ77 stream exactly.
GBAGFX_FLAGS :=
# The outer image's program (ported from colosseum-mb): crt0, AgbMain, which copies
# the bootstrap to IWRAM and runs it, and the bootstrap, which unpacks the
# payload to 0x02000000 (placed by ver/<version>/ld_script.ld).
OUTER_OBJS := outer/crt0.o outer/main.o outer/libagbsyscall.o libgcc/_call_via_rX.o outer/iwram.o
RSFONT := ../tools/rsfont/rsfont
RSFONT_SRCS := $(addprefix ../tools/rsfont/,main.c convert_png.c util.c font.c)
FONT_BINS := \
	graphics/fonts/font0_lat.2bpp \
	graphics/fonts/font1_lat.2bpp \
	graphics/fonts/font0_en_colbtl.2bpp \
	graphics/fonts/font1_en_colbtl.2bpp \
	graphics/fonts/font0_fre.2bpp \
	graphics/fonts/font1_fre.2bpp \
	graphics/fonts/font0_ger.2bpp \
	graphics/fonts/font1_ger.2bpp \
	graphics/fonts/font0_ita.2bpp \
	graphics/fonts/font1_ita.2bpp \
	graphics/fonts/font0_spa.2bpp \
	graphics/fonts/font1_spa.2bpp \
	graphics/fonts/font0_jpn.2bpp \
	graphics/fonts/font1_jpn.2bpp \
	graphics/fonts/font_null.2bpp

GRAPHICS_BINS := \
	graphics/title.8bpp.lz \
	graphics/title.gbapal.rl \
	graphics/title.bin.lz \
	graphics/title_extra.gbapal.rl \
	graphics/mon_info_icons.4bpp.lz \
	graphics/party_menu.4bpp.lz \
	graphics/message_box.4bpp.lz \
	graphics/text.gbapal.lz \
	graphics/party_menu.bin.lz \
	graphics/party_slots.bin.lz \
	graphics/summary_screen.4bpp.lz \
	graphics/summary_icons.4bpp.lz \
	graphics/summary.gbapal.lz \
	graphics/summary_screen_skills.bin.lz \
	graphics/summary_screen_moves.bin.lz \
	graphics/type_status_icons.4bpp.lz \
	graphics/type_status.gbapal.lz \
	graphics/battle_screen.4bpp.lz \
	graphics/battle.gbapal.rl \
	graphics/battle_screen.bin.lz \
	graphics/battle_windows.bin.lz

define regional-graphics
graphics/regions/$(1)/title.8bpp.rl \
graphics/regions/$(1)/title.gbapal.rl \
graphics/regions/$(1)/title.bin.lz \
graphics/regions/$(1)/title_extra.gbapal.rl \
graphics/regions/$(1)/mon_info_icons.4bpp.lz \
graphics/regions/$(1)/party_menu.4bpp.lz \
graphics/regions/$(1)/message_box.4bpp.lz \
graphics/regions/$(1)/text.gbapal.lz \
graphics/regions/$(1)/party_menu.bin.lz \
graphics/regions/$(1)/party_slots.bin.lz \
graphics/regions/$(1)/summary_screen.4bpp.lz \
graphics/regions/$(1)/summary_icons.4bpp.lz \
graphics/regions/$(1)/summary.gbapal.lz \
graphics/regions/$(1)/summary_skills.bin.lz \
graphics/regions/$(1)/summary_moves.bin.lz \
graphics/regions/$(1)/type_status_icons.4bpp.lz \
graphics/regions/$(1)/type_status.gbapal.lz \
graphics/regions/$(1)/battle_screen.4bpp.lz \
graphics/regions/$(1)/battle.gbapal.rl \
graphics/regions/$(1)/battle_screen.bin.lz \
graphics/regions/$(1)/battle_windows.bin.lz
endef

# Most C is linked as one object per function (or data item), so each
# version links exactly the ones it has, in its own order: the file is
# compiled with -ffunction-sections (-fdata-sections) and each is extracted
# with ../tools/extract_function_object.py (extract_data_object.py). These
# lists name them; object src/<file>_<name>.o comes from src/<file>.c.
FUNCTION_SPLITS := \
	src/main src/render_text src/fade src/window \
	src/party_menu src/link_message src/battle_menu \
	src/link_transfer src/pokemon_summary_screen \
	src/pokemon_2 src/pokemon_1 src/pokemon_4 \
	src/get_party_mon_pal src/get_mon_ability \
	src/ball_gfx src/mon_move_pp \
	src/pokemon_pokerus src/spinda src/pokemon_5 \
	src/set_mon_data src/give_ribbon src/pokemon_3 \
	src/mon_gender src/mon_sprite_icon \
	src/species_name src/mon_data_lookups \
	src/move_name src/get_party_slot \
	src/get_mon_gender src/libpmagb/item_id_remap \
	src/libpmagb/pokedex src/libpmagb/flag src/libpmagb/target_game \
	src/libpmagb/string_util src/libpmagb/map_check \
	src/player_record src/joybus_recv_mon src/joybus_recv \
	src/joybus_cmd src/joybus_init src/joybus_wait

src/main_FUNCS := RunLinkPartyMenu GF_Main RunBerryFix
src/render_text_FUNCS := \
	RenderText RenderText_NoPlaceholders BufferString GetStringWidth \
	NumToPmString3CustomZeroChar NumToPmString3RightAlign RenderTextAt
src/fade_FUNCS := \
	IsScreenFadedOut OverrideScreenFadeState FadeIn FadeOut
src/window_FUNCS := \
	DrawTextWindowBorder ClearVram CreateTextWindow
src/party_menu_FUNCS := \
	DrawPartyMonHealthBar CreatePartyMonHPWindow BuildTwoRowMonName \
	PutMonLvlOnWindow PutMonGenderOnBgTilemap CopyMonIconToVram \
	ConfigurePartyMenuTilemap PrintPartySelectionOrder CreateMonIcon \
	UndoLastPartySelection CreatePartyMonIconSprite DrawPartyMonSlotLeft \
	DrawPartyMonSlotRight PrintPartySelectionPrompt InitPartyMenu \
	PrintPartySelectionError HandlePartyMonActionMenu \
	HandlePartyMonSelectionMenu HandleSinglePartySelection \
	FindMonBySelectionOrder SendPartySelectionOrder \
	HandleMultiPartySelection ShowPartyMenu ResetPartyMenuState \
	ResetPartyMenuCursor
src/link_message_FUNCS := \
	InitLinkMessageScreen PrintSaveStatusMessage RunLinkStandbyLoop \
	WarningPrint ErrorPrint
src/battle_menu_FUNCS := \
	InitBattleMenu CreateBattleMonSprite PrintBattleMonGender \
	DrawBattleMonInfo PrintMenuOptions HandleMenuInput PrintMoveTypeAndPP \
	DrawMoveSelectionMenu FormatLinkMonName DrawTargetSelectionMenu \
	ConfirmGiveIn PrintBattleMenuMessage ShowBattleMenu \
	ShowBattleMonMessage RestoreBattleMenu RestoreTargetMenu \
	BufferSelectedMoveName ValidateSelectedMove GetMoveRestriction \
	IsMoveSelectionLocked GetBattleMenuRestriction GetForcedMoveSlot \
	IsMonFainted
ifeq ($(VERSION),colbtl_jp)
src/battle_menu_FUNCS += PrintBattleMenuOptions
endif
src/link_transfer_FUNCS := \
	ApplyLinkBattlePartyData FinalizeLinkSave FinalizeCouponTransfer \
	FinalizePokemonTransfer BufferFirstPartyMon GetSwitchOutRestriction \
	GetLinkSelectedMonId ShouldUseLinkPartyMenu GetNibbleByIndex \
	IsPartySlotOccupied
src/pokemon_summary_screen_FUNCS := \
	InitGraphics CreatePartyMonFrontPic PrintTrainerMemoPage \
	PrintMovesPage PrintPowerAndAccuracy ToggleMoveDetails PrintMainInfo \
	TrainerMemoHandleInput MovesPageHandleInput ShowPokemonSummaryScreen \
	PrintMoveDescription_ SpriteCB_MoveSelector
src/pokemon_2_FUNCS := \
	GetSubstruct
src/pokemon_1_FUNCS := \
	CalculateBoxMonChecksum
src/pokemon_4_FUNCS := \
	GetBoxMonData
src/get_party_mon_pal_FUNCS := \
	GetPartyMonSpritePalStruct
src/get_mon_ability_FUNCS := \
	GetBoxMonAbility
src/ball_gfx_FUNCS := \
	BoxMonGetCaughtBallItemSpriteSheet BoxMonGetCaughtBallItemPalette
src/mon_move_pp_FUNCS := \
	GetBoxMonMoveBySlot GetBoxMonPPByMoveSlot
src/pokemon_pokerus_FUNCS := \
	CheckPartyPokerus GetMonAilment CheckPartyHasHadPokerus
src/spinda_FUNCS := \
	DrawSpindaSpots
src/pokemon_5_FUNCS := \
	SetBoxMonData
src/set_mon_data_FUNCS := \
	SetMonData
src/give_ribbon_FUNCS := \
	GiveGiftRibbonToParty
src/pokemon_3_FUNCS := \
	GetMonData
src/mon_gender_FUNCS := \
	GetBoxMonGender
src/mon_sprite_icon_FUNCS := \
	GetUnownLetterByPersonality \
	GetMonSpritePalStructFromOtIdPersonality GetIconSpecies
src/species_name_FUNCS := \
	GetSpeciesName
src/mon_data_lookups_FUNCS := \
	GetMonType GetTypePaletteNum GetAbilityName \
	GetAbilityDescription CalculatePPWithBonus
src/move_name_FUNCS := \
	CopyMoveName
src/get_party_slot_FUNCS := \
	GetPtrToEmptyPartySlot
src/get_mon_gender_FUNCS := \
	GetMonGender
src/libpmagb/item_id_remap_FUNCS := \
	ItemIdToBallId
src/libpmagb/pokedex_FUNCS := \
	GetSetPokedexFlag SpeciesToNationalPokedexNum SetSpeciesCaughtFlag
src/libpmagb/flag_FUNCS := \
	FlagSet FlagClear FlagGet
src/libpmagb/target_game_FUNCS := \
	DetectROM GetPlayerMapType EmptyFunction CheckIfPokedexIsObtained \
	IsFRLG CheckGameClear
src/libpmagb/string_util_FUNCS := \
	StringLength StringCopy StringAppend StringCompare \
	GetExtCtrlCodeLength
src/libpmagb/map_check_FUNCS := \
	GetRSPlayerMapType
src/player_record_FUNCS := \
	SetPlayerLinkInfo GetPlayerLinkInfo GetTextBufferPointer
src/joybus_recv_mon_FUNCS := \
	ReceivePartyMon
src/joybus_recv_FUNCS := \
	SetLinkCommandReady IsLinkCommandReady \
	ReceiveMonData ReceiveGiftData ProcessReceiveCommand CopyBytes
src/joybus_cmd_FUNCS := \
	TransmitGiftData ProcessTransmitCommand JoyBusVCountIntr
src/joybus_init_FUNCS := \
	InitJoyBus InitLink
src/joybus_wait_FUNCS := \
	VBlankCB

DATA_SPLITS := \
	src/pokemon_tables src/berries src/text src/maps \
	src/target_games
src/pokemon_tables_FUNCS := \
	gSpindaSpotGraphics gText_Egg gBitTable gTypeToPaletteNumber \
	gPPUpGetMask gGiftRibbonMonDataIds gSpeciesToNationalPokedexNum \
	gText_BadEgg
src/berries_FUNCS := \
	gBerries
src/text_FUNCS := \
	gExtCtrlCodeLengths
src/maps_FUNCS := \
	gRSPokemonCenterMaps gRSLobbyMaps
src/target_games_FUNCS := \
	gAgbPmRomParams_AXVE_rev0 gAgbPmRomParams_AXVE_rev2 \
	gAgbPmRomParams_AXPE_rev0 gAgbPmRomParams_AXPE_rev2

# The Japanese version also links these library files split.
JP_FUNCTION_SPLITS := \
	src/libpmagb/berry src/gflib/bg src/gflib/text \
	src/libpmagb/save_block_pointers src/libpmagb/pic_uncomp src/siirtc
src/libpmagb/berry_FUNCS := \
	CopyItemName GetBerryInfo GetEnigmaBerryChecksum IsEnigmaBerryValid \
	GetItemName
src/gflib/bg_FUNCS := \
	CopyFromBgTilemapBufferRect CopyRectWithinBgTilemapBuffer \
	CopyToBgTilemapBufferRect DoGpuUpdateAndTilemapTransfers \
	FillBgTilemapBufferRect ResetGpuBuffers SetBgPos \
	SetBgTilemapBufferPaletteRect SetBgTilemapBufferTileAt
src/gflib/text_FUNCS := \
	DrawGlyphOnWindow_NoShadow DrawGlyphOnWindow_WithShadow GenerateFontHalfrowLookupTable \
	AddWindow ClearWindowCharBuffer FillWindowCharBufferRect SetTextColor \
	TextWindowFillTileBufferForText TextWindowNextLine \
	TextWindowPrintSimpleString TextWindowSetXY TextWindowShiftXY
src/libpmagb/save_block_pointers_FUNCS := SaveBlocksInit SaveRandom UpdateSaveBlockPtr
ifeq ($(VERSION),colbtl_jp)
src/libpmagb/save_block_pointers_FUNCS += GetLastSaveBlockPtr
endif
src/libpmagb/pic_uncomp_FUNCS := GetPicUncompPtr
src/siirtc_FUNCS := \
	DisableGpioPortRead EnableGpioPortRead ReadData SiiRtcProbe \
	WriteCommand WriteData

# The objects split from <file>: src/<file>_<name>.o.
split-objs = $(foreach f,$(1),$(addprefix $(f)_,$(addsuffix .o,$($(f)_FUNCS))))

GRAPHICS_NAMES := \
	gfx_title_gfx gfx_title_pal gfx_title_tilemap gfx_title_extra_pal \
	gfx_mon_info_icons gfx_party_menu gfx_message_box gfx_text_pal \
	gfx_party_menu_tilemap gfx_party_slots_tilemap gfx_summary_screen \
	gfx_summary_icons gfx_summary_pal gfx_summary_skills_tilemap \
	gfx_summary_moves_tilemap gfx_type_status_icons gfx_type_status_pal \
	gfx_battle_screen gfx_battle_pal gfx_battle_tilemap \
	gfx_battle_windows_tilemap

include ../multiboot.mk

ifneq ($(VERSION),)
GRAPHICS_OBJS := $(addprefix $(BUILD)/src/,$(addsuffix .o,$(GRAPHICS_NAMES)))
ifeq ($(VERSION),colbtl)
VERSION_GRAPHICS_BINS := $(GRAPHICS_BINS)
else
VERSION_GRAPHICS_BINS := $(call regional-graphics,$(VERSION))
endif

# Regional text includes must also trigger rebuilds when edited.
$(BUILD)/src/strings.o $(BUILD)/src/strings_sections.o: \
    src/data/strings_regions.inc src/data/strings_japanese.inc

# Compiler per file. Whole-file objects not listed use ../config.mk's agbcc
# defaults; the old_agbcc ones match the original's older compiler.
$(BUILD)/src/berry_fix.o \
$(BUILD)/src/m4a.o \
$(BUILD)/src/m4a_tables.o \
$(BUILD)/src/strings.o \
$(BUILD)/src/gflib/bg.o \
$(BUILD)/src/gflib/decompress.o \
$(BUILD)/src/gflib/init.o \
$(BUILD)/src/gflib/keys.o \
$(BUILD)/src/gflib/sound.o \
$(BUILD)/src/gflib/sprite.o \
$(BUILD)/src/gflib/text.o \
$(BUILD)/src/libpmagb/save_block_pointers.o \
$(BUILD)/src/libpmagb/berry.o \
$(BUILD)/src/libpmagb/pic_uncomp.o \
$(BUILD)/src/libpmagb/save.o \
$(GRAPHICS_OBJS): CC1 := $(OLD_CC1)

# The outer program's main, built as colosseum-mb builds it.
$(BUILD)/outer/main.o: CC1 := $(OLD_CC1)
$(BUILD)/outer/main.o: CFLAGS := -O2 -mthumb-interwork -fhex-asm -Wimplicit -Werror

# Berry Fix was compiled without an optimization flag.
$(BUILD)/src/berry_fix.o: CFLAGS := -mthumb-interwork -fhex-asm -Wimplicit -Werror

# The SDK's flash and RTC libraries, with their own compiler flags.
$(BUILD)/src/siirtc.o: CFLAGS := -mthumb-interwork -fhex-asm -Wimplicit -Werror
$(BUILD)/src/agb_flash.o \
$(BUILD)/src/agb_flash_1m.o \
$(BUILD)/src/agb_flash_le.o \
$(BUILD)/src/agb_flash_mx.o: CFLAGS := -O -mthumb-interwork -g

# libpmagb has its own optimization level (save.c is the exception).
$(BUILD)/src/libpmagb/save_block_pointers.o \
$(BUILD)/src/libpmagb/berry.o \
$(BUILD)/src/libpmagb/pic_uncomp.o: CFLAGS := -O3 -mthumb-interwork -g -fhex-asm -Wimplicit -Werror

# The split files: old_agbcc, except the RTC library's.
$(foreach f,$(FUNCTION_SPLITS) $(DATA_SPLITS) $(filter-out src/siirtc,$(JP_FUNCTION_SPLITS)),$(BUILD)/$(f)_sections.o): CC1 := $(OLD_CC1)
$(BUILD)/src/libpmagb/save_block_pointers_sections.o \
$(BUILD)/src/libpmagb/berry_sections.o \
$(BUILD)/src/libpmagb/pic_uncomp_sections.o: CFLAGS := -O3 -mthumb-interwork -g -fhex-asm -Wimplicit -Werror
$(BUILD)/src/siirtc_sections.o: CFLAGS := -mthumb-interwork -fhex-asm -Wimplicit -Werror
# payload_raw.s places the save-block pointers' bss.
$(BUILD)/src/libpmagb/save_block_pointers.o $(BUILD)/src/libpmagb/save_block_pointers_sections.o: CPPFLAGS += -DSDK_EXTERN_BSS

$(BUILD)/src/gfx_title_gfx.o: GRAPHICS_PART := 1
$(BUILD)/src/gfx_title_pal.o: GRAPHICS_PART := 2
$(BUILD)/src/gfx_title_tilemap.o: GRAPHICS_PART := 3
$(BUILD)/src/gfx_title_extra_pal.o: GRAPHICS_PART := 4
$(BUILD)/src/gfx_mon_info_icons.o: GRAPHICS_PART := 5
$(BUILD)/src/gfx_party_menu.o: GRAPHICS_PART := 6
$(BUILD)/src/gfx_message_box.o: GRAPHICS_PART := 7
$(BUILD)/src/gfx_text_pal.o: GRAPHICS_PART := 8
$(BUILD)/src/gfx_party_menu_tilemap.o: GRAPHICS_PART := 9
$(BUILD)/src/gfx_party_slots_tilemap.o: GRAPHICS_PART := 10
$(BUILD)/src/gfx_summary_screen.o: GRAPHICS_PART := 11
$(BUILD)/src/gfx_summary_icons.o: GRAPHICS_PART := 12
$(BUILD)/src/gfx_summary_pal.o: GRAPHICS_PART := 13
$(BUILD)/src/gfx_summary_skills_tilemap.o: GRAPHICS_PART := 14
$(BUILD)/src/gfx_summary_moves_tilemap.o: GRAPHICS_PART := 15
$(BUILD)/src/gfx_type_status_icons.o: GRAPHICS_PART := 16
$(BUILD)/src/gfx_type_status_pal.o: GRAPHICS_PART := 17
$(BUILD)/src/gfx_battle_screen.o: GRAPHICS_PART := 18
$(BUILD)/src/gfx_battle_pal.o: GRAPHICS_PART := 19
$(BUILD)/src/gfx_battle_tilemap.o: GRAPHICS_PART := 20
$(BUILD)/src/gfx_battle_windows_tilemap.o: GRAPHICS_PART := 21

$(GRAPHICS_OBJS): $(BUILD)/src/%.o: src/graphics.c $(HEADERS) $(VERSION_GRAPHICS_BINS)
	@mkdir -p $(dir $@)
	$(CPP) $(CPPFLAGS) -DGRAPHICS_PART=$(GRAPHICS_PART) $< | $(PREPROC) -i $< $(CHARMAP) > $(BUILD)/src/$*.i
	$(CC1) $(CFLAGS) -o $(BUILD)/src/$*.s $(BUILD)/src/$*.i
	printf '\t.text\n\t.align 2, 0\n' >> $(BUILD)/src/$*.s
	$(AS) $(ASFLAGS) -o $@ $(BUILD)/src/$*.s

# $(1): the file, without .c; $(2): -ffunction-sections or -fdata-sections;
# $(3): the extractor.
define split-rules
$(BUILD)/$(1)_sections.o: $(call source-of,$(1),c) $(HEADERS)
	@mkdir -p $$(dir $$@)
	$$(CPP) $$(CPPFLAGS) $$< | $$(PREPROC) -i $$< $$(CHARMAP) > $(BUILD)/$(1)_sections.i
	$$(CC1) $$(CFLAGS) $(2) -o $(BUILD)/$(1)_sections.s $(BUILD)/$(1)_sections.i
	$$(AS) $$(ASFLAGS) -o $$@ $(BUILD)/$(1)_sections.s

$(addprefix $(BUILD)/,$(call split-objs,$(1))): $(BUILD)/$(1)_%.o: $(BUILD)/$(1)_sections.o ../tools/$(3)
	python3 ../tools/$(3) $$< $$* $$@
endef
$(foreach f,$(FUNCTION_SPLITS) $(JP_FUNCTION_SPLITS),$(eval $(call split-rules,$(f),-ffunction-sections,extract_function_object.py)))
$(foreach f,$(DATA_SPLITS),$(eval $(call split-rules,$(f),-fdata-sections,extract_data_object.py)))
# Newlib's Thumb memcpy returns directly with pop {pc}, without interworking.
$(BUILD)/src/libpmagb/libc.o: CC1 := $(OLD_CC1)
$(BUILD)/src/libpmagb/libc.o: CFLAGS := -O2 -fhex-asm -Wimplicit -Wparentheses
# The Spinda spots are INCBIN'd.
$(BUILD)/src/pokemon_tables_sections.o: $(wildcard graphics/spinda_spots/*.bin)

# The original image links selected objects from agbcc's compiler runtime.
# Offer each archive member independently so the layout matcher keeps only
# the routines actually present in a given regional payload.
$(BUILD)/libgcc/%.o:
	@mkdir -p $(BUILD)/libgcc
	cd $(BUILD)/libgcc && $(PREFIX)ar x $(abspath $(AGBCC_DIR)/lib/libgcc.a) $(@F)

# Keep each font resource independently matchable. Western builds have
# language-specific Latin glyphs, while the Japanese glyph sets are shared.
$(BUILD)/data/font0_latin.o: DATA_PART := 1
$(BUILD)/data/font1_latin.o: DATA_PART := 2
$(BUILD)/data/font0_japanese.o: DATA_PART := 3
$(BUILD)/data/font1_japanese.o: DATA_PART := 4
$(BUILD)/data/ui_data.o: DATA_PART := 5
$(BUILD)/data/font0_latin.o \
$(BUILD)/data/font1_latin.o \
$(BUILD)/data/font0_japanese.o \
$(BUILD)/data/font1_japanese.o \
$(BUILD)/data/ui_data.o: data/all.s $(FONT_BINS)
	@mkdir -p $(dir $@)
	$(CPP) $(CPPFLAGS) -DDATA_PART=$(DATA_PART) $< | $(PREPROC) -i $< $(CHARMAP) | $(AS) $(ASFLAGS) -o $@

endif

$(RSFONT): $(RSFONT_SRCS) $(wildcard ../tools/rsfont/*.h)
	$(MAKE) -C ../tools/rsfont

graphics/fonts/font0_lat.2bpp: graphics/fonts/font0_lat.png $(RSFONT)
	$(RSFONT) $< $@ 254 1

graphics/fonts/font1_lat.2bpp: graphics/fonts/font1_lat.png $(RSFONT)
	$(RSFONT) $< $@ 110 2

graphics/fonts/font0_jpn.2bpp: graphics/fonts/font0_jpn.png $(RSFONT)
	$(RSFONT) $< $@ 254 1

graphics/fonts/font1_jpn.2bpp: graphics/fonts/font1_jpn.png $(RSFONT)
	$(RSFONT) $< $@ 110 2

graphics/fonts/font_null.2bpp: graphics/fonts/font_null.png $(RSFONT)
	$(RSFONT) $< $@ 254 1

graphics/fonts/font0_%.2bpp: graphics/fonts/font0_%.png $(RSFONT)
	$(RSFONT) $< $@ 254 1

graphics/fonts/font1_%.2bpp: graphics/fonts/font1_%.png $(RSFONT)
	$(RSFONT) $< $@ 110 2

%.8bpp: %.png
	$(GBAGFX) $< $@

%.lz: %
	$(GBAGFX) $< $@

%.rl: %
	$(GBAGFX) $< $@
