#include "global.h"
#include "text.h"
#include "save.h"
#include "delivery.h"

extern u16 gTilemap[];
extern const u8 gText_NotPokemonCartridge[];
extern const u8 gText_ErrorNoSaveData[];
extern const u8 gText_ErrorProceedGame[];
extern const u8 gText_Error6Pokemon[];
extern const u8 gText_ErrorDestroyedFlashData[];
extern const u8 gText_FailedToSave[];
extern const u8 gText_JirachiAlreadyDelivered[];
extern const u8 gText_UnsupportedVersion[];

void ClearTilemapRect(s32 x, s32 y, s32 width, s32 height);

// Draws an ASCII string (with 0x1B for the é of "Pokémon") to the BG
// tilemap, one tile per character. '\n' starts a new row back at x.
// Characters the font lacks are drawn as blanks.
void PrintString(const u8 *str, s32 x, s32 y, u32 palette)
{
    s32 cursorX = x;
    s32 cursorY = y;

    while (*str != '\0')
    {
        switch (*str)
        {
        default:
            if (*str >= 'A' && *str <= 'Z')
                gTilemap[cursorX + cursorY * 32] = (palette << 12) | (*str - 33);
            else if (*str >= 'a' && *str <= 'z')
                gTilemap[cursorX + cursorY * 32] = (palette << 12) | (*str - 39);
            else if (*str >= '0' && *str <= '9')
                gTilemap[cursorX + cursorY * 32] = (palette << 12) | (*str - 26);
            else
                gTilemap[cursorX + cursorY * 32] = palette << 12;
            cursorX++;
            str++;
            break;
        case ' ':
            gTilemap[cursorX + cursorY * 32] = palette << 12;
            cursorX++;
            str++;
            break;
        case '.':
            gTilemap[cursorX + cursorY * 32] = (palette << 12) | 18;
            cursorX++;
            str++;
            break;
        case '!':
            gTilemap[cursorX + cursorY * 32] = (palette << 12) | 7;
            cursorX++;
            str++;
            break;
        case '?':
            gTilemap[cursorX + cursorY * 32] = (palette << 12) | 6;
            cursorX++;
            str++;
            break;
        case '\'':
            gTilemap[cursorX + cursorY * 32] = (palette << 12) | 124;
            cursorX++;
            str++;
            break;
        case CHAR_e_ACUTE:
            gTilemap[cursorX + cursorY * 32] = (palette << 12) | 125;
            cursorX++;
            str++;
            break;
        case '\n':
            cursorY++;
            cursorX = x;
            str++;
            break;
        }
    }
}

void UNUSED PrintDecimal(u32 value, u32 x, u32 y, u32 palette)
{
    u32 n;
    u32 digits = 0;
    u32 i;
    s32 pos;

    n = value;
    if (n != 0)
    {
        do
        {
            digits++;
            n /= 10;
        } while (n != 0);
    }

    if (digits == 0)
        gTilemap[y * 32 + x] = (palette << 12) | 22;

    pos = 0;
    while (digits != 0)
    {
        n = value;
        for (i = 0, digits--; i < digits; i++)
            n /= 10;
        n %= 10;
        gTilemap[y * 32 + x + pos] = (palette << 12) | (n + 22);
        pos++;
    }
}

// Prints value as "0x" followed by numBytes * 2 lowercase hex digits.
void PrintHex(u32 value, s32 x, s32 y, u32 palette, s32 numBytes)
{
    s32 i, j;

    gTilemap[x + y * 32] = (palette << 12) | 22;
    gTilemap[x + y * 32 + 1] = (palette << 12) | 'x';
    for (i = numBytes * 2 - 1, j = 2; i >= 0; j++, i--)
    {
        s32 digit = (value >> (i * 4)) & 0xF;

        if (digit <= 9)
            digit += 22;
        else
            digit += 'a' - 10;
        gTilemap[x + y * 32 + j] = digit | (palette << 12);
    }
}

void ClearTilemapRect(s32 x, s32 y, s32 width, s32 height)
{
    s32 i, j;

    for (i = 0; i < width; i++)
    {
        for (j = 0; j < height; j++)
            gTilemap[(y + j) * 32 + x + i] = 0;
    }
}

