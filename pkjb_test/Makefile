TARGET := pkjb_test
BUILD  := build

include ../config.mk

PREFIX  := arm-none-eabi-
AS      := $(PREFIX)as
LD      := $(PREFIX)ld
OBJCOPY := $(PREFIX)objcopy
OBJDUMP := $(PREFIX)objdump
CHARMAP := ../common/charmap.txt

# A failing command in a pipeline fails the rule.
SHELL := /bin/bash
.SHELLFLAGS := -o pipefail -c

ASFLAGS := -mcpu=arm7tdmi -I . -I ../common
LDFLAGS := -T ld_script.ld -Map $(BUILD)/$(TARGET).map

HEADERS := $(wildcard include/*.h)
ASM_SRCS := asm/rom_header.s asm/crt0.s asm/libagbsyscall.s
C_SRCS := $(sort $(shell find src -name '*.c'))
OBJS := $(ASM_SRCS:%.s=$(BUILD)/%.o) $(C_SRCS:%.c=$(BUILD)/%.o)

.PHONY: all compare clean png
.SECONDARY:

all: $(TARGET).mb

compare: $(TARGET).mb
	@sha1sum -c rom.sha1

$(BUILD)/%.o: %.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

# Ready for incremental C replacements. Every project uses the one CC1
# configured in ../config.mk rather than carrying its own compiler copy.
$(BUILD)/%.o: %.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(CPP) $(CPPFLAGS) $< | $(PREPROC) -i $< $(CHARMAP) > $(BUILD)/$*.i
	$(CC1) $(CFLAGS) -o $(BUILD)/$*.s $(BUILD)/$*.i
	@printf '\t.text\n\t.align 2, 0\n' >> $(BUILD)/$*.s
	$(AS) $(ASFLAGS) -o $@ $(BUILD)/$*.s

$(BUILD)/src/intr.o: CC1 := $(OLD_CC1)
$(BUILD)/src/main.o: CC1 := $(OLD_CC1)
$(BUILD)/src/test_screen.o: CC1 := $(OLD_CC1)
# -O3, for -finline-functions: LoadTestScreen is also inlined into its caller.
$(BUILD)/src/test_screen.o: CFLAGS := -O3 -mthumb-interwork

# Graphics are kept as PNG and JASC palettes and converted at build time.
%.4bpp: %.png
	$(GBAGFX) $< $@
%.gbapal: %.pal
	$(GBAGFX) $< $@

# The package the program decompresses: palette, tilemap, then tiles.
$(BUILD)/graphics/test_screen.bin: graphics/test_screen.gbapal \
                                     graphics/test_screen.bin \
                                     graphics/test_screen.4bpp
	@mkdir -p $(dir $@)
	cat $^ > $@

$(BUILD)/graphics/test_screen.lz: $(BUILD)/graphics/test_screen.bin
	$(GBAGFX) $< $@

$(BUILD)/graphics/test_screen_lz.bin: $(BUILD)/graphics/test_screen.lz
	cp $< $@

$(BUILD)/graphics/test_screen.padded.huff: $(BUILD)/graphics/test_screen_lz.bin
	$(GBAGFX) $< $@ -depth 4

# The compressor's meaningful bitstream is exact. The original image's last
# three alignment bytes came from whatever followed its bit writer, so retain
# those bytes explicitly (graphics/test_screen.huff_footer).
$(BUILD)/graphics/test_screen.huff: $(BUILD)/graphics/test_screen.padded.huff \
                                    graphics/test_screen.huff_footer
	head -c 5653 $< > $@
	cat graphics/test_screen.huff_footer >> $@

$(BUILD)/src/assets.o: $(BUILD)/graphics/test_screen.huff

$(BUILD)/graphics/test_screen.png: graphics/test_screen.gbapal \
                                    graphics/test_screen.bin \
                                    graphics/test_screen.4bpp \
                                    ../tools/render_bg.py
	python3 ../tools/render_bg.py graphics/test_screen.gbapal \
	  graphics/test_screen.bin graphics/test_screen.4bpp $@

png: $(BUILD)/graphics/test_screen.png

$(BUILD)/$(TARGET).elf: $(OBJS) ld_script.ld
	@mkdir -p $(BUILD)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

$(TARGET).mb: $(BUILD)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

clean:
	rm -rf $(BUILD) $(TARGET).mb
