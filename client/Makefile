# Channel Jirachi client multiboot image, both known revisions:
#
#   client_2003_1112  client.2003_1112.bin  REVISION=0
#   client            client.bin            REVISION=1
#
# Everything under src/, include/ and asm/ is shared; C code branches on
# REVISION where the revisions differ. The build itself lives in
# ../multiboot.mk (see there for targets); the per-version files are in
# ver/<version>/.

VERSIONS := client_2003_1112 client
# The loader decompresses the payload here before running it.
PAYLOAD_BASE := 0x02010000
# gbagfx -search 1 reproduces both original LZ77 streams exactly.
GBAGFX_FLAGS := -search 1

include ../multiboot.mk

# Graphics each data file INCBINs, converted from graphics/ first.
BG_GFX := $(wildcard graphics/background/*.bin) graphics/background/palettes.gbapal graphics/background/tiles.4bpp
SPRITE_GFX := $(patsubst %.png,%.4bpp,$(wildcard graphics/jirachi/*.png graphics/save_icon/*.png)) \
              $(patsubst %.pal,%.gbapal,$(wildcard graphics/jirachi/*.pal graphics/save_icon/*.pal))

ifneq ($(VERSION),)
$(BUILD)/src/bg_graphics.o: $(BG_GFX)
$(BUILD)/src/sprite_graphics.o: $(SPRITE_GFX)
# The SDK flash driver was built with the compiler's -O setting.
$(BUILD)/src/agb_flash_v102.o: CFLAGS := -O1 -mthumb-interwork
$(BUILD)/src/agb_flash_1m_v102.o: CFLAGS := -O1 -mthumb-interwork
$(BUILD)/src/agb_flash_mx_v102.o: CFLAGS := -O1 -mthumb-interwork
# siirtc was built with no -O at all, as in pokechan.
$(BUILD)/src/siirtc.o: CFLAGS := -mthumb-interwork
# payload_raw.s places the RTC libraries' bss at its original addresses.
$(BUILD)/src/siirtc.o $(BUILD)/src/rtc.o $(BUILD)/src/berry_fix_rtc.o: CPPFLAGS += -DSDK_EXTERN_BSS
$(BUILD)/src/create_box_mon.o: CC1 := $(OLD_CC1)
$(BUILD)/src/create_box_mon.o: CFLAGS := -O2 -mthumb-interwork
$(BUILD)/src/calculate_box_mon_checksum.o: CFLAGS := -O2 -mthumb-interwork
$(BUILD)/src/level.o: CFLAGS := -O2 -mthumb-interwork
$(BUILD)/src/moves.o: CFLAGS := -O2 -mthumb-interwork
# The bundled newlib routine was built by the older compiler without
# interworking, matching pokechan's shared libc build rule.
$(BUILD)/src/libpmagb/strcmp.o: CC1 := $(OLD_CC1)
$(BUILD)/src/libpmagb/strcmp.o: CFLAGS := -O2
endif
