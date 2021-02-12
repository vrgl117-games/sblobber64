#include <stdlib.h>

#include "colors.h"
#include "debug.h"
#include "fps.h"
#include "input.h"
#include "map.h"
#include "player.h"
#include "rdp.h"
#include "screens.h"
#include "sounds.h"
#include "ui.h"

#define DISABLE_FPS 1

screen_t screen = game;
screen_t prev_screen; //used in credits to know where to go back to

int main()
{
    init_interrupts();
    display_init(RESOLUTION_640x480, DEPTH_16_BPP, 2, GAMMA_NONE, ANTIALIAS_RESAMPLE);
    dfs_init(DFS_DEFAULT_LOCATION);
    rdp_init();
    rdp_set_default_clipping();
    controller_init();
    timer_init();
    //debug_clear();
    colors_init();
    sound_init();

    map_init();
    player_init();
    ui_init();

    srand(timer_ticks() & 0x7FFFFFFF);

#ifndef DISABLE_FPS
    // 1s
    new_timer(TIMER_TICKS(1000000), TF_CONTINUOUS, fps_timer);
#endif

    // 500ms
    new_timer(TIMER_TICKS(500000), TF_CONTINUOUS, screen_timer_title);

    // 50ms
    new_timer(TIMER_TICKS(50000), TF_CONTINUOUS, input_timer);

    display_context_t disp = 0;

    while (true)
    {
        // get controllers
        input_t input = input_get();

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
                if (identify_accessory(0) == ACCESSORY_RUMBLEPAK && is_memory_expanded())
                {
                    sound_start_music();
                    screen = game;
                }
                else
                    screen = rumble;
            }
            break;
        case rumble:
            if (screen_rumble(disp, &input))
            {
                sound_start_music();
                screen = game;
            }
            break;
        case game: // actual game.
            if (input.start)
            {
                screen = pause;
                sound_pause_music();
                screen_pause(disp, &input, true);
            }
            else
            {
                screen = screen_game(disp, &input);
                if (screen != game)
                    sound_pause_music();
            }
            break;
        case pause: // pause menu.
            switch (screen_pause(disp, &input, false))
            {
            case restart:
                map_layer_reset();
                player_reset_in_map();
            case resume:
                sound_resume_music();
                screen = game;
                break;
            case creds:
                prev_screen = pause;
                screen = credits;
                break;
            case quit:
                sound_resume_music();
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
                    sound_resume_music();
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
                sound_resume_music();
                map_select(0);
                player_reset();
                screen = game;
            }
            break;
        case death_fire:
        case death_grid:
            if (screen_death(disp, &input, screen))
            {
                sound_resume_music();
                map_layer_reset();
                player_reset_in_map();
                screen = game;
            }
            break;
        }

#ifndef DISABLE_FPS
        // increment fps counter
        fps_frame();

        // display fps
        fps_draw(disp);
#endif

        // draw debug
        //debug_draw(disp);

        // update display
        display_show(disp);
    }

    return 0;
}