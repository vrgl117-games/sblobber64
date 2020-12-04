#ifndef __SCREENS_H__
#define __SCREENS_H__

#include <libdragon.h>

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
} screen_t;

typedef enum pause_selection
{
    none = -1,
    resume = 0,
    restart = 1,
    creds = 2,
    quit = 3,
} pause_selection_t;

typedef struct SI_condat input_t;

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

#endif //__SCREENS_H__