#include "global.h"
#include "gift.h"
#include "save_blocks.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/species.h"

// The delivery's data, which is the gift, its messages (Shift-JIS, see
// SJIS_SRCS in the Makefile) and graphics.

// GetSpeciesName's.
const u8 gJirachiName[] = _("ジラーチ$");

static const u8 sGiftOTName[] = _("ネガイボシ$$");

const struct GiftOT gGiftOT = {sGiftOTName, 30719, 0};

static const u32 sJirachiRibbons[] = {0, 0};

// Written to at run time (the held item), though it is read-only data.
const struct GiftMon gJirachiGift =
{
    .level = 5,
    .species = SPECIES_JIRACHI,
    .moves = {MOVE_NONE, MOVE_NONE, MOVE_NONE, MOVE_NONE},
    .heldItem = ITEM_DREAM_MAIL,
    .ribbons = sJirachiRibbons,
    .shiny = FALSE,
};

// Unused (see CreateGiftEgg). Eggs from other events of this program's era.
const struct EggTemplate gGiftEggs[] =
{
    {5, 100, SPECIES_PICHU,   {MOVE_TEETER_DANCE}, ITEM_DREAM_MAIL, FALSE},
    {5,  25, SPECIES_PICHU,   {MOVE_TEETER_DANCE}, ITEM_DREAM_MAIL, TRUE},
    {5, 100, SPECIES_PICHU,   {MOVE_WISH},         ITEM_DREAM_MAIL, FALSE},
    {5,  25, SPECIES_PICHU,   {MOVE_WISH},         ITEM_DREAM_MAIL, TRUE},
    {5, 125, SPECIES_BAGON,   {MOVE_IRON_DEFENSE}, ITEM_DREAM_MAIL, FALSE},
    {5, 125, SPECIES_BAGON,   {MOVE_WISH},         ITEM_DREAM_MAIL, FALSE},
    {5, 125, SPECIES_ABSOL,   {MOVE_SPITE},        ITEM_DREAM_MAIL, FALSE},
    {5, 125, SPECIES_ABSOL,   {MOVE_WISH},         ITEM_DREAM_MAIL, FALSE},
    {5, 125, SPECIES_RALTS,   {MOVE_CHARM},        ITEM_DREAM_MAIL, FALSE},
    {5, 125, SPECIES_RALTS,   {MOVE_WISH},         ITEM_DREAM_MAIL, FALSE},
    {0},
};

// The species IsGiftSpecies looks for in the party and boxes.
const u16 gGiftSpecies[] = {SPECIES_JIRACHI, SPECIES_NONE};

const u8 gText_NoCartridge[] = "カートリッジがささっていません";
const u8 gText_NotPokemonCartridge[] = "これはポケモンのカートリッジではありません";
const u8 gText_ReceivingJirachi[] = "ジラーチをうけとっています\nでんげんをきらないでください";
const u8 gGameCode_Ruby[] = "AXVJ";
const u8 gGameCode_Sapphire[] = "AXPJ";
const u8 gText_ErrorNoFlashRam[] = "フラッシュメモリがみつかりません";
const u8 gText_ErrorNoSaveData[] = "セーブデータがみつかりません";
const u8 gText_ErrorDestroyedFlashData[] = "セーブデータがこわれています";
const u8 gText_ErrorProceedGame[] = "ゲームがすすんでいません";
const u8 gText_Error6Pokemon[] = "てもちポケモンがいっぱいです";
// Unused.
const u8 gText_DataTampered[] = "データかいぞうのうたがいがあります\nＡボタンでセーブかいし";
const u8 gText_DebugError[] = "デバッグちゅうのエラー";
const u8 gText_ReceivedJirachi[] = "ジラーチをうけとりました";
const u8 gText_FailedToSave[] = "セーブしっぱい\nもういちどやりなおしてください";
const u8 gText_ReceivedJirachiTurnOff[] = "ジラーチをうけとりました\nかわいがってあげてね\nでんげんをきってケーブルをぬいてください";

const u16 gDeliveryBgPalette[] = INCBIN_U16("graphics/delivery_bg.gbapal");

// One 64x64 affine sprite.
const u16 gJirachiOamData[] = {0x0100, 0xC000, 0x0000, 0xFFFF};

const u16 gJirachiPalette[] = INCBIN_U16("graphics/jirachi/palette.gbapal");
const u8 gJirachiGfx[] = INCBIN_U8("graphics/jirachi/front_pic.4bpp");

// The letter the gift would hold if its item were Dream Mail.
struct MailStruct gGiftMail =
{
    .words = {0x0201, 0x0202, 0x0203, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF},
    .playerName = _("オクリヌシ$$"),
    .trainerId = {0x87, 0x65, 0x43, 0x21},
    .species = SPECIES_JIRACHI,
    .itemId = ITEM_DREAM_MAIL,
};
