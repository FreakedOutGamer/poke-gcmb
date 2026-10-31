#include "global.h"
#include "gflib/sound.h"

// coupon.bin has no sound engine, so gflib's sound interface is linked as
// stubs (colbtl's src/gflib/sound.c is the real one, on m4a).

void InitSound(void)
{
}

void PauseSoundVSync(void)
{
}

bool32 UNUSED EnableSoundVSync(bool32 enable)
{
}

void UNUSED PlaySE(u16 song)
{
}

void UNUSED StopSE(u16 song)
{
}

void UNUSED StopAllSound(void)
{
}

bool8 UNUSED ReturnTrue(void)
{
    return TRUE;
}

void UNUSED FadeOutSE(u16 song, u16 _speed)
{
}

void UNUSED SetSEVolume(u16 song, u16 volume)
{
}

void UNUSED SetSETempo(u16 song, u16 tempo)
{
}

void UNUSED SetSEPitch(u16 song, s16 pitch)
{
}

void SoundVSyncOff(void)
{
}

void SoundVSyncOn(void)
{
}
