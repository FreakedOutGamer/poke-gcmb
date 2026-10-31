# client.2003_1112.bin: the earlier revision.
VERSION_DEFINES := -DREVISION=0
PAYLOAD_SIZE := 0x97F0

# Every object linked into the payload; ver/client_2003_1112/payload.ld
# decides where each one goes.
PAYLOAD_OBJS := \
	payload_raw.o \
	asm/crt0.o \
	asm/libagbsyscall.o \
	asm/multi_sio_asm.o \
	data/flash.o \
	libgcc/_call_via_rX.o \
	libgcc/_divsi3.o \
	libgcc/_dvmd_tls.o \
	libgcc/_modsi3.o \
	libgcc/_udivsi3.o \
	libgcc/_umodsi3.o \
	src/bg_graphics.o \
	src/pokemon_tables.o \
	src/sprite_graphics.o \
	src/strings.o \
	src/variables.o \
	src/delivery.o \
	src/give_jirachi.o \
	src/jirachi.o \
	src/intr.o \
	src/gflib/reset.o \
	src/text.o \
	src/gflib/trig.o \
	src/agb_flash_v102.o \
	src/agb_flash_1m_v102.o \
	src/agb_flash_mx_v102.o \
	src/libpmagb/random.o \
	src/save.o \
	src/libpmagb/save_data.o \
	src/libpmagb/strcmp.o \
	src/libpmagb/string_util.o \
	src/libpmagb/target_game.o \
	src/joybus.o \
	src/multi_sio.o \
	src/main.o \
	src/box_crypto.o \
	src/calculate_box_mon_checksum.o \
	src/calculate_mon_stats.o \
	src/create_box_mon.o \
	src/get_box_mon_data.o \
	src/get_mon_data.o \
	src/get_nature.o \
	src/get_species_name.o \
	src/get_substruct.o \
	src/level.o \
	src/move_shift.o \
	src/moves.o \
	src/modify_stat_by_nature.o \
	src/pokemon.o \
	src/set_box_mon_data.o \
	src/set_mon_data.o
