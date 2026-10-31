#include "global.h"

// Loaded to OBJ palettes 0 (Jirachi) and 1 (the save icon).
#if REVISION == 0
const u16 gDeliverySpritePalettes[][16] =
{
    INCBIN_U16("graphics/jirachi/palette_rev0.gbapal"),
    INCBIN_U16("graphics/save_icon/palette_rev0.gbapal"),
};

const u8 gSaveIconGfx[] = INCBIN_U8("graphics/save_icon/icon_rev0.4bpp");
const u8 gJirachiSpriteGfx[] = INCBIN_U8("graphics/jirachi/front_pic_rev0.4bpp");
#else
const u16 gDeliverySpritePalettes[][16] =
{
    INCBIN_U16("graphics/jirachi/palette_rev1.gbapal"),
    INCBIN_U16("graphics/save_icon/palette_rev1.gbapal"),
};

const u8 gSaveIconGfx[] = INCBIN_U8("graphics/save_icon/icon_rev1.4bpp");
const u8 gJirachiSpriteGfx[] = INCBIN_U8("graphics/jirachi/front_pic_rev1.4bpp");
#endif
