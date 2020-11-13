#ifndef __SCREENS_H__
#define __SCREENS_H__

#include <libdragon.h>

typedef enum screen
{
    intro,
    title,
    game,
    pause,
    win,
} screen_t;

typedef enum pause_selection
{
    none = -1,
    resume = 0,
    restart = 1,
    quit = 2,
} pause_selection_t;

typedef struct SI_condat input_t;

bool screen_intro(display_context_t disp);
bool screen_game(display_context_t disp, input_t *input);
pause_selection_t screen_pause(display_context_t disp, input_t *input, bool reset);
void screen_title(display_context_t disp);
void screen_timer_title();
bool screen_win(display_context_t disp, input_t *input);

#endif //__SCREENS_H__