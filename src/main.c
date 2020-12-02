#include <stdlib.h>

#include "colors.h"
#include "debug.h"
#include "fps.h"
#include "map.h"
#include "player.h"
#include "rdp.h"
#include "screens.h"
#include "sounds.h"

screen_t screen = game; //TODO: set back to intro
screen_t prev_screen;   //used in credits to know where to go back to

int main()
{
    init_interrupts();
    display_init(RESOLUTION_640x480, DEPTH_16_BPP, 2, GAMMA_NONE, ANTIALIAS_RESAMPLE);
    dfs_init(DFS_DEFAULT_LOCATION);
    rdp_init();
    rdp_set_default_clipping();
    controller_init();
    timer_init();
    debug_clear();
    colors_init();
    screen_load_title_resources(); //TODO: remove
    sound_init();

    map_init();
    player_init();

    srand(timer_ticks() & 0x7FFFFFFF);

    new_timer(TIMER_TICKS(1000000), TF_CONTINUOUS, fps_timer);

    new_timer(TIMER_TICKS(50000), TF_CONTINUOUS, screen_timer_title);

    display_context_t disp = 0;

    while (true)
    {
        // get controllers
        controller_scan();
        input_t input = get_keys_down().c[0];

        // update sound
        sound_update();

        // wait for display
        while (!(disp = display_lock()))
            ;

        switch (screen)
        {
        case intro: // n64, n64brew jam and vrgl117 logo.
            if (screen_intro(disp))
                screen = game;
            break;
        case game: // actual game.
            if (input.start)
            {
                screen = pause;
                screen_pause(disp, &input, true);
            }
            else if (screen_game(disp, &input))
                screen = win;
            break;
        case pause: // pause menu.
            switch (screen_pause(disp, &input, false))
            {
            case restart:
                player_reset_in_map();
            case resume:
                screen = game;
                break;
            case creds:
                prev_screen = pause;
                screen = credits;
                break;
            case quit:
                map_select(0);
                screen = game;
                break;
            default:
                break;
            }
            break;
        case win:
            if (screen_win(disp, &input))
            {
                prev_screen = win;
                screen = credits;
            }
            break;
        case credits:
            if (screen_credits(disp, &input))
            {
                if (prev_screen == win)
                {
                    map_select(0);
                    player_reset_in_map();
                    screen_load_title_resources();
                    screen = game;
                }
                else
                {
                    screen = prev_screen;
                }
            }
        }

        // increment fps counter
        fps_frame();

        // display fps
        fps_draw(disp);

        debug_draw(disp);

        // update display
        display_show(disp);
    }

    return 0;
}