// Clears the given rectangle, then prints str centered inside it.
void PrintStringCentered(const u8 *str, s32 x, s32 y, s32 width, s32 height, u32 palette)
{
    s32 maxLineLength = 0;
    s32 numLines = 1;
    s32 lineLength = 0;
    const u8 *c = str;

    ClearTilemapRect(x, y, width, height);
    for (; *c != '\0'; c++)
    {
        if (*c == '\n')
        {
            if (lineLength > maxLineLength)
                maxLineLength = lineLength;
            lineLength = 0;
            numLines++;
        }
        else
        {
            lineLength++;
        }
    }
    if (lineLength > maxLineLength)
        maxLineLength = lineLength;

    PrintString(str, x + (width - maxLineLength) / 2, y + (height - numLines) / 2, palette);
}

void PrintErrorMessage(u32 error)
{
    switch (error)
    {
    case DELIVERY_ERROR_NOT_POKEMON_CARTRIDGE:
        PrintStringCentered(gText_NotPokemonCartridge, 1, MESSAGE_BOX_Y, 27, MESSAGE_BOX_HEIGHT, 0);
        break;
    case DELIVERY_ERROR_NO_SAVE_DATA:
        PrintStringCentered(gText_ErrorNoSaveData, 1, MESSAGE_BOX_Y, 27, MESSAGE_BOX_HEIGHT, 0);
        break;
    case DELIVERY_ERROR_SAVE_FAILED:
        PrintStringCentered(gText_FailedToSave, 1, MESSAGE_BOX_Y, 27, MESSAGE_BOX_HEIGHT, 0);
        break;
    case DELIVERY_ERROR_PROCEED_GAME:
        PrintStringCentered(gText_ErrorProceedGame, 1, MESSAGE_BOX_Y, 27, MESSAGE_BOX_HEIGHT, 0);
        break;
    case DELIVERY_ERROR_PARTY_FULL:
        PrintStringCentered(gText_Error6Pokemon, 1, MESSAGE_BOX_Y, 27, MESSAGE_BOX_HEIGHT, 0);
        break;
    case DELIVERY_ERROR_NO_SAVE_DATA_UNUSED:
        PrintStringCentered(gText_ErrorNoSaveData, 1, MESSAGE_BOX_Y, 27, MESSAGE_BOX_HEIGHT, 0);
        break;
    case DELIVERY_ERROR_SAVE_CORRUPTED:
        PrintStringCentered(gText_ErrorDestroyedFlashData, 1, MESSAGE_BOX_Y, 27, MESSAGE_BOX_HEIGHT, 0);
        break;
    case DELIVERY_ERROR_NO_FLASH:
        // BUG: The payload has an "Error! No Flash RAM." message for this, but
        // it is never used (the later revision leaves an unused second copy of
        // its "save file is corrupted" message in the same place).
        PrintStringCentered(gText_ErrorDestroyedFlashData, 1, MESSAGE_BOX_Y, 27, MESSAGE_BOX_HEIGHT, 0);
        break;
    case DELIVERY_ERROR_ALREADY_DELIVERED:
        PrintStringCentered(gText_JirachiAlreadyDelivered, 1, MESSAGE_BOX_Y, 27, MESSAGE_BOX_HEIGHT, 0);
        break;
    case DELIVERY_ERROR_UNSUPPORTED_VERSION:
        PrintStringCentered(gText_UnsupportedVersion, 1, MESSAGE_BOX_Y, 27, MESSAGE_BOX_HEIGHT, 0);
        break;
    }
}

s32 SaveStatusToDeliveryError(s32 status)
{
    u32 result;

    switch (status)
    {
    case SAVE_STATUS_CORRUPT:
    case SAVE_STATUS_ERROR:
        result = DELIVERY_ERROR_SAVE_CORRUPTED;
        break;
    case SAVE_STATUS_NO_FLASH:
        result = DELIVERY_ERROR_NO_FLASH;
        break;
    case SAVE_STATUS_EMPTY:
        result = DELIVERY_ERROR_NO_SAVE_DATA;
        break;
    }
    return result;
}
