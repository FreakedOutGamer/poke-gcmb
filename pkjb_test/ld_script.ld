/* pkjb_test is streamed to and executed from GBA EWRAM. */
ENTRY(Start)

SECTIONS
{
    . = 0x02000000;

    .text : ALIGN(4)
    {
        build/asm/rom_header.o(.header)
        build/asm/crt0.o(.crt0)
        build/src/main.o(.text)
        build/src/intr.o(.text)
        build/src/test_screen.o(.text)
        build/asm/libagbsyscall.o(.text.libagbsyscall)
        build/src/assets.o(.data)
        build/src/assets.o(.rodata)
    }

    __image_end = .;
    ASSERT(__image_end == 0x02001B24, "pkjb_test image size changed")

    /DISCARD/ :
    {
        *(.comment)
        *(.ARM.attributes)
        *(.note.*)
    }
}
