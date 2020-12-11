#ifndef __SOUNDS_H__
#define __SOUNDS_H__

#include <libdragon.h>

void sound_init();
void sound_pause_music();
void sound_resume_music();
void sound_start_music();
void sound_start_sfx(char *sound);
void sound_update();

#endif //__SOUNDS_H__