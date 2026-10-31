#ifndef GUARD_TEXT_H
#define GUARD_TEXT_H

void LoadTilemapFont(u8 *dest, u8 tilemapFont, u8 ink, u8 shadow);
void PrintfTilemap(u16 *tilemap, u8 x, u8 y, u16 attr, const char *fmt, ...);

#endif // GUARD_TEXT_H
