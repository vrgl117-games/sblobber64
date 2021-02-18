#ifndef __SCREENS_H__
#define __SCREENS_H__

#include <libdragon.h>

#include "input.h"

typedef enum screen
{
    intro,
    rumble,
    game,
    pause,
    death_fire,
    death_grid,
    win,
    credits,
    game_over,
    options,
} screen_t;

typedef enum pause_selection
{
    pause_none = -1,
    pause_resume = 0,
    pause_options = 1,
    pause_restart = 2,
    pause_credits = 3,
    pause_quit = 4,
} pause_selection_t;

bool screen_credits(display_context_t disp, input_t *input);
void screen_load_title_resources();
bool screen_intro(display_context_t disp);
screen_t screen_game(display_context_t disp, input_t *input);
pause_selection_t screen_pause(display_context_t disp, input_t *input, bool reset);
void screen_timer_title();
bool screen_win(display_context_t disp, input_t *input);
bool screen_death(display_context_t disp, input_t *input, screen_t reason);
bool screen_game_over(display_context_t disp, input_t *input);
bool screen_rumble(display_context_t disp, input_t *input);
bool screen_options(display_context_t disp, input_t *input);

#endif //__SCREENS_H__