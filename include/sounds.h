#ifndef __SOUNDS_H__
#define __SOUNDS_H__

#include <libdragon.h>

#define NUM_SFX 3

typedef enum
{
    SFX_KEY,
    SFX_WARP,
    SFX_BUTTON
} sound_t;

typedef enum
{
    STEREO,
    MONO
} sound_mode_t;

typedef enum
{
    VOL_MUSIC_100 = 128,
    VOL_MUSIC_75 = 96,
    VOL_MUSIC_50 = 64,
    VOL_MUSIC_25 = 32,
    VOL_MUSIC_0 = 0
} volume_music_t;

typedef enum
{
    VOL_SFX_100 = 256,
    VOL_SFX_75 = 192,
    VOL_SFX_50 = 128,
    VOL_SFX_25 = 64,
    VOL_SFX_0 = 0
} volume_sfx_t;

void sound_init();
void sound_pause_music();
void sound_resume_music();
void sound_switch_mode();
void sound_start_music();
void sound_switch_volume_music(bool left);
void sound_start_sfx(sound_t sound);
void sound_switch_volume_sfx(bool left);
void sound_update();

#endif //__SOUNDS_H__