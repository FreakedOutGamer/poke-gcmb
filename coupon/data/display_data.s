	.section .rodata.main_display, "a"
	.align 2, 0

	@ BIOS run-length stream copied to VRAM at 0x06000000 during startup.
	@ Built reproducibly from graphics/main_screen/tiles.png; no baserom data
	@ is used by the normal build.
	.global gTitle_Gfx
gTitle_Gfx:
	.incbin "graphics/main_screen/tiles.4bpp.rl"

	.global gTitle_Pal
gTitle_Pal:
	.incbin "graphics/main_screen/palettes.gbapal.rl"

	.global gTitle_Tilemap
gTitle_Tilemap:
	.incbin "graphics/main_screen/tilemap.bin.lz"

	.global gTitleExtraPal
gTitleExtraPal:
	.incbin "graphics/overlay/palette.gbapal.rl"

	.global gMessageBox_Gfx
gMessageBox_Gfx:
	.incbin "graphics/overlay/tiles.4bpp.lz"

	.global gTextPal
gTextPal:
	.incbin "graphics/alternate_palettes.gbapal.lz"
	.align 2, 0
