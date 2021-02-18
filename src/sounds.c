#include <stdlib.h>
#include <string.h>

#include <mikmod.h>

#include "sounds.h"
#include "dfs.h"

MIKMODAPI extern UWORD md_mode __attribute__((section(".data")));
MIKMODAPI extern UWORD md_mixfreq __attribute__((section(".data")));

static SAMPLE *sfxs[NUM_SFX] = {0};
sound_mode_t sound_mode = STEREO;
volume_sfx_t volume_sfx = VOL_SFX_100;
volume_music_t volume_music = VOL_MUSIC_75;

void sound_init()
{
    audio_init(44100, 2);

    MikMod_RegisterAllDrivers();
    MikMod_RegisterAllLoaders();

    md_mixfreq = audio_get_frequency();
    MikMod_Init("");
    MikMod_SetNumVoices(-1, NUM_SFX);
    MikMod_EnableOutput();

    sfxs[SFX_KEY] = Sample_Load("rom://sfx/key.wav");
    sfxs[SFX_WARP] = Sample_Load("rom://sfx/warp.wav");
    sfxs[SFX_BUTTON] = Sample_Load("rom://sfx/button.wav");
}

static MODULE *music = NULL;
void sound_start_music()
{
    music = Player_Load("rom://sfx/music.mod", 256, 0);
    music->wrap = 1;
    music->fadeout = 1;
    audio_write_silence();
    audio_write_silence();
    Player_Start(music);
    Player_SetVolume(volume_music);
}

void sound_switch_mode()
{
    sound_mode = (sound_mode == MONO ? STEREO : MONO);
    Player_Stop();
    Player_Free(music);
    MikMod_Exit();
    md_mode = DMODE_16BITS | DMODE_SOFT_MUSIC | DMODE_SOFT_SNDFX;
    if (sound_mode == STEREO)
        md_mode |= DMODE_STEREO;
    MikMod_Init("");
    MikMod_SetNumVoices(-1, NUM_SFX);
    MikMod_EnableOutput();
    sound_start_music();
    sound_pause_music();
}

void sound_switch_volume_music(bool left)
{
    switch (volume_music)
    {
    case VOL_MUSIC_100:
        volume_music = (left ? VOL_MUSIC_75 : VOL_MUSIC_0);
        break;
    case VOL_MUSIC_75:
        volume_music = (left ? VOL_MUSIC_50 : VOL_MUSIC_100);
        break;
    case VOL_MUSIC_50:
        volume_music = (left ? VOL_MUSIC_25 : VOL_MUSIC_75);
        break;
    case VOL_MUSIC_25:
        volume_music = (left ? VOL_MUSIC_0 : VOL_MUSIC_50);
        break;
    case VOL_MUSIC_0:
        volume_music = (left ? VOL_MUSIC_100 : VOL_MUSIC_25);
        break;
    }
    Player_SetVolume(volume_music);
}

static SBYTE voice = 0;
void sound_start_sfx(sound_t sound)
{
    voice = Sample_Play(sfxs[sound], 0, 0);
    Voice_SetPanning(voice, PAN_CENTER);
    //Voice_SetVolume(voice, 256);
    Voice_SetVolume(voice, volume_sfx);
}

void sound_switch_volume_sfx(bool left)
{
    switch (volume_sfx)
    {
    case VOL_SFX_100:
        volume_sfx = (left ? VOL_SFX_75 : VOL_SFX_0);
        break;
    case VOL_SFX_75:
        volume_sfx = (left ? VOL_SFX_50 : VOL_SFX_100);
        break;
    case VOL_SFX_50:
        volume_sfx = (left ? VOL_SFX_25 : VOL_SFX_75);
        break;
    case VOL_SFX_25:
        volume_sfx = (left ? VOL_SFX_0 : VOL_SFX_50);
        break;
    case VOL_SFX_0:
        volume_sfx = (left ? VOL_SFX_100 : VOL_SFX_25);
        break;
    }
    Voice_SetVolume(voice, volume_sfx);
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
