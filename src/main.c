#include <stdlib.h>

#include "rdp.h"
#include "screens.h"

screen_t screen = intro;

int main()
{
    init_interrupts();
    display_init(RESOLUTION_640x480, DEPTH_16_BPP, 2, GAMMA_NONE, ANTIALIAS_RESAMPLE);
    dfs_init(DFS_DEFAULT_LOCATION);
    rdp_init();
    timer_init();

    srand(timer_ticks() & 0x7FFFFFFF);

    new_timer(TIMER_TICKS(50000), TF_CONTINUOUS, screen_timer_title);

    display_context_t disp = 0;

    while (true)
    {
        // get controllers
        // update game state
        while (!(disp = display_lock()))
            ;

        switch (screen)
        {
        case intro: // n64 logo and vrgl117 logo.
            if (screen_intro(disp))
                screen = title;
            break;
        case title: // press start.
            screen = game;
            break;
        case game: // actual game.
            break;
        }
        // update display
        display_show(disp);
    }

    // cleanup, never called
    timer_close();
    rdp_close();
    display_close();
    return 0;
}