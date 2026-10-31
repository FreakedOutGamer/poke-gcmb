# sample0519.bin, a GBA multiboot program on the Pokémon Colosseum Bonus Disc
# (inside files/tgc/pokedownload.tgc). Like client/, it is an outer loader
# plus an LZ77-compressed payload; the build lives in ../multiboot.mk.

VERSIONS := sample0519
# The loader decompresses the payload here before running it.
PAYLOAD_BASE := 0x02018000
# gbagfx -search 1 reproduces the original LZ77 stream exactly.
GBAGFX_FLAGS := -search 1
SJIS_SRCS := src/delivery_data.c

include ../multiboot.mk

ifneq ($(VERSION),)
$(BUILD)/src/delivery_data.o: graphics/delivery_bg.gbapal graphics/jirachi/palette.gbapal graphics/jirachi/front_pic.4bpp
$(BUILD)/src/graphics.o: graphics/font/index.bin graphics/font/glyphs.bin graphics/frame/tiles.4bpp graphics/frame/tilemap.bin
# The SDK flash driver was built with the compiler's -O setting.
$(BUILD)/src/agb_flash_v102.o: CFLAGS := -O1 -mthumb-interwork
$(BUILD)/src/agb_flash_1m_v102.o: CFLAGS := -O1 -mthumb-interwork
$(BUILD)/src/agb_flash_mx_v102.o: CFLAGS := -O1 -mthumb-interwork
# siirtc was built with no -O at all.
$(BUILD)/src/siirtc.o: CFLAGS := -mthumb-interwork
# payload_raw.s places the RTC libraries' bss at its original addresses.
$(BUILD)/src/siirtc.o $(BUILD)/src/rtc.o: CPPFLAGS += -DSDK_EXTERN_BSS
$(BUILD)/src/calculate_box_mon_checksum.o: CFLAGS := -O2 -mthumb-interwork
$(BUILD)/src/get_substruct.o: CFLAGS := -O2 -mthumb-interwork
# The bundled newlib routine was built by the older compiler without
# interworking.
$(BUILD)/src/libpmagb/strcmp.o: CC1 := $(OLD_CC1)
$(BUILD)/src/libpmagb/strcmp.o: CFLAGS := -O2
endif
