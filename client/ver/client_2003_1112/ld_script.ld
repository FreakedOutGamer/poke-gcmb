/* Outer multiboot image: AGB header, ARM loader and the LZ77 payload. */
ENTRY(Start)
SECTIONS
{
    . = 0x02000000;
    .text : ALIGN(4)
    {
        build/client_2003_1112/asm/rom_header.o(.header)
        build/client_2003_1112/data/raw.o(.raw)
    }
    __image_end = .;
    ASSERT(__image_end == 0x02005F2C, "image size changed")
    /DISCARD/ : { *(.comment) *(.ARM.attributes) *(.note.*) }
}
