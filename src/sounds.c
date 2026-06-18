#include <stdlib.h>
#include <string.h>

#include "sounds.h"
#include "dfs.h"

#define MUSIC_CHANNEL 0
#define SFX_CHANNEL 1

static wav64_t sfxs[NUM_SFX];
static wav64_t music;
static bool music_started = false;
static bool music_paused = false;
static float music_pause_pos = 0.0f;
sound_mode_t sound_mode = STEREO;
volume_sfx_t volume_sfx = VOL_SFX_100;
volume_music_t volume_music = VOL_MUSIC_75;

static float volume_music_value(void)
{
    return (float)volume_music / (float)VOL_MUSIC_100;
}

static float volume_sfx_value(void)
{
    return (float)volume_sfx / (float)VOL_SFX_100;
}

static void sound_apply_music_volume(void)
{
    float volume = music_paused ? 0.0f : volume_music_value();
    mixer_ch_set_vol(MUSIC_CHANNEL, volume, volume);
}

void sound_init()
{
    audio_init(44100, 4);
    mixer_init(2);
    wav64_init_compression(1);

    wav64_open(&sfxs[SFX_KEY], "rom:/sfx/key.wav64");
    wav64_open(&sfxs[SFX_WARP], "rom:/sfx/warp.wav64");
    wav64_open(&sfxs[SFX_BUTTON], "rom:/sfx/button.wav64");
    wav64_open(&music, "rom:/sfx/music.wav64");
    wav64_set_loop(&music, true);

    mixer_ch_set_vol(SFX_CHANNEL, volume_sfx_value(), volume_sfx_value());
    sound_apply_music_volume();
}

void sound_start_music()
{
    wav64_play(&music, MUSIC_CHANNEL);
    music_started = true;
    music_paused = false;
    music_pause_pos = 0.0f;
    sound_apply_music_volume();
}

void sound_switch_mode()
{
    sound_mode = (sound_mode == MONO ? STEREO : MONO);
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
    sound_apply_music_volume();
}

void sound_start_sfx(sound_t sound)
{
    mixer_ch_play(SFX_CHANNEL, &sfxs[sound].wave);
    mixer_ch_set_vol(SFX_CHANNEL, volume_sfx_value(), volume_sfx_value());
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
    mixer_ch_set_vol(SFX_CHANNEL, volume_sfx_value(), volume_sfx_value());
}

void sound_pause_music()
{
    if (!music_started || music_paused)
        return;

    music_pause_pos = mixer_ch_get_pos(MUSIC_CHANNEL);
    mixer_ch_stop(MUSIC_CHANNEL);
    music_paused = true;
}

void sound_resume_music()
{
    if (!music_started || !music_paused)
        return;

    wav64_play(&music, MUSIC_CHANNEL);
    mixer_ch_set_pos(MUSIC_CHANNEL, music_pause_pos);
    music_paused = false;
    sound_apply_music_volume();
}

inline void sound_update()
{
    while (audio_can_write())
    {
        int16_t *buf = audio_write_begin();
        mixer_poll(buf, audio_get_buffer_length());
        audio_write_end();
    }
}
