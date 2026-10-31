# pokechan: the Pokemon Channel Jirachi distributor, a GBA multiboot image
# shipped on the Pokemon Channel GameCube disc.
#
#   make            build pokechan.mb
#   make compare    build and check it against rom.sha1
#   make syms       write the symbol table to build/pokechan.sym
#   make clean
#
# Unlike client/ and colbtl/ (../multiboot.mk), this is a single image with
# no compressed payload, linked directly by ld_script.ld and ld_text.ld.

TARGET := pokechan
BUILD := build

include ../config.mk

PREFIX := arm-none-eabi-
AS := $(PREFIX)as
LD := $(PREFIX)ld
OBJCOPY := $(PREFIX)objcopy
OBJDUMP := $(PREFIX)objdump
CHARMAP := ../common/charmap.txt

# A failing command in a pipeline (the C rule's cpp | preproc) fails the rule.
SHELL := /bin/bash
.SHELLFLAGS := -o pipefail -c

ASFLAGS := -mcpu=arm7tdmi -I .
LDFLAGS := -T ld_script.ld -L . -Map $(BUILD)/$(TARGET).map

HEADERS := $(shell find include ../common/include -type f -name '*.h')
ASM_SRCS := $(wildcard asm/*.s) $(wildcard data/*.s)
# The program's own sources, in pret's layout (src/, src/gflib/, src/libpmagb/).
C_SRCS := $(sort $(shell find src -name '*.c'))
# Sources shared with other projects live in ../common at the same path;
# their objects build as if they were the project's own.
COMMON_SRCS := \
	src/gflib/bg.c \
	src/gflib/decompress.c \
	src/gflib/intr.c \
	src/gflib/keys.c \
	src/gflib/sprite.c \
	src/agb_flash.c \
	src/agb_flash_1m.c \
	src/agb_flash_le.c \
	src/agb_flash_mx.c \
	src/berry_fix_rtc.c \
	src/rtc.c \
	src/siirtc.c \
	src/joybus_recv_mon.c \
	src/libpmagb/flag.c \
	src/player_record.c \
	src/libpmagb/item_id_remap.c \
	src/libpmagb/libc.c \
	src/libpmagb/map_check.c \
	src/libpmagb/save_chunks.c \
	src/libpmagb/string_util.c \
	src/libpmagb/save_init.c \
	src/libpmagb/save_load.c \
	src/libpmagb/save_read_all.c \
	src/libpmagb/save_reset.c \
	src/libpmagb/save_retry.c \
	src/libpmagb/save_write_sector_or_slot.c \
	src/ball_gfx.c \
	src/get_mon_ability.c \
	src/get_mon_gender.c \
	src/get_party_mon_pal.c \
	src/get_party_slot.c \
	src/give_ribbon.c \
	src/mon_data_lookups.c \
	src/mon_gender.c \
	src/mon_move_pp.c \
	src/move_name.c \
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
	src/libpmagb/item.c \
	src/libpmagb/pokedex.c
ASM_SRCS += $(filter %.s,$(COMMON_SRCS))
C_SRCS += $(filter %.c,$(COMMON_SRCS))
# Prebuilt agbcc libgcc objects, placed by linked_objs.txt.
LIBGCC_OBJS := $(shell sed -n 's/^0x[0-9A-Fa-f]* 0x[0-9A-Fa-f]* //p' linked_objs.txt)
# Functions from shared files that also hold code pokechan does not have:
# the file is compiled with -ffunction-sections and those functions are
# extracted into their own objects, as coupon and colbtl do.
SPLITS := src/libpmagb/save src/libpmagb/save_block_pointers
src/libpmagb/save_FUNCS := RestoreSaveBackupVarsAndIncrement RestoreSaveBackupVars \
	RevertSaveCounters HandleWriteIncrementalSector HandleReplaceSectorAndVerify
src/libpmagb/save_block_pointers_FUNCS := SaveBlocksInit SaveRandom UpdateSaveBlockPtr \
	GetIwramWorkAreaAddress
SPLIT_OBJS := $(foreach f,$(SPLITS),$(addprefix $(BUILD)/$(f)_,$(addsuffix .o,$($(f)_FUNCS))))
OBJS := $(ASM_SRCS:%.s=$(BUILD)/%.o) $(C_SRCS:%.c=$(BUILD)/%.o) $(SPLIT_OBJS) $(LIBGCC_OBJS)
SPLASH_INPUTS := graphics/splash/splash.png graphics/splash/bit15.bin graphics/splash/footer.bin

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
	src/player_record.o \
	src/receive_jirachi.o \
	src/scene.o \
	src/startup.o \
	src/gflib/bg.o \
	src/gflib/decompress.o \
	src/gflib/intr.o \
	src/gflib/keys.o \
	src/gflib/sprite.o \
	src/libpmagb/enigma_berry.o \
	src/libpmagb/flag.o \
	src/libpmagb/item.o \
	src/libpmagb/item_id_remap.o \
	src/libpmagb/map_check.o \
	src/libpmagb/pokedex.o \
	src/libpmagb/save_chunk.o \
	src/libpmagb/save_chunks.o \
	src/libpmagb/save_link.o \
	src/libpmagb/save_failed_screen.o \
	src/libpmagb/save_init.o \
	src/libpmagb/save_load.o \
	src/libpmagb/save_read_all.o \
	src/libpmagb/save_reset.o \
	src/libpmagb/save_retry.o \
	src/libpmagb/save_write.o \
	src/libpmagb/save_write_sector_or_slot.o \
	src/libpmagb/string_util.o \
	src/libpmagb/target_game.o \
	src/joybus_cmd.o \
	src/joybus_init.o \
	src/joybus_transmit.o \
	src/joybus_recv_jirachi.o \
	src/joybus_recv_mon.o \
	src/joybus_status.o \
	src/joybus_wait.o \
	src/main.o \
	src/ball_gfx.o \
	src/get_mon_ability.o \
	src/get_mon_gender.o \
	src/get_party_mon_pal.o \
	src/get_party_slot.o \
	src/mon_data_lookups.o \
	src/mon_gender.o \
	src/mon_move_pp.o \
	src/mon_sprite_icon.o \
	src/move_name.o \
	src/pokemon_1.o \
	src/pokemon_4.o \
	src/pokemon_5.o \
	src/pokemon_pokerus.o \
	src/set_mon_data.o \
	src/species_name.o \
	src/spinda.o

# agbcc -O2 without config.mk's extra flags.
AGBCC_O2_SRCS := \
	src/give_ribbon.o \
	src/pokemon_2.o \
	src/pokemon_3.o \
	src/pokemon_icon.o

# The SDK flash driver was built with -O1.
AGBCC_O1_SRCS := \
	src/agb_flash.o \
	src/agb_flash_1m.o \
	src/agb_flash_le.o \
	src/agb_flash_mx.o

# siirtc/rtc and pret/berry-fix's payload were built with no -O at all.
AGBCC_O0_SRCS := \
	src/berry_fix_event_data.o \
	src/berry_fix.o \
	src/berry_fix_rtc.o \
	src/rtc.o \
	src/siirtc.o

$(addprefix $(BUILD)/, $(OLD_AGBCC_SRCS)): CC1 := $(OLD_CC1)
$(addprefix $(BUILD)/, $(OLD_AGBCC_SRCS)): CFLAGS := -O2 -mthumb-interwork
# The shared gflib's bss is placed by ld_script.ld.
$(BUILD)/src/gflib/bg.o $(BUILD)/src/gflib/sprite.o $(BUILD)/src/gflib/keys.o: CPPFLAGS += -DSDK_EXTERN_BSS
$(addprefix $(BUILD)/, $(AGBCC_O2_SRCS)): CFLAGS := -O2 -mthumb-interwork
$(addprefix $(BUILD)/, $(AGBCC_O1_SRCS)): CFLAGS := -O1 -mthumb-interwork
$(addprefix $(BUILD)/, $(AGBCC_O0_SRCS)): CFLAGS := -mthumb-interwork
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
# Graphics. Tiles and palettes are kept as PNG and JASC palettes, as in
# client/, and converted at build time.

%.4bpp: %.png
	$(GBAGFX) $< $@
%.gbapal: %.pal
	$(GBAGFX) $< $@
%.lz: %
	$(GBAGFX) $< $@

# The splash screen is a mode-3 (direct color) bitmap: graphics/splash/
# splash.png holds its colors, bit15.bin the pixels' undisplayed bit 15
# (which the original sets on about 40% of them), and footer.bin four bytes
# the compressed stream carries past the image.
$(BUILD)/graphics/splash.bin: $(SPLASH_INPUTS) ../tools/render_mode3.py
	@mkdir -p $(BUILD)/graphics
	python3 ../tools/render_mode3.py to-bin $(SPLASH_INPUTS) $@

$(BUILD)/src/graphics.o: $(BUILD)/graphics/splash.bin.lz \
                              graphics/receive_scene/frame.4bpp.lz \
                              graphics/receive_scene/frame.gbapal \
                              graphics/receive_scene/picture.gbapal
