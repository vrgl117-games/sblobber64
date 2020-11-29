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
    screen_init(); //TODO: remove
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
                map_restart();
                player_reset();
            case resume:
                screen = game;
                break;
            case quit:
                map_reset(0);
                screen = game;
                break;
            default:
                break;
            }
            break;
        case win:
            if (screen_win(disp, &input))
            {
                map_reset(0);
                screen = game;
            }
            break;
        }

        // increment fps counter
        fps_frame();

        // display fps
        fps_draw(disp);

        debug_draw(disp);

        // update display
        display_show(disp);
        sound_update();
    }

    return 0;
}