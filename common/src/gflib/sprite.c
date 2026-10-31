#include "global.h"
#include "gflib/sprite.h"

// BuildOamBuffer's size key is (shape << 2) | size, the reverse of
// pokeemerald's SPRITE_SIZE_* encoding.
#define SPRITE_KEY(dim) (SPRITE_SIZE(dim) | (SPRITE_SHAPE(dim) << 2))

// An image that places this bss itself builds with SDK_EXTERN_BSS.
#ifdef SDK_EXTERN_BSS
extern struct Sprite sSprites[18];
#else
static struct Sprite sSprites[18];
#endif

struct OamBuffer
{
    struct OamData oamBuffer[0x80];
    u8 numSprites;
};

#ifdef SDK_EXTERN_BSS
extern struct OamBuffer sOamBuffer;
#else
static struct OamBuffer sOamBuffer;
#endif

static void InsertSprite(struct Sprite * sprite1, struct Sprite * sprite2)
{
    sprite2->next = sprite1->next;
    sprite2->prev = sprite1 - sSprites;
    sprite1->next = sSprites[sprite1->next].prev = sprite2 - sSprites;
}

struct Sprite * AddSprite(s32 x, s32 y, const struct Subsprite * subsprites)
{
    s32 i;
    struct Sprite * newSprite = &sSprites[sSprites[0].next];

    sSprites[newSprite->prev].next = newSprite->next;
    sSprites[newSprite->next].prev = newSprite->prev;
    InsertSprite(&sSprites[1], newSprite);
    newSprite->x = x;
    newSprite->y = y;
    newSprite->subsprites = subsprites;
    newSprite->flags = 0;
    newSprite->paletteBits = 0;
    newSprite->tileOffset = 0;
    newSprite->callback = NULL;
    for (i = 0; i < 4; i++)
        newSprite->data[i] = 0;
    return newSprite;
}

static void BuildOamBuffer(struct Sprite * sprite)
{
    u32 numSprites;
    struct OamData * oam;
    const struct Subsprite * templates;
    s32 width;
    s32 left;
    s32 top;
    s32 height;
    u32 size;
    u32 flags;

    numSprites = sOamBuffer.numSprites;
    oam = &sOamBuffer.oamBuffer[numSprites];
    templates = sprite->subsprites;

    while (templates->attrs != 0xFFFF)
    {
        if (numSprites == 0x80)
            return;

        if ((left = templates->x + sprite->x) < 0xF0 && (top = templates->y + sprite->y) < 0xA0)
        {
            size = ((templates->attrs & 0xC000) >> 12) + (templates->attrs >> 30);
            flags = templates->attrs;
            switch (size)
            {
            case SPRITE_KEY(8x8):
            case SPRITE_KEY(8x16):
            case SPRITE_KEY(8x32):
                width = 8;
                break;
            case SPRITE_KEY(16x16):
            case SPRITE_KEY(16x8):
            case SPRITE_KEY(16x32):
                width = 16;
                break;
            case SPRITE_KEY(32x32):
            case SPRITE_KEY(32x8):
            case SPRITE_KEY(32x16):
            case SPRITE_KEY(32x64):
                width = 32;
                break;;
            case SPRITE_KEY(64x64):
            case SPRITE_KEY(64x32):
                width = 64;
                break;
            default:
                width = 0;
                break;
            }
            if (left + width >= 0)
            {
                switch (size)
                {
                case SPRITE_KEY(8x8):
                case SPRITE_KEY(16x8):
                case SPRITE_KEY(32x8):
                    height = 8;
                    break;
                case SPRITE_KEY(16x16):
                case SPRITE_KEY(32x16):
                case SPRITE_KEY(8x16):
                    height = 16;
                    break;
                case SPRITE_KEY(32x32):
                case SPRITE_KEY(64x32):
                case SPRITE_KEY(8x32):
                case SPRITE_KEY(16x32):
                    height = 32;
                    break;
                case SPRITE_KEY(64x64):
                case SPRITE_KEY(32x64):
                    height = 64;
                    break;
                }
                if (top + height >= 0)
                {
                    left &= 0x1FF;
                    top &= 0xFF;
                    *(u32 *)oam = top | ((left << 16) | flags);
                    *((u16 *)oam + 2) = ((templates->attr2 & 0xFFF) | sprite->paletteBits) + sprite->tileOffset;
                    oam++;
                    numSprites++;
                }
            }
        }
        templates++;
    }
    sOamBuffer.numSprites = numSprites;
}

void UpdateSprites(void)
{
    s32 numSprites;
    s32 i;
    struct OamData * oam;
    struct Sprite * sprite;

    i = sSprites[1].next;

    while (i != 1)
    {
        sprite = &sSprites[i];

        if (sprite->callback != NULL)
            sprite->callback(sprite);

        if (!(sprite->flags & 1))
            BuildOamBuffer(sprite);
        i = sprite->next;
    }

    oam = &sOamBuffer.oamBuffer[sOamBuffer.numSprites];
    for (numSprites = sOamBuffer.numSprites; numSprites < 0x80 && *(u16 *)oam != 0x200; numSprites++, oam++)
    {
        *(u16 *)oam = 0x200;
    }
    sOamBuffer.numSprites = 0;
}

void ResetSprites(void)
{
    s32 i;
    struct OamData * oam;
    struct Sprite * sprite;

    CpuFill16(0, sSprites, sizeof(sSprites));
    sSprites[0].next = 0;
    sSprites[0].prev = 0;
    sSprites[1].next = 1;
    sSprites[1].prev = 1;

    for (i = 2; i < 18; i++)
    {
        InsertSprite(&sSprites[0], &sSprites[i]);
    }
    sOamBuffer.numSprites = 0;
    oam = &sOamBuffer.oamBuffer[0];
    for (i = 0; i < 0x80; i++, oam++)
        *(u16 *)oam = 0x200;
}

void MoveSpriteToHead(struct Sprite * sprite)
{
    sSprites[sprite->prev].next = sprite->next;
    sSprites[sprite->next].prev = sprite->prev;
    InsertSprite(&sSprites[0], sprite);
}

void DoOamBufferTransfer(void)
{
    DmaCopy32(3, sOamBuffer.oamBuffer, (void *)OAM, OAM_SIZE);
}

void SetSpritePos(struct Sprite * sprite, s32 x, s32 y)
{
    sprite->x = x;
    sprite->y = y;
}

void AddSpritePos(struct Sprite * sprite, s32 x, s32 y)
{
    sprite->x += x;
    sprite->y += y;
}

void SetSpritePaletteNum(struct Sprite * sprite, s32 paletteNum)
{
    sprite->paletteBits = paletteNum << 12;
}

void SetSpriteTileOffset(struct Sprite * sprite, s32 tileOffset)
{
    sprite->tileOffset = tileOffset;
}

void SetSpriteInvisible(struct Sprite * sprite, s32 invisible)
{
    sprite->flags = ((sprite->flags & ~1) | invisible);
}
