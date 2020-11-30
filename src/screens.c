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

static sprites_t *logo;
static sprites_t *take_the_stairs;

void screen_init()
{
    logo = dfs_load_sprites("/gfx/sprites_sets/ui/logo-%d_%d.sprite", NULL);
    take_the_stairs = dfs_load_sprites("/gfx/sprites_sets/ui/take_the_stairs-%d_%d.sprite", NULL);
}

// display the n64 logo and then the vrgl117 games logo.
// return true when the animation is done.
bool screen_intro(display_context_t disp)
{
    static int anim = 0;

    rdp_attach(disp);

    rdp_draw_filled_fullscreen(colors[COLOR_BLACK]);

    rdp_detach_display();
    sprite_t *intro = NULL;

    switch (anim)
    {
    case 1 ... 9:
        intro = dfs_load_spritef("/gfx/sprites/intro/n64brew_jam_logo_%d.sprite", tick);
        break;
    case 10 ... 30:
        intro = dfs_load_sprite("/gfx/sprites/intro/n64brew_jam_logo.sprite");
        break;
    case 31 ... 39:
        intro = dfs_load_spritef("/gfx/sprites/intro/n64brew_jam_logo_%d.sprite", 40 - tick);
        break;
    case 40:
        logo = dfs_load_sprites("/gfx/sprites_sets/ui/logo-%d_%d.sprite", NULL);
        take_the_stairs = dfs_load_sprites("/gfx/sprites_sets/ui/take_the_stairs-%d_%d.sprite", NULL);
        break;
    case 41 ... 49:
        intro = dfs_load_spritef("/gfx/sprites/intro/vrgl117_logo_%d.sprite", tick - 40);
        break;
    case 50 ... 70:
        intro = dfs_load_sprite("/gfx/sprites/intro/vrgl117_logo.sprite");
        break;
    case 71 ... 79:
        intro = dfs_load_spritef("/gfx/sprites/intro/vrgl117_logo_%d.sprite", 80 - tick);
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

bool screen_game(display_context_t disp, input_t *input)
{
    player_move(input);

    rdp_attach(disp);

    rdp_draw_filled_fullscreen(colors[COLOR_BLACK]);

    uint8_t map_id = map_draw(tick);

    player_draw();

    // display logo on title map
    if (map_id == 0)
    {
        rdp_draw_sprites_with_texture(logo, __width / 2 - logo->width / 2, 32, 0);

        if (tick % 14 < 7)
        {
            rdp_draw_sprites_with_texture(take_the_stairs, __width / 2 - take_the_stairs->width / 2, 352, 0);
        }
    }

    rdp_detach_display();

    // detect if we are on the end
    if (player_detect_tile(TILES_END))
    {
        int8_t new_map_id = map_next();
        // if we were on title and just change map, free sprites
        if (map_id == 0 && new_map_id != map_id)
        {
            dfs_free_sprites(logo);
            dfs_free_sprites(take_the_stairs);
        }
        return new_map_id == -1;
    }

    return false;
}

pause_selection_t screen_pause(display_context_t disp, input_t *input, bool reset)
{
    static int selected = 0;

    if (reset)
        selected = 0;

    if (input->up)
        selected = (selected == 0 ? 2 : selected - 1);
    else if (input->down)
        selected = (selected == 2 ? 0 : selected + 1);

    rdp_attach(disp);

    rdp_draw_filled_fullscreen(colors[COLOR_BLACK]);

    rdp_attach(disp);

    sprite_t *pause_sp = dfs_load_sprite("/gfx/sprites/ui/pause.sprite");
    graphics_draw_sprite(disp, __width / 2 - pause_sp->width / 2, 10, pause_sp);
    free(pause_sp);

    sprite_t *resume_sp = dfs_load_sprite((selected == 0 ? "/gfx/sprites/ui/resume_selected.sprite" : "/gfx/sprites/ui/resume.sprite"));
    graphics_draw_sprite(disp, __width / 2 - resume_sp->width / 2, 200, resume_sp);
    free(resume_sp);
    sprite_t *restart_sp = dfs_load_sprite((selected == 1 ? "/gfx/sprites/ui/restart_selected.sprite" : "/gfx/sprites/ui/restart.sprite"));
    graphics_draw_sprite(disp, __width / 2 - restart_sp->width / 2, 250, restart_sp);
    free(restart_sp);
    sprite_t *quit_sp = dfs_load_sprite((selected == 2 ? "/gfx/sprites/ui/quit_selected.sprite" : "/gfx/sprites/ui/quit.sprite"));
    graphics_draw_sprite(disp, __width / 2 - quit_sp->width / 2, 300, quit_sp);
    free(quit_sp);

    if (input->A)
        return selected;
    if (input->start)
        return resume;
    return -1;
}

void screen_timer_title()
{
    tick++;
}

bool screen_win(display_context_t disp, input_t *input)
{
    rdp_attach(disp);

    rdp_draw_filled_fullscreen(colors[COLOR_BLACK]);

    rdp_attach(disp);

    sprite_t *you_win_sp = dfs_load_sprite("/gfx/sprites/ui/you_win.sprite");
    graphics_draw_sprite(disp, __width / 2 - you_win_sp->width / 2, 40, you_win_sp);
    free(you_win_sp);

    sprite_t *restart_sp = dfs_load_sprite("/gfx/sprites/ui/restart_selected.sprite");
    graphics_draw_sprite(disp, __width / 2 - restart_sp->width / 2, 250, restart_sp);
    free(restart_sp);

    return (input->A || input->start);
}
