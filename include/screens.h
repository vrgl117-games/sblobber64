#ifndef __SCREENS_H__
#define __SCREENS_H__

#include <libdragon.h>

typedef enum screens
{
    intro,
    title,
    game
} screen_t;

typedef struct SI_condat input_t;

bool screen_intro(display_context_t disp);
void screen_game(display_context_t disp, input_t *input);
void screen_title(display_context_t disp);

void screen_timer_title();

#endif //__SCREENS_H__