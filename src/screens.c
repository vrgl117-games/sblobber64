#include <stdlib.h>

#include "colors.h"
#include "dfs.h"
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
        graphics_draw_sprite(disp, __width / 2 - intro->width / 2, __height / 2 - intro->height / 2, intro);
        free(intro);
    }

    anim++;
    return (anim >= 82);
}

void screen_game(display_context_t disp, input_t *input)
{
    rdp_attach(disp);

    rdp_draw_filled_fullscreen(colors[COLOR_BG]);

    //single
    sprite_t *slime = dfs_load_sprite("/gfx/sprites/slime/tile_single_0.sprite");
    rdp_draw_sprite_with_texture(slime, 32 * 4, 32 * 4, 0);
    free(slime);

    //ver
    slime = dfs_load_sprite("/gfx/sprites/slime/tile_ver_0.sprite");
    rdp_draw_sprite_with_texture(slime, 32 * 4, 32 * 6, 0);
    free(slime);
    slime = dfs_load_sprite("/gfx/sprites/slime/tile_ver_1.sprite");
    rdp_draw_sprite_with_texture(slime, 32 * 4, 32 * 7, 0);
    free(slime);
    slime = dfs_load_sprite("/gfx/sprites/slime/tile_ver_2.sprite");
    rdp_draw_sprite_with_texture(slime, 32 * 4, 32 * 8, 0);
    free(slime);

    //hor
    slime = dfs_load_sprite("/gfx/sprites/slime/tile_hor_0.sprite");
    rdp_draw_sprite_with_texture(slime, 32 * 6, 32 * 4, 0);
    free(slime);
    slime = dfs_load_sprite("/gfx/sprites/slime/tile_hor_1.sprite");
    rdp_draw_sprite_with_texture(slime, 32 * 7, 32 * 4, 0);
    free(slime);
    slime = dfs_load_sprite("/gfx/sprites/slime/tile_hor_2.sprite");
    rdp_draw_sprite_with_texture(slime, 32 * 8, 32 * 4, 0);
    free(slime);

    //sq
    slime = dfs_load_sprite("/gfx/sprites/slime/tile_sq_0.sprite");
    rdp_draw_sprite_with_texture(slime, 32 * 6, 32 * 6, 0);
    free(slime);
    slime = dfs_load_sprite("/gfx/sprites/slime/tile_sq_1.sprite");
    rdp_draw_sprite_with_texture(slime, 32 * 7, 32 * 6, 0);
    free(slime);
    slime = dfs_load_sprite("/gfx/sprites/slime/tile_sq_2.sprite");
    rdp_draw_sprite_with_texture(slime, 32 * 8, 32 * 6, 0);
    free(slime);
    slime = dfs_load_sprite("/gfx/sprites/slime/tile_sq_3.sprite");
    rdp_draw_sprite_with_texture(slime, 32 * 6, 32 * 7, 0);
    free(slime);
    slime = dfs_load_sprite("/gfx/sprites/slime/tile_sq_4.sprite");
    rdp_draw_sprite_with_texture(slime, 32 * 7, 32 * 7, 0);
    free(slime);
    slime = dfs_load_sprite("/gfx/sprites/slime/tile_sq_5.sprite");
    rdp_draw_sprite_with_texture(slime, 32 * 8, 32 * 7, 0);
    free(slime);
    slime = dfs_load_sprite("/gfx/sprites/slime/tile_sq_6.sprite");
    rdp_draw_sprite_with_texture(slime, 32 * 6, 32 * 8, 0);
    free(slime);
    slime = dfs_load_sprite("/gfx/sprites/slime/tile_sq_7.sprite");
    rdp_draw_sprite_with_texture(slime, 32 * 7, 32 * 8, 0);
    free(slime);
    slime = dfs_load_sprite("/gfx/sprites/slime/tile_sq_8.sprite");
    rdp_draw_sprite_with_texture(slime, 32 * 8, 32 * 8, 0);
    free(slime);

    //walls
    sprite_t *wall = dfs_load_sprite("/gfx/sprites/map/tile_w_tl.sprite");
    rdp_draw_sprite_with_texture(wall, 32 * 2, 32 * 2, 0);
    free(wall);
    wall = dfs_load_sprite("/gfx/sprites/map/tile_w_tr.sprite");
    rdp_draw_sprite_with_texture(wall, 32 * 17, 32 * 2, 0);
    free(wall);
    wall = dfs_load_sprite("/gfx/sprites/map/tile_w_bl.sprite");
    rdp_draw_sprite_with_texture(wall, 32 * 2, 32 * 12, 0);
    free(wall);
    wall = dfs_load_sprite("/gfx/sprites/map/tile_w_br.sprite");
    rdp_draw_sprite_with_texture(wall, 32 * 17, 32 * 12, 0);
    free(wall);
    wall = dfs_load_sprite("/gfx/sprites/map/tile_w_hor.sprite");
    for (int x = 3; x < 17; x++)
    {
        rdp_draw_sprite_with_texture(wall, 32 * x, 32 * 2, 0);
        rdp_draw_sprite_with_texture(wall, 32 * x, 32 * 12, 0);
    }
    free(wall);
    wall = dfs_load_sprite("/gfx/sprites/map/tile_w_ver.sprite");
    for (int y = 3; y < 12; y++)
    {
        rdp_draw_sprite_with_texture(wall, 32 * 2, 32 * y, 0);
        rdp_draw_sprite_with_texture(wall, 32 * 17, 32 * y, 0);
    }
    free(wall);

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

    sprite_t *logo = dfs_load_sprite("/gfx/sprites/logo.sprite");
    graphics_draw_sprite(disp, __width / 2 - logo->width / 2, 10, logo);
    free(logo);

    // draw only press start half of the time (blink).
    if (tick % 14 < 7)
    {
        sprite_t *press_start = dfs_load_sprite("/gfx/sprites/press_start.sprite");
        graphics_draw_sprite(disp, __width / 2 - press_start->width / 2, 340, press_start);
        free(press_start);
    }
}
