	.section .rodata.fonts, "a"
	.align 2, 0

	@ Font rendering parameters followed by the two glyph-set pointers.
	@ The packed leading values are retained as words until the renderer that
	@ consumes this descriptor is decompiled and its fields can be named.
	.global gFont0LatinInfo
gFont0LatinInfo:
	.word 0x160F0402
	.word 0x00200004
	.word 0x0000100F
	.word 0x06008400
	.word gCouponJapaneseGlyphs
	.word gCouponLatinGlyphs
	.word 0

	@ Editable sources are graphics/fonts/*.png.  The normal build recreates
	@ these native 1bpp tile streams without reading baserom.mb.
	.global gCouponJapaneseGlyphs
gCouponJapaneseGlyphs:
	.incbin "graphics/fonts/japanese.1bpp"

	.global gCouponLatinGlyphs
gCouponLatinGlyphs:
	.incbin "graphics/fonts/latin.1bpp"

