#include "global.h"
#include "string_util.h"
#include "pokemon.h"

// The only species name the program has. It is here in the client, and with
// the delivery data (src/delivery_data.c) in sample0519.
#ifdef PROGRAM_CLIENT
const u8 gJirachiName[] = _("JIRACHI");
#else
extern const u8 gJirachiName[];
#endif

void GetSpeciesName(u8 *name, s32 species)
{
    s32 i;

    for (i = 0; i < POKEMON_NAME_LENGTH + 1; i++)
    {
        name[i] = gJirachiName[i];
        if (name[i] == EOS)
            break;
    }
    name[i] = EOS;
}
