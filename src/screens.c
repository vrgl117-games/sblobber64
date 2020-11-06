#include <stdlib.h>

#include "dfs.h"
#include "rdp.h"
#include "screens.h"

static volatile int tick = 0;
static sprites_t *logo;

// load a few sprites in memory
void screen_init()
{
    logo = dfs_load_sprites("/gfx/sprites_sets/logo-%d_%d.sprite", NULL);
}

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
        intro = dfs_load_spritef("/gfx/sprites/n64brew_jam_logo_%d.sprite", anim);
        break;
    case 10 ... 30:
        intro = dfs_load_sprite("/gfx/sprites/n64brew_jam_logo.sprite");
        break;
    case 31 ... 39:
        intro = dfs_load_spritef("/gfx/sprites/n64brew_jam_logo_%d.sprite", 40 - anim);
        break;
    case 41 ... 49:
        intro = dfs_load_spritef("/gfx/sprites/intro_%d.sprite", anim - 40);
        break;
    case 50 ... 70:
        intro = dfs_load_sprite("/gfx/sprites/intro.sprite");
        break;
    case 71 ... 79:
        intro = dfs_load_spritef("/gfx/sprites/intro_%d.sprite", 80 - anim);
        break;
    }

    if (intro != NULL)
    {
        graphics_draw_sprite(disp, 320 - intro->width / 2, 150, intro);
        free(intro);
    }

    anim++;
    return (anim >= 82);
}

void screen_game(display_context_t disp, input_t *input)
{
    rdp_attach(disp);

    rdp_draw_filled_fullscreen(0);

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

    rdp_draw_filled_fullscreen(0);

    rdp_draw_sprites_with_texture(logo, 192, 16, 0);

    rdp_detach_display();
}