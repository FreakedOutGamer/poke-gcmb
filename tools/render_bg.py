#!/usr/bin/env python3
"""Render a GBA 4bpp text background from its palette, map, and tiles."""

import argparse
import struct
from pathlib import Path

from PIL import Image


def gba_color(value):
    red = value & 0x1F
    green = (value >> 5) & 0x1F
    blue = (value >> 10) & 0x1F
    return tuple((channel << 3) | (channel >> 2) for channel in (red, green, blue))


parser = argparse.ArgumentParser()
parser.add_argument("palette", type=Path)
parser.add_argument("tilemap", type=Path)
parser.add_argument("tiles", type=Path)
parser.add_argument("output", type=Path)
args = parser.parse_args()

palette_data = args.palette.read_bytes()
map_data = args.tilemap.read_bytes()
tile_data = args.tiles.read_bytes()

if len(palette_data) != 0x20:
    raise SystemExit("expected one 16-color palette (0x20 bytes)")
if len(map_data) != 0x800:
    raise SystemExit("expected one 32x32 text-background map (0x800 bytes)")
if len(tile_data) % 32:
    raise SystemExit("4bpp tile data must be a multiple of 32 bytes")

palette = [gba_color(value) for value in struct.unpack("<16H", palette_data)]
entries = struct.unpack("<1024H", map_data)
tile_count = len(tile_data) // 32
image = Image.new("RGB", (256, 256), palette[0])
pixels = image.load()

for map_y in range(32):
    for map_x in range(32):
        entry = entries[map_y * 32 + map_x]
        tile_index = entry & 0x3FF
        hflip = bool(entry & 0x400)
        vflip = bool(entry & 0x800)
        palette_bank = entry >> 12

        # The program clears EWRAM before decompression and copies a full
        # 0x8000-byte character block. References beyond the stored 0x5000
        # bytes therefore select blank, zero-filled tiles.
        if tile_index >= tile_count:
            continue

        tile = tile_data[tile_index * 32:(tile_index + 1) * 32]
        for y in range(8):
            source_y = 7 - y if vflip else y
            for x in range(8):
                source_x = 7 - x if hflip else x
                packed = tile[source_y * 4 + source_x // 2]
                color_index = (packed >> (4 * (source_x & 1))) & 0xF
                # This payload has one palette bank, but retaining the bank
                # calculation documents the normal GBA text-map format.
                palette_index = palette_bank * 16 + color_index
                pixels[map_x * 8 + x, map_y * 8 + y] = palette[palette_index]

args.output.parent.mkdir(parents=True, exist_ok=True)
image.save(args.output)
