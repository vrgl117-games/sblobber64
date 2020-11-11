#ifndef __SOUNDS_H__
#define __SOUNDS_H__

#include <libdragon.h>

void sound_init();
void sound_start(int sound);
void sound_stop();
void sound_play_pause();
void sound_update();

#endif //__SOUNDS_H__