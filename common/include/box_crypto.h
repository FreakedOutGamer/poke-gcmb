#ifndef GUARD_BOX_CRYPTO_H
#define GUARD_BOX_CRYPTO_H

#include "pokemon.h"

// XOR a box Pokémon's secure data with its personality and OT id (both ways).
void EncryptBoxMon(struct BoxPokemon *boxMon);
void DecryptBoxMon(struct BoxPokemon *boxMon);

#endif // GUARD_BOX_CRYPTO_H
