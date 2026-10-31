# coupon: Pokemon Colosseum's Poke Coupon link program, a GBA multiboot
# image shipped on the Pokemon Colosseum GameCube discs.
#
#   make            build coupon.mb
#   make compare    build and check it against rom.sha1
#   make syms       write the symbol table to build/coupon.sym
#   make clean
#
# Like pokechan/, this is a single image with no compressed payload, linked
# directly by ld_script.ld and ld_text.ld.

TARGET := coupon
BUILD := build
CHARMAP := ../common/charmap.txt

include ../config.mk

PREFIX := arm-none-eabi-
AS := $(PREFIX)as
LD := $(PREFIX)ld
OBJCOPY := $(PREFIX)objcopy
OBJDUMP := $(PREFIX)objdump

ASFLAGS := -mcpu=arm7tdmi -I .
LDFLAGS := -T ld_script.ld -Map $(BUILD)/$(TARGET).map

HEADERS := $(shell find include ../common/include -type f -name '*.h')
ASM_SRCS := $(sort $(shell find asm data -name '*.s'))
# Source folders group code by purpose, as in pokechan/.
C_SRCS := $(sort $(shell find src -name '*.c'))
# Sources shared with other projects live in ../common at the same path;
# their objects build as if they were the project's own.
COMMON_SRCS := \
	src/gflib/bg.c \
	src/gflib/decompress.c \
	src/gflib/intr.c \
	src/gflib/keys.c \
	src/gflib/sprite.c \
	src/gflib/text.c \
	src/agb_flash.c \
	src/agb_flash_1m.c \
	src/agb_flash_le.c \
	src/agb_flash_mx.c \
	src/joybus_recv_mon.c \
	src/libpmagb/flag.c \
	src/player_record.c \
	src/libpmagb/item_id_remap.c \
	src/libpmagb/libc.c \
	src/libpmagb/pokedex.c \
	src/libpmagb/map_check.c \
	src/libpmagb/save_chunks.c \
	src/libpmagb/string_util.c \
	src/libpmagb/save_init.c \
	src/libpmagb/save_load.c \
	src/libpmagb/save_read_all.c \
	src/libpmagb/save_retry.c \
	src/libpmagb/save_write_sector_or_slot.c \
	src/ball_gfx.c \
	src/get_mon_ability.c \
	src/get_mon_gender.c \
	src/get_party_mon_pal.c \
	src/get_party_slot.c \
	src/mon_data_lookups.c \
	src/mon_gender.c \
	src/mon_move_pp.c \
	src/move_name.c \
	src/give_ribbon.c \
	src/pokemon_1.c \
	src/pokemon_2.c \
	src/pokemon_3.c \
	src/pokemon_4.c \
	src/pokemon_5.c \
	src/pokemon_pokerus.c \
	src/set_mon_data.c \
	src/species_name.c \
	src/pokemon_icon.c \
	src/mon_sprite_icon.c \
	src/libpmagb/enigma_berry.c \
	src/libpmagb/item.c
ASM_SRCS += $(filter %.s,$(COMMON_SRCS))
C_SRCS += $(filter %.c,$(COMMON_SRCS))
# Prebuilt agbcc libgcc objects, placed by linked_objs.txt.
LIBGCC_OBJS := $(shell sed -n 's/^0x[0-9A-Fa-f]* 0x[0-9A-Fa-f]* //p' linked_objs.txt)
# Shared files coupon has only some functions of: each is compiled with
# -ffunction-sections and those functions extracted into their own objects,
# as colbtl's own build does.
SPLITS := src/joybus_recv src/libpmagb/save_block_pointers src/libpmagb/save src/libpmagb/save_reset src/gflib/init src/render_text src/fade src/window src/link_message \
	src/link_transfer src/battle_menu src/party_menu
src/joybus_recv_FUNCS := ReceiveGiftData ProcessReceiveCommand
src/libpmagb/save_block_pointers_FUNCS := SaveBlocksInit SaveRandom UpdateSaveBlockPtr GetLastSaveBlockPtr
src/libpmagb/save_FUNCS := HandleWriteSector WriteSaveSlot HandleReplaceSector \
	WriteSectorSignatureByte WriteSectorSignatureByteOrRevert FullSaveAndLoopOverFailedSectors WriteLinkSave WipeSector \
	GetSaveReadBufferPointer RestoreSaveBackupVarsAndIncrement RestoreSaveBackupVars \
	RevertSaveCounters HandleWriteIncrementalSector HandleReplaceSectorAndVerify
src/libpmagb/save_reset_FUNCS := Save_ResetSaveCounters ClearSaveData
src/gflib/init_FUNCS := AgbMain DelayFrames InitIntr SetIntrFunc SetVBlankCallback
src/render_text_FUNCS := BufferString RenderText RenderText_NoPlaceholders GetStringWidth \
	NumToPmString3CustomZeroChar NumToPmString3RightAlign RenderTextAt
