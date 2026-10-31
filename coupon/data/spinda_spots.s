	.section .rodata.spinda_spots, "a"
	.align 2, 0

	@ Four 16x16 masks used by DrawSpindaSpots.  Each record stores the
	@ unshifted origin, sixteen little-endian row masks, and an unused halfword.
	.global gSpindaSpotGraphics
gSpindaSpotGraphics:
	.byte 16, 14
	.incbin "graphics/spinda_spots/spot_0.bin"
	.short 0
	.byte 40, 15
	.incbin "graphics/spinda_spots/spot_1.bin"
	.short 0
	.byte 22, 32
	.incbin "graphics/spinda_spots/spot_2.bin"
	.short 0
	.byte 34, 33
	.incbin "graphics/spinda_spots/spot_3.bin"
	.short 0
