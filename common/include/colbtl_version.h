#ifndef GUARD_COLBTL_VERSION_H
#define GUARD_COLBTL_VERSION_H

// Shared code that differs between colbtl's releases tests COLBTL_IS(version)
// (e.g. COLBTL_IS(COLBTL) for the first English one). colbtl's Makefile sets
// COLBTL_VERSION; every other program is none of colbtl's versions. Code that
// differs only by language tests GAME_LANGUAGE instead.
#ifdef COLBTL_VERSION
#define COLBTL_IS(version) (COLBTL_VERSION == VERSION_##version)
#else
#define COLBTL_IS(version) 0
#endif

#endif // GUARD_COLBTL_VERSION_H
