#include <stdlib.h>

#include "colors.h"
#include "debug.h"
#include "fps.h"
#include "map.h"
#include "player.h"
#include "rdp.h"
#include "screens.h"
#include "sounds.h"
#include "ui.h"

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
    ui_init();

    srand(timer_ticks() & 0x7FFFFFFF);

    // 1s
    new_timer(TIMER_TICKS(1000000), TF_CONTINUOUS, fps_timer);

    // 500ms
    new_timer(TIMER_TICKS(500000), TF_CONTINUOUS, screen_timer_title);

    display_context_t disp = 0;

    while (true)
    {
        // get controllers
        controller_scan();
        input_t input = get_keys_down().c[0];

        // stop rumble if needed.
        if (player_stop_rumble())
            rumble_stop(0);

        // update sound
        sound_update();

        // wait for display
        while (!(disp = display_lock()))
            ;

        switch (screen)
        {
        case intro: // n64, n64brew jam and vrgl117 logo.
            if (screen_intro(disp))
            {
                if (identify_accessory(0) == ACCESSORY_RUMBLEPAK)
                    screen = game;
                else
                    screen = rumble;
            }
            break;
        case rumble:
            if (screen_rumble(disp, &input))
                screen = game;
            break;
        case game: // actual game.
            if (input.start)
            {
                screen = pause;
                screen_pause(disp, &input, true);
            }
            else
                screen = screen_game(disp, &input);
            break;
        case pause: // pause menu.
            switch (screen_pause(disp, &input, false))
            {
            case restart:
                map_layer_reset();
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
                player_reset();
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
                    player_reset();
                    screen = game;
                }
                else
                    screen = prev_screen;
            }
            break;
        case game_over:
            if (screen_game_over(disp, &input))
            {
                map_select(0);
                player_reset();
                screen = game;
            }
            break;
        case death_fire:
        case death_grid:
            if (screen_death(disp, &input, screen))
            {
                map_layer_reset();
                player_reset_in_map();
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
    }

    return 0;
}