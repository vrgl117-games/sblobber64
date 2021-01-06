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

void sound_init();
void sound_pause_music();
void sound_resume_music();
void sound_start_music();
void sound_start_sfx(sound_t sound);
void sound_update();

#endif //__SOUNDS_H__