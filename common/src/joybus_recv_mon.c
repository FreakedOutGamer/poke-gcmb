#include "global.h"
#include "libpmagb/save_block_pointers.h"
#include "pokemon.h"
#include "colbtl.h"
#include "joybus.h"

// Takes the words of a party Pokémon transfer. These are the party slot,
// then the species, then the Pokémon itself, then its ribbons.

void ReceivePartyMon(u32 val)
{
    u32 species, lowerByte, upperByte;

    switch (gJoyBusState.state)
    {
    case 0:
        gMonLinkData.monId = (u8)(val & 0x7);
        gJoyBusState.state++;
        break;
    case 1:
        gMonLinkData.species2 = val & 0xFFFF;

        // The high half is the species again, stored a byte at a time.
        species = (val >> 16) & 0xFFFF;
        lowerByte = species & 0xFF;
        gMonLinkData.speciesLowerByte = lowerByte;
        upperByte = (species >> 8) & 0xFF;
        gMonLinkData.speciesUpperByte = upperByte;

        gMonLinkData.monPtr = (void*) &gPlayerPartyPtr[gMonLinkData.monId];
        gJoyBusState.transferSize = sizeof(struct Pokemon);
        gJoyBusState.transferBytes = 0;
        gJoyBusState.state++;
        break;
    case 2:
        gMonLinkData.monPtr[gJoyBusState.transferBytes / 4] = val;
        gJoyBusState.transferBytes += 4;

        if (gJoyBusState.transferBytes >= gJoyBusState.transferSize)
        {
            gJoyBusState.buffer = (void *) gMonLinkData.giftRibbons;
            gJoyBusState.transferSize = sizeof(gMonLinkData.giftRibbons);
            gJoyBusState.transferBytes = 0;
            gJoyBusState.state++;
        }
        break;
    case 3:
        gJoyBusState.buffer[gJoyBusState.transferBytes / 4] = val;
        gJoyBusState.transferBytes += 4;

        if (gJoyBusState.transferBytes >= gJoyBusState.transferSize)
        {
            gMonLinkData.transferComplete = 1;
            gJoyBusState.command = LINK_CMD_RESET;
        }
        break;
    }
}
