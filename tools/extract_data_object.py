#!/usr/bin/env python3
"""Extract one compiler-generated data section as a relocatable object."""

from pathlib import Path
import subprocess
import sys
import tempfile

from elftools.elf.elffile import ELFFile


def main():
    if len(sys.argv) != 4:
        raise SystemExit(f"usage: {sys.argv[0]} INPUT.o SYMBOL OUTPUT.o")
    input_path = Path(sys.argv[1])
    symbol_name = sys.argv[2]
    output_path = Path(sys.argv[3])

    with input_path.open("rb") as obj:
        elf = ELFFile(obj)
        symbols = elf.get_section_by_name(".symtab").get_symbol_by_name(symbol_name)
        if not symbols:
            raise ValueError(f"{symbol_name}: symbol not found")
        symbol = symbols[0]
        section = elf.get_section(symbol["st_shndx"])
        section_name = section.name
        output_section = ".rodata" if section_name.startswith(".rodata") else ".data"

    with tempfile.NamedTemporaryFile("w", suffix=".ld") as script:
        script.write(
            "SECTIONS {\n"
            f"  {output_section} : {{ {input_path}({section_name}) }}\n"
            "  /DISCARD/ : {\n"
            f"    {input_path}(.text) {input_path}(.data*) {input_path}(.rodata*)\n"
            f"    {input_path}(.bss) {input_path}(.ARM.attributes)\n"
            "  }\n"
            "}\n"
        )
        script.flush()
        subprocess.run(
            ["arm-none-eabi-ld", "-r", "-T", script.name, "-o", output_path, input_path],
            check=True,
        )


if __name__ == "__main__":
    main()
