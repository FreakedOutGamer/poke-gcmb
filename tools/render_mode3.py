#!/usr/bin/env python3
"""Build a GBA mode-3 (direct color) bitmap from a PNG, or convert back.

  render_mode3.py to-bin IMAGE.png BIT15.bin FOOTER.bin OUT.bin
  render_mode3.py to-png IN.bin WIDTH HEIGHT OUT.png BIT15.bin FOOTER.bin

IMAGE.png holds the colors (8 bits per channel, from 5-bit values expanded
the usual way, so they convert back exactly). The PPU ignores bit 15 of a
mode-3 pixel, which a PNG cannot carry, so BIT15.bin keeps it: one bit per
pixel, in pixel order, LSB first. FOOTER.bin is appended after the pixels
(bytes the original stream carries past the image).
"""
import struct
import sys

from PIL import Image


def expand_5bit(value):
    return (value << 3) | (value >> 2)


def to_bin(png, bit15, footer, out):
    image = Image.open(png).convert('RGB')
    width, height = image.size
    mask = open(bit15, 'rb').read()
    if len(mask) != (width * height + 7) // 8:
        sys.exit(f'{bit15}: expected {(width * height + 7) // 8} bytes, got {len(mask)}')
    pixels = image.load()
    words = bytearray()
    for index in range(width * height):
        red, green, blue = pixels[index % width, index // width]
        if (expand_5bit(red >> 3), expand_5bit(green >> 3), expand_5bit(blue >> 3)) != (red, green, blue):
            sys.exit(f'{png}: pixel {index} is not a GBA color')
        word = (red >> 3) | ((green >> 3) << 5) | ((blue >> 3) << 10)
        if mask[index // 8] & (1 << (index & 7)):
            word |= 0x8000
        words += struct.pack('<H', word)
    open(out, 'wb').write(bytes(words) + open(footer, 'rb').read())


def to_png(binary, width, height, png, bit15, footer):
    data = open(binary, 'rb').read()
    count = width * height
    words = struct.unpack(f'<{count}H', data[:count * 2])
    image = Image.new('RGB', (width, height))
    pixels = image.load()
    mask = bytearray((count + 7) // 8)
    for index, word in enumerate(words):
        pixels[index % width, index // width] = (expand_5bit(word & 0x1F),
                                                 expand_5bit((word >> 5) & 0x1F),
                                                 expand_5bit((word >> 10) & 0x1F))
        if word & 0x8000:
            mask[index // 8] |= 1 << (index & 7)
    image.save(png)
    open(bit15, 'wb').write(mask)
    open(footer, 'wb').write(data[count * 2:])


if __name__ == '__main__':
    if len(sys.argv) == 6 and sys.argv[1] == 'to-bin':
        to_bin(*sys.argv[2:])
    elif len(sys.argv) == 8 and sys.argv[1] == 'to-png':
        to_png(sys.argv[2], int(sys.argv[3]), int(sys.argv[4]), *sys.argv[5:])
    else:
        sys.exit(__doc__)
