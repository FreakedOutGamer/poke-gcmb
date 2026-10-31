#!/usr/bin/env python3
"""Extract one compiler-generated function section as a relocatable object.

The input is produced from the original C file with -ffunction-sections.  This
keeps independently matching functions independently placeable without
duplicating source files.  Old GNU as pads isolated Thumb sections with a NOP;
the original combined section used zero alignment, so that tail padding is
normalized here.
"""

from pathlib import Path
import shutil
import subprocess
import sys
import tempfile

from elftools.elf.elffile import ELFFile


def main():
    if len(sys.argv) != 4:
        raise SystemExit(f"usage: {sys.argv[0]} INPUT.o FUNCTION OUTPUT.o")
    input_path = Path(sys.argv[1])
    function = sys.argv[2]
    output_path = Path(sys.argv[3])
    section_name = f".text.{function}"

    with tempfile.NamedTemporaryFile(suffix=".o") as rewritten:
        shutil.copyfile(input_path, rewritten.name)
        # Static functions are local symbols in the compiler output. Promote
        # all of them before separating the sections so sibling calls become
        # ordinary undefined relocations and the selected function can satisfy
        # those calls from its own independently linked object.
        with open(rewritten.name, "rb") as obj:
            elf = ELFFile(obj)
            symbols = elf.get_section_by_name(".symtab")
            local_symbols = [
                symbol.name for symbol in symbols.iter_symbols()
                if (symbol.name
                    and symbol["st_info"]["type"] in ("STT_FUNC", "STT_OBJECT")
                    and symbol["st_info"]["bind"] == "STB_LOCAL"
                    and symbol["st_shndx"] != "SHN_UNDEF")
            ]
        subprocess.run(
            ["arm-none-eabi-objcopy", "--strip-debug",
             *(f"--globalize-symbol={name}" for name in local_symbols),
             rewritten.name],
            check=True,
        )
        with open(rewritten.name, "r+b") as obj:
            elf = ELFFile(obj)
            symbols = elf.get_section_by_name(".symtab")
            symbol_table_offset = symbols["sh_offset"]
            symbol_size = symbols["sh_entsize"]
            for index, symbol in enumerate(symbols.iter_symbols()):
                if symbol.name == function or symbol["st_shndx"] == "SHN_UNDEF":
                    continue
                entry = symbol_table_offset + index * symbol_size
                if symbol["st_info"]["type"] == "STT_FUNC":
                    # Calls to other functions in the same C file must remain
                    # relocations for the independently linked output object.
                    obj.seek(entry + 4)   # st_value
                    obj.write(bytes(4))
                    obj.seek(entry + 12)  # st_info: global function
                    obj.write(bytes([0x12]))
                    obj.seek(entry + 14)  # st_shndx
                    obj.write(bytes(2))
                elif symbol["st_info"]["bind"] in ("STB_GLOBAL", "STB_WEAK"):
                    # Tentative definitions emitted as COMMON belong to the
                    # original translation unit, not every extracted routine.
                    # Keep them as ordinary external references so a split
                    # object cannot allocate duplicate EWRAM storage.
                    obj.seek(entry + 4)   # st_value / COMMON alignment
                    obj.write(bytes(4))
                    obj.seek(entry + 14)  # st_shndx
                    obj.write(bytes(2))

        rewritten_path = Path(rewritten.name)
        with tempfile.NamedTemporaryFile("w", suffix=".ld") as script:
            script.write(
                "SECTIONS {\n"
                f"  .text : {{ {rewritten_path}({section_name}) }}\n"
                "  /DISCARD/ : {\n"
                f"    {rewritten_path}(.text.*) {rewritten_path}(.rodata)\n"
                f"    {rewritten_path}(.data) {rewritten_path}(.bss)\n"
                f"    {rewritten_path}(ewram_data) {rewritten_path}(.ARM.attributes)\n"
                "  }\n"
                "}\n"
            )
            script.flush()
            subprocess.run(
                ["arm-none-eabi-ld", "-r", "-T", script.name, "-o", output_path, rewritten_path],
                check=True,
            )

    with output_path.open("r+b") as out:
        elf = ELFFile(out)
        text = elf.get_section_by_name(".text")
        symbols = elf.get_section_by_name(".symtab")
        matching_symbols = symbols.get_symbol_by_name(function)
        if not matching_symbols:
            raise ValueError(f"{function}: symbol not found")
        symbol = matching_symbols[0]
        function_size = symbol["st_size"]
        section_size = text["sh_size"]
        if function_size > section_size:
            raise ValueError(f"{function}: symbol exceeds its section")
        out.seek(text["sh_offset"] + function_size)
        out.write(bytes(section_size - function_size))


if __name__ == "__main__":
    main()
