#ifndef GUARD_POKEDEX_UTIL_H
#define GUARD_POKEDEX_UTIL_H

// pokeruby's, not libpmagb's. sample0519 links its own copies.
s8 GetSetPokedexFlag(u16 nationalDexNo, u8 caseID);
u16 SpeciesToNationalPokedexNum(u16 species);

#endif // GUARD_POKEDEX_UTIL_H
