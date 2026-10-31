#ifndef GUARD_FADE_H
#define GUARD_FADE_H

// common/src/fade.c; pokechan's main.c has its own FadeIn and FadeOut.

bool32 IsScreenFadedOut(void);
void OverrideScreenFadeState(bool32 faded);
void FadeIn(void);
void FadeOut(void);

#endif // GUARD_FADE_H
