# Shared build for multi-version multiboot projects whose image is an outer
# loader plus an LZ77-compressed payload (client/, colbtl/, sample0519/).
#
# The including Makefile sets, before including this file:
#   VERSIONS      every version this project builds
#   PAYLOAD_BASE  address the payload is linked at
#   GBAGFX_FLAGS  gbagfx options that reproduce the original LZ77 streams
#   SJIS_SRCS     (optional) C files whose plain string literals are written
#                 in UTF-8 but built as Shift-JIS (the _() strings are not
#                 affected; preproc has already turned them into bytes)
# and may add per-object compiler overrides afterwards, inside
# `ifneq ($(VERSION),)`, using $(BUILD).
#
# Each ver/<version>/ holds rom.sha1, ld_script.ld (outer image) and
# version.mk, which sets VERSION_DEFINES, PAYLOAD_SIZE and (generated)
# PAYLOAD_OBJS. A project whose outer image has its own program (a loader
# before the payload) lists its objects in OUTER_OBJS, built from outer/,
# and places them in ver/<version>/ld_script.ld. payload.ld and
# payload_raw.s are generated files.
#
#   make                          build and compare every version
#   make <version>                build and compare one version
#   make VERSION=<version> <target>
#   make clean

include ../config.mk

# A failing command in a pipeline (the C rule's cpp | preproc) fails the rule.
SHELL := /bin/bash
.SHELLFLAGS := -o pipefail -c

# GBAGFX and PREPROC (through ../common/charmap.txt) come from ../config.mk.
CHARMAP := ../common/charmap.txt

# Graphics are kept as PNG and JASC palettes and converted at build time.
%.4bpp: %.png
	$(GBAGFX) $< $@
%.gbapal: %.pal
	$(GBAGFX) $< $@

ifeq ($(VERSION),)

.PHONY: all compare clean $(VERSIONS)

all compare: $(VERSIONS)

$(VERSIONS):
	@$(MAKE) --no-print-directory VERSION=$@ compare

clean:
	rm -rf build $(VERSIONS:%=%.mb)

else

ifeq ($(filter $(VERSION),$(VERSIONS)),)
$(error unknown VERSION '$(VERSION)'; expected one of: $(VERSIONS))
endif

VER_DIR := ver/$(VERSION)
BUILD := build/$(VERSION)
ROM := $(VERSION).mb

PREFIX := arm-none-eabi-
AS := $(PREFIX)as
LD := $(PREFIX)ld
OBJCOPY := $(PREFIX)objcopy

include $(VER_DIR)/version.mk

# VERSION_DEFINES are -DNAME=value flags; .s files get the same values
# through --defsym (use .if NAME there).
CPPFLAGS += $(VERSION_DEFINES)
ASFLAGS := -mcpu=arm7tdmi -I . -I ../common $(patsubst -D%,--defsym %,$(VERSION_DEFINES))
HEADERS := $(shell find include ../common/include src -name '*.h' -type f 2>/dev/null)

PAYLOAD_OBJS := $(addprefix $(BUILD)/,$(PAYLOAD_OBJS))
OUTER_OBJS := $(addprefix $(BUILD)/,$(OUTER_OBJS))

.PHONY: all compare
.SECONDARY:

all: $(ROM)

compare: $(ROM)
	@sha1sum -c $(VER_DIR)/rom.sha1

# A source missing from the project's src/ is taken from ../common/src/ (the
# same path), so objects shared by several projects keep the project's object
# path, compiler flags and link order.
define c-rule
$(BUILD)/src/%.o: $(1)src/%.c $(HEADERS)
	@mkdir -p $$(dir $$@)
	$$(CPP) $$(CPPFLAGS) $$< | $$(PREPROC) -i $$< $$(CHARMAP) | $$(if $$(filter $$<,$$(SJIS_SRCS)),iconv -f UTF-8 -t SHIFT_JIS,cat) > $$(BUILD)/src/$$*.i
	$$(CC1) $$(CFLAGS) -o $$(BUILD)/src/$$*.s $$(BUILD)/src/$$*.i
	@# As pret does: pad .text with zeros, not the assembler's nops.
	printf '\t.text\n\t.align 2, 0\n' >> $$(BUILD)/src/$$*.s
	$$(AS) $$(ASFLAGS) -o $$@ $$(BUILD)/src/$$*.s
endef
$(eval $(call c-rule,))
$(eval $(call c-rule,../common/))

# The project's copy of a file (path without extension), else ../common's.
source-of = $(firstword $(wildcard $(1).$(2)) ../common/$(1).$(2))

$(BUILD)/asm/%.o: asm/%.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

# The outer image's own program, if any (OUTER_OBJS).
$(BUILD)/outer/%.o: outer/%.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

$(BUILD)/outer/%.o: outer/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(CPP) $(CPPFLAGS) $< | $(PREPROC) -i $< $(CHARMAP) > $(BUILD)/outer/$*.i
	$(CC1) $(CFLAGS) -o $(BUILD)/outer/$*.s $(BUILD)/outer/$*.i
	printf '\t.text\n\t.align 2, 0\n' >> $(BUILD)/outer/$*.s
	$(AS) $(ASFLAGS) -o $@ $(BUILD)/outer/$*.s

# Payload data kept in assembly.
$(BUILD)/data/%.o: data/%.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

# The version's still-raw payload ranges, cut from its extracted payload.
$(BUILD)/payload_raw.o: $(VER_DIR)/payload_raw.s $(wildcard $(VER_DIR)/payload.bin)
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -I $(VER_DIR) -o $@ $<

# Prebuilt compiler-runtime objects, linked straight from agbcc's libgcc.a.
$(BUILD)/libgcc/%.o:
	@mkdir -p $(BUILD)/libgcc
	cd $(BUILD)/libgcc && $(PREFIX)ar x $(abspath $(AGBCC_DIR)/lib/libgcc.a) $(@F)

# Prebuilt newlib objects, linked straight from agbcc's libc.a.
$(BUILD)/libc/%.o:
	@mkdir -p $(BUILD)/libc
	cd $(BUILD)/libc && $(PREFIX)ar x $(abspath $(AGBCC_DIR)/lib/libc.a) $(@F)

# The decompressed payload, relinked from the sources and recompressed.
$(BUILD)/payload.elf: $(PAYLOAD_OBJS) $(VER_DIR)/payload.ld
	$(LD) -T $(VER_DIR)/payload.ld -Map $(BUILD)/payload.map -o $@ $(PAYLOAD_OBJS)

$(BUILD)/payload.bin: $(BUILD)/payload.elf
	$(OBJCOPY) -O binary $< $@

$(BUILD)/payload.lz: $(BUILD)/payload.bin
	$(GBAGFX) $< $@ $(GBAGFX_FLAGS)

# The outer image: header, any outer program (OUTER_OBJS) and the rebuilt
# payload.lz (data/raw.s).
$(BUILD)/data/raw.o: $(call source-of,data/raw,s) $(BUILD)/payload.lz
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -I $(VER_DIR) -I $(BUILD) -o $@ $<

$(BUILD)/$(VERSION).elf: $(BUILD)/asm/rom_header.o $(BUILD)/data/raw.o $(OUTER_OBJS) $(VER_DIR)/ld_script.ld
	$(LD) -T $(VER_DIR)/ld_script.ld -Map $(BUILD)/$(VERSION).map -o $@ $(BUILD)/asm/rom_header.o $(BUILD)/data/raw.o $(OUTER_OBJS)

$(ROM): $(BUILD)/$(VERSION).elf
	$(OBJCOPY) -O binary $< $@

endif
