#include <stdlib.h>

#include "colors.h"
#include "debug.h"
#include "fps.h"
#include "map.h"
#include "player.h"
#include "rdp.h"
#include "screens.h"

screen_t screen = title; //TODO: set back to intro

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

        // update game state
        while (!(disp = display_lock()))
            ;

        switch (screen)
        {
        case intro: // n64, n64brew jam and vrgl117 logo.
            if (screen_intro(disp))
            {
                screen = title;
            }
            break;
        case title: // press start.
            screen_title(disp);
            if (input.start)
            {
                screen = game;
            }
            break;
        case game: // actual game.
            screen_game(disp, &input);
            break;
        }

        // increment fps counter
        fps_frame();

        // display fps
        fps_draw(disp);

        // update display
        display_show(disp);
    }

    return 0;
}