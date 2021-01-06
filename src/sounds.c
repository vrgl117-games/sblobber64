#include <stdlib.h>
#include <string.h>

#include <mikmod.h>

#include "sounds.h"
#include "dfs.h"

MIKMODAPI extern UWORD md_mode __attribute__((section(".data")));
MIKMODAPI extern UWORD md_mixfreq __attribute__((section(".data")));

static SAMPLE *sfxs[NUM_SFX] = {0};

void sound_init()
{
    audio_init(44100, 2);

    MikMod_RegisterAllDrivers();
    MikMod_RegisterAllLoaders();

    md_mode |= DMODE_16BITS;
    md_mode |= DMODE_SOFT_MUSIC;
    md_mode |= DMODE_SOFT_SNDFX;

    md_mixfreq = audio_get_frequency();
    MikMod_Init("");
    MikMod_SetNumVoices(-1, NUM_SFX);
    MikMod_EnableOutput();

    sfxs[SFX_KEY] = Sample_Load("rom://sfx/key.wav");
    sfxs[SFX_WARP] = Sample_Load("rom://sfx/warp.wav");
    sfxs[SFX_BUTTON] = Sample_Load("rom://sfx/button.wav");
}

void sound_start_music()
{
    MODULE *music = Player_Load("rom://sfx/music.mod", 256, 0);
    music->wrap = 1;
    music->fadeout = 1;
    audio_write_silence();
    audio_write_silence();
    Player_Start(music);
}

void sound_start_sfx(sound_t sound)
{
    SBYTE voice = Sample_Play(sfxs[sound], 0, 0);
    Voice_SetPanning(voice, PAN_CENTER);
    Voice_SetVolume(voice, 256);
}

void sound_pause_music()
{
    if (!Player_Paused())
        Player_TogglePause();
}

void sound_resume_music()
{
    if (Player_Paused())
        Player_TogglePause();
}

inline void sound_update()
{
    MikMod_Update();
}
