#include "global.h"

// Localized label stored beside the regional ROM descriptors.
// Fixed record sizes preserve the original trailing zero bytes.
#if COLBTL_VERSION == VERSION_COLBTL_JP
const u8 gText_BadEgg[8] = _("ダメタマゴ");
#elif COLBTL_VERSION == VERSION_COLBTL_F
const u8 gText_BadEgg[12] = _("Mauv. OEUF");
#elif COLBTL_VERSION == VERSION_COLBTL_G
const u8 gText_BadEgg[12] = _("Schl. EI");
#elif COLBTL_VERSION == VERSION_COLBTL_I
const u8 gText_BadEgg[12] = _("UOVO peste");
#elif COLBTL_VERSION == VERSION_COLBTL_S
const u8 gText_BadEgg[12] = _("Huevo MALO");
#endif
