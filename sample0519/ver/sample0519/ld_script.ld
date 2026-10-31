/* Outer multiboot image: AGB header, ARM loader and the LZ77 payload. */
ENTRY(Start)
SECTIONS
{
    . = 0x02000000;
    .text : ALIGN(4)
    {
        build/sample0519/asm/rom_header.o(.header)
        build/sample0519/data/raw.o(.raw)
    }
    __image_end = .;
    ASSERT(__image_end == 0x020116E8, "image size changed")
    /DISCARD/ : { *(.comment) *(.ARM.attributes) *(.note.*) }
}
