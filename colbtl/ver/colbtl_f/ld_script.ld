/* Outer multiboot image, laid out as colosseum-mb's: AGB header, outer
   program, the LZ77 payload, the outer program's data and the bootstrap. */
ENTRY(Start)
SECTIONS
{
    . = 0x02000000;
    .text : ALIGN(4)
    {
        build/colbtl_f/asm/rom_header.o(.header)
        build/colbtl_f/outer/crt0.o(.text)
        build/colbtl_f/outer/main.o(.text)
        build/colbtl_f/outer/libagbsyscall.o(.text)
        build/colbtl_f/libgcc/_call_via_rX.o(.text)
        . += 0x100;
        build/colbtl_f/data/raw.o(.raw)
        . = ALIGN(16);
        build/colbtl_f/outer/main.o(.rodata)
        build/colbtl_f/outer/iwram.o(.text)
    } = 0
    __image_end = .;
    ASSERT(__image_end == 0x020168CC, "image size changed")

    /* Where AgbMain copies the bootstrap: the LZ77 routine, then
       arm_Bootstrap, which it runs. */
    arm_LZ77UnCompWRAM_Buffer = 0x03000000;
    arm_LZ77UnCompWRAM_FuncStart = 0x03000130;
    arm_LZ77UnCompWRAM_BufferEnd = 0x030001C4;

    /DISCARD/ : { *(.comment) *(.ARM.attributes) *(.note.*) }
}
