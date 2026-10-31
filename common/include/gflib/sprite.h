#ifndef GFLIB_SPRITE_H
#define GFLIB_SPRITE_H

// A sprite is drawn as a list of these, ending with one whose attrs are
// 0xFFFF. x and y are offsets from the sprite's own position, and attrs
// carries the shape and size bits BuildOamBuffer measures the piece with.
struct Subsprite
{
    /*0x00*/ u32 attrs;
    /*0x04*/ s16 x;
    /*0x06*/ s16 y;
    /*0x08*/ u16 attr2;
    /*0x0A*/ u16 unused;
};

// gflib's own sprite record, not a raw OAM entry. Its x and y are whole
// halfwords rather than the packed fields OAM uses.
struct Sprite
{
    // Sprites are chained by index rather than by pointer, one byte each way.
    /*0x00*/ u8 prev;
    /*0x01*/ u8 next;
    /*0x02*/ u8 flags; // bit 0 only; see SetSpriteInvisible
    /*0x04*/ u16 paletteBits; // OR'd into attr2, so the palette number
    /*0x06*/ u16 tileOffset; // added to attr2, so a tile offset
    /*0x08*/ s16 x;
    /*0x0A*/ s16 y;
    /*0x0C*/ const struct Subsprite *subsprites;
    /*0x10*/ void (*callback)(struct Sprite *);
    /*0x14*/ u32 data[4];
}; /*size = 0x24*/

struct CompressedSpriteSheet
{
    const u32 *data;  // LZ77 compressed pixel data
    u16 size;        // Uncompressed size of pixel data
    u16 tag;
};

struct SpritePalette
{
    const u16 *data;  // Raw uncompressed palette data
    u16 tag;
};

struct CompressedSpritePalette
{
    const u32 *data;  // LZ77 compressed palette data
    u16 tag;
};

void ResetSprites(void);
void UpdateSprites(void);
void DoOamBufferTransfer(void);
struct Sprite * AddSprite(s32 x, s32 y, const struct Subsprite *subsprites);
void MoveSpriteToHead(struct Sprite * sprite);
void SetSpritePos(struct Sprite * sprite, s32 x, s32 y);
void AddSpritePos(struct Sprite * sprite, s32 x, s32 y);
void SetSpritePaletteNum(struct Sprite * sprite, s32 paletteNum);
void SetSpriteTileOffset(struct Sprite * sprite, s32 tileOffset);
void SetSpriteInvisible(struct Sprite * sprite, s32 invisible);

#endif //GFLIB_SPRITE_H