src/fade_FUNCS := IsScreenFadedOut FadeIn FadeOut
src/battle_menu_FUNCS := GetMoveRestriction IsMoveSelectionLocked GetBattleMenuRestriction \
	GetForcedMoveSlot IsMonFainted
src/party_menu_FUNCS := DrawPartyMonHealthBar
src/window_FUNCS := DrawTextWindowBorder ClearVram
src/link_message_FUNCS := PrintLinkCommandMessage RunLinkStandbyLoop InitLinkMessageScreen WarningPrint ErrorPrint
src/link_transfer_FUNCS := FinalizeCouponTransfer BufferFirstPartyMon FinalizeLinkSave FinalizePokemonTransfer GetSwitchOutRestriction \
	GetLinkSelectedMonId ShouldUseLinkPartyMenu GetNibbleByIndex IsPartySlotOccupied
SPLIT_OBJS := $(foreach f,$(SPLITS),$(addprefix $(BUILD)/$(f)_,$(addsuffix .o,$($(f)_FUNCS))))
OBJS := $(ASM_SRCS:%.s=$(BUILD)/%.o) $(C_SRCS:%.c=$(BUILD)/%.o) $(SPLIT_OBJS) $(LIBGCC_OBJS)

.PHONY: all compare syms clean
.SECONDARY:

all: $(TARGET).mb

# $(TARGET).mb is a prerequisite, so a failed build stops here rather than
# comparing against whatever the last successful link left behind.
compare: $(TARGET).mb
	@sha1sum -c rom.sha1

clean:
	rm -rf $(BUILD) $(TARGET).mb

syms: $(BUILD)/$(TARGET).elf
	$(OBJDUMP) -t $< | sort > $(BUILD)/$(TARGET).sym

# ---------------------------------------------------------------------------
# Compiler selection. Objects not listed use ../config.mk's agbcc defaults.

# old_agbcc -O2: most of the program.
OLD_AGBCC_SRCS := \
	src/main.o \
	src/bad_egg.o \
	src/gflib/bg.o \
	src/gflib/decompress.o \
	src/gflib/intr.o \
	src/gflib/keys.o \
	src/gflib/sound.o \
	src/gflib/sprite.o \
	src/gflib/text.o \
	src/libpmagb/save_chunks.o \
	src/libpmagb/save_init.o \
	src/libpmagb/save_load.o \
	src/libpmagb/save_read_all.o \
	src/libpmagb/save_retry.o \
	src/libpmagb/save_write_sector_or_slot.o \
	src/player_record.o \
	src/receive_coupon_data.o \
	src/pokemon_tables.o \
	src/scene_data.o \
	src/libpmagb/enigma_berry.o \
	src/libpmagb/flag.o \
	src/libpmagb/item.o \
	src/libpmagb/item_id_remap.o \
	src/libpmagb/map_check.o \
	src/libpmagb/pokedex.o \
	src/libpmagb/string_util.o \
	src/libpmagb/target_game.o \
	src/joybus_cmd.o \
	src/joybus_transmit.o \
	src/joybus_recv_mon.o \
	src/ball_gfx.o \
	src/get_mon_ability.o \
	src/get_mon_gender.o \
	src/get_party_mon_pal.o \
	src/get_party_slot.o \
	src/mon_data_lookups.o \
	src/mon_gender.o \
	src/mon_move_pp.o \
	src/give_ribbon.o \
	src/mon_sprite_icon.o \
	src/move_name.o \
	src/pokemon_1.o \
	src/pokemon_3.o \
	src/pokemon_4.o \
	src/pokemon_5.o \
	src/pokemon_icon.o \
	src/pokemon_pokerus.o \
	src/set_mon_data.o \
	src/species_name.o

# agbcc -O2 without config.mk's extra flags.
AGBCC_O2_SRCS := \
	src/pokemon_2.o \
	src/spinda.o

# The SDK flash driver was built with -O1.
AGBCC_O1_SRCS := \
	src/agb_flash.o \
	src/agb_flash_1m.o \
	src/agb_flash_le.o \
	src/agb_flash_mx.o

$(addprefix $(BUILD)/, $(OLD_AGBCC_SRCS)): CC1 := $(OLD_CC1)
$(addprefix $(BUILD)/, $(OLD_AGBCC_SRCS)): CFLAGS := -O2 -mthumb-interwork
$(addprefix $(BUILD)/, $(AGBCC_O2_SRCS)): CFLAGS := -O2 -mthumb-interwork
$(addprefix $(BUILD)/, $(AGBCC_O1_SRCS)): CFLAGS := -O1 -mthumb-interwork
# ld_script.ld places the flash library's bss at its original addresses.
$(BUILD)/src/agb_flash.o: CPPFLAGS += -DSDK_EXTERN_BSS
$(BUILD)/src/gflib/bg.o: CPPFLAGS += -DSDK_EXTERN_BSS
$(BUILD)/src/gflib/text.o: CPPFLAGS += -DSDK_EXTERN_BSS
$(BUILD)/src/gflib/keys.o: CPPFLAGS += -DSDK_EXTERN_BSS
$(BUILD)/src/gflib/sprite.o: CPPFLAGS += -DSDK_EXTERN_BSS
# newlib was built by the older compiler with no interworking, the same way
# pokeemerald builds its libc.o.
$(BUILD)/src/libpmagb/libc.o: CC1 := $(OLD_CC1)
$(BUILD)/src/libpmagb/libc.o: CFLAGS := -O2

