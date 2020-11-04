#ifndef __SCREENS_H__
#define __SCREENS_H__

#include <libdragon.h>

typedef enum screens
{
    intro,
    title,
    game
} screen_t;

bool screen_intro(display_context_t disp);
void screen_timer_title();

#endif //__SCREENS_H__