#include <stdlib.h>

#include "colors.h"
#include "dfs.h"
#include "map.h"
#include "player.h"
#include "rdp.h"
#include "screens.h"

extern uint32_t __width;
extern uint32_t __height;
extern uint32_t colors[];
static volatile int tick = 0;

// display the n64 logo and then the vrgl117 games logo.
// return true when the animation is done.
bool screen_intro(display_context_t disp)
{
    static int anim = 0;

    rdp_attach(disp);

    rdp_draw_filled_fullscreen(0);

    rdp_detach_display();
    sprite_t *intro = NULL;

    switch (anim)
    {
    case 1 ... 9:
        intro = dfs_load_spritef("/gfx/sprites/intro/n64brew_jam_logo_%d.sprite", anim);
        break;
    case 10 ... 30:
        intro = dfs_load_sprite("/gfx/sprites/intro/n64brew_jam_logo.sprite");
        break;
    case 31 ... 39:
        intro = dfs_load_spritef("/gfx/sprites/intro/n64brew_jam_logo_%d.sprite", 40 - anim);
        break;
    case 41 ... 49:
        intro = dfs_load_spritef("/gfx/sprites/intro/vrgl117_logo_%d.sprite", anim - 40);
        break;
    case 50 ... 70:
        intro = dfs_load_sprite("/gfx/sprites/intro/vrgl117_logo.sprite");
        break;
    case 71 ... 79:
        intro = dfs_load_spritef("/gfx/sprites/intro/vrgl117_logo_%d.sprite", 80 - anim);
        break;
    }

    if (intro != NULL)
    {
        graphics_draw_sprite(disp, __width / 2 - intro->width / 2, __height / 2 - intro->height / 2, intro);
        free(intro);
    }

    anim++;
    return (anim >= 82);
}

void screen_game(display_context_t disp, input_t *input)
{
    player_move(input);
    rdp_attach(disp);

    rdp_draw_filled_fullscreen(colors[COLOR_BG]);

    map_draw();
    player_draw();

    rdp_detach_display();
}

void screen_timer_title()
{
    tick++;
}

// display the title screen.
void screen_title(display_context_t disp)
{
    rdp_attach(disp);

    rdp_draw_filled_fullscreen(colors[COLOR_BG]);

    rdp_detach_display();

    sprite_t *logo = dfs_load_sprite("/gfx/sprites/ui/logo.sprite");
    graphics_draw_sprite(disp, __width / 2 - logo->width / 2, 10, logo);
    free(logo);

    // draw only press start half of the time (blink).
    if (tick % 14 < 7)
    {
        sprite_t *press_start = dfs_load_sprite("/gfx/sprites/ui/press_start.sprite");
        graphics_draw_sprite(disp, __width / 2 - press_start->width / 2, 340, press_start);
        free(press_start);
    }
}