# ---------------------------------------------------------------------------
# Build rules.

$(BUILD)/%.o: %.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

# Every object depends on every header. Coarse, but the alternative is stale
# objects after a header edit, which shows up as a mismatch in code you did
# not touch.
$(BUILD)/%.o: %.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(CPP) $(CPPFLAGS) $< | $(PREPROC) -i $< $(CHARMAP) > $(BUILD)/$*.i
	$(CC1) $(CFLAGS) -o $(BUILD)/$*.s $(BUILD)/$*.i
	@# As pret does: pad .text with zeros, not the assembler's nops.
	@printf '\t.text\n\t.align 2, 0\n' >> $(BUILD)/$*.s
	$(AS) $(ASFLAGS) -o $@ $(BUILD)/$*.s

$(BUILD)/%.o: ../common/%.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

$(BUILD)/%.o: ../common/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(CPP) $(CPPFLAGS) $< | $(PREPROC) -i $< $(CHARMAP) > $(BUILD)/$*.i
	$(CC1) $(CFLAGS) -o $(BUILD)/$*.s $(BUILD)/$*.i
	@# As pret does: pad .text with zeros, not the assembler's nops.
	@printf '\t.text\n\t.align 2, 0\n' >> $(BUILD)/$*.s
	$(AS) $(ASFLAGS) -o $@ $(BUILD)/$*.s

# Prebuilt compiler-runtime objects, linked straight from agbcc's libgcc.a.
$(BUILD)/libgcc/%.o:
	@mkdir -p $(BUILD)/libgcc
	cd $(BUILD)/libgcc && $(PREFIX)ar x $(abspath $(AGBCC_DIR)/lib/libgcc.a) $(@F)

$(BUILD)/$(TARGET).elf: $(OBJS) ld_script.ld ld_text.ld
	@mkdir -p $(BUILD)
	@rm -f $@ $(TARGET).mb
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

$(TARGET).mb: $(BUILD)/$(TARGET).elf
	@rm -f $@
	$(OBJCOPY) -O binary $< $@

# ---------------------------------------------------------------------------
# Graphics. Tiles, fonts and palettes are kept as PNG and JASC palettes, as
# in pokechan/, and converted at build time; gbagfx recreates the original
# RL/LZ streams exactly.

graphics/main_screen/tiles.4bpp: graphics/main_screen/tiles.png
	$(GBAGFX) $< $@ -num_tiles 640 -Werror=num_tiles
graphics/overlay/tiles.4bpp: graphics/overlay/tiles.png
	$(GBAGFX) $< $@ -num_tiles 10 -Werror=num_tiles
graphics/fonts/japanese.1bpp: graphics/fonts/japanese.png
	$(GBAGFX) $< $@ -num_tiles 1016 -mwidth 1 -mheight 2
graphics/fonts/latin.1bpp: graphics/fonts/latin.png
	$(GBAGFX) $< $@ -num_tiles 504 -mwidth 1 -mheight 2
%.gbapal: %.pal
	$(GBAGFX) $< $@
%.rl: %
	$(GBAGFX) $< $@
%.lz: %
	$(GBAGFX) $< $@

DISPLAY_ASSETS := \
	graphics/main_screen/tiles.4bpp.rl \
	graphics/main_screen/palettes.gbapal.rl \
	graphics/main_screen/tilemap.bin.lz \
	graphics/overlay/palette.gbapal.rl \
	graphics/overlay/tiles.4bpp.lz \
	graphics/alternate_palettes.gbapal.lz

$(BUILD)/data/display_data.o: $(DISPLAY_ASSETS)
$(BUILD)/data/font_data.o: graphics/fonts/japanese.1bpp graphics/fonts/latin.1bpp
$(BUILD)/data/spinda_spots.o: $(wildcard graphics/spinda_spots/*.bin)

# The split objects: compile each shared file once, then extract a function.
$(BUILD)/%_sections.o: ../common/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(CPP) $(CPPFLAGS) $< | $(PREPROC) -i $< $(CHARMAP) > $(BUILD)/$*_sections.i
	$(OLD_CC1) -O2 -mthumb-interwork -ffunction-sections -o $(BUILD)/$*_sections.s $(BUILD)/$*_sections.i
	$(AS) $(ASFLAGS) -o $@ $(BUILD)/$*_sections.s

define split-rules
$(addprefix $(BUILD)/$(1)_,$(addsuffix .o,$($(1)_FUNCS))): $(BUILD)/$(1)_%.o: $(BUILD)/$(1)_sections.o ../tools/extract_function_object.py
	python3 ../tools/extract_function_object.py $$< $$* $$@
endef
$(foreach f,$(SPLITS),$(eval $(call split-rules,$(f))))
