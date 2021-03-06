#include <stdlib.h>

#include "colors.h"
#include "dfs.h"
#include "map.h"
#include "player.h"
#include "rdp.h"
#include "screens.h"
#include "sounds.h"
#include "ui.h"

extern uint32_t __width;
extern uint32_t __height;
extern uint32_t colors[];
extern sprite_t *tiles[255];
extern player_t player;
extern map_t *map;
extern sound_mode_t sound_mode;
extern volume_sfx_t volume_sfx;
extern volume_music_t volume_music;

static volatile int tick = 0;

static sprites_t *take_the_stairs;

// load some sprites in memory
void screen_load_title_resources()
{
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
        intro = dfs_load_spritef("/gfx/sprites/intro/n64brew_jam_logo_%d.sprite", anim);
        break;
    case 10 ... 30:
        intro = dfs_load_sprite("/gfx/sprites/intro/n64brew_jam_logo.sprite");
        break;
    case 31 ... 39:
        intro = dfs_load_spritef("/gfx/sprites/intro/n64brew_jam_logo_%d.sprite", 40 - anim);
        break;
    case 40:
        screen_load_title_resources();
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

// main screen for the game
screen_t screen_game(display_context_t disp, input_t *input)
{
    char on = player_move(input);
    if (on == TILES_FIRE[0] || on == TILES_GRID[0])
    {
        if (player.lives == 0)
            return game_over;
        return (on == TILES_FIRE[0] ? death_fire : death_grid);
    }

    rdp_attach(disp);

    rdp_draw_filled_fullscreen(colors[COLOR_BLACK]);

    uint8_t map_id = map_draw(tick);

    player_draw();

    if (map->anim == MAP_NUM_ANIMS)
    {
        // display text on map
        if (map->txt)
            rdp_draw_sprites_with_texture(map->txt, __width / 2 - map->txt->width / 2, 32, 0);
        // display blinking text on title map
        if (map_id == 0)
        {
            if (tick % 2 < 1)
            {
                rdp_draw_sprites_with_texture(take_the_stairs, __width / 2 - take_the_stairs->width / 2, 352, 0);
                rdp_draw_sprite_with_texture(tiles['#'], 464, 398, 0);
            }
        }
        else
            ui_draw();
    }

    rdp_detach_display();

    // detect if we are on the end
    if (on == TILES_END[0])
    {
        if (map->anim_direction != -1)
        {
            map->anim_direction = -1;
            if (map->id == 0) // free sprites if we leaving title
            {
                dfs_free_sprites(take_the_stairs);
            }
        }
        else if (map->anim == 0)
        {
            if (!map_select(map->id + 1)) // next map can't be loaded means we were on the last one, win the game
                return win;
            return game;
        }
    }

    return game;
}

// pause menu
pause_selection_t screen_pause(display_context_t disp, input_t *input, bool reset)
{
    static uint8_t selected = 0;

    if (reset)
        selected = 0;

    if (input->up)
        selected = (selected == 0 ? (map->id ? 4 : 3) : selected - 1);
    else if (input->down)
        selected = (selected == (map->id ? 4 : 3) ? 0 : selected + 1);

    rdp_attach(disp);

    rdp_draw_filled_fullscreen(colors[COLOR_BG]);

    rdp_detach_display();

    sprite_t *pause_sp = dfs_load_sprite("/gfx/sprites/ui/pause.sprite");
    graphics_draw_sprite(disp, __width / 2 - pause_sp->width / 2, 10, pause_sp);
    free(pause_sp);

    sprite_t *resume_sp = dfs_load_sprite((selected == 0 ? "/gfx/sprites/ui/resume_selected.sprite" : "/gfx/sprites/ui/resume.sprite"));
    graphics_draw_sprite(disp, __width / 2 - resume_sp->width / 2, 180, resume_sp);
    free(resume_sp);
    sprite_t *options_sp = dfs_load_sprite((selected == 1 ? "/gfx/sprites/ui/options_selected.sprite" : "/gfx/sprites/ui/options.sprite"));
    graphics_draw_sprite(disp, __width / 2 - options_sp->width / 2, 230, options_sp);
    free(options_sp);
    sprite_t *restart_sp = dfs_load_sprite((selected == 2 ? "/gfx/sprites/ui/restart_selected.sprite" : "/gfx/sprites/ui/restart.sprite"));
    graphics_draw_sprite(disp, __width / 2 - restart_sp->width / 2, 280, restart_sp);
    free(restart_sp);
    sprite_t *credits_sp = dfs_load_sprite((selected == 3 ? "/gfx/sprites/ui/credits_selected.sprite" : "/gfx/sprites/ui/credits.sprite"));
    graphics_draw_sprite(disp, __width / 2 - credits_sp->width / 2, 330, credits_sp);
    free(credits_sp);
    if (map->id) // do not display quit on title
    {
        sprite_t *quit_sp = dfs_load_sprite((selected == 4 ? "/gfx/sprites/ui/quit_selected.sprite" : "/gfx/sprites/ui/quit.sprite"));
        graphics_draw_sprite(disp, __width / 2 - quit_sp->width / 2, 380, quit_sp);
        free(quit_sp);
    }

    if (input->A)
        return selected;
    if (input->start)
        return pause_resume;
    return pause_none;
}

void screen_timer_title()
{
    tick++;
}

// end game screen
bool screen_win(display_context_t disp, input_t *input)
{
    rdp_attach(disp);

    rdp_draw_filled_fullscreen(colors[COLOR_BG]);

    sprite_t *slime_g = dfs_load_sprite("/gfx/sprites/slime/tile_single_0.sprite");
    sprite_t *slime_y = dfs_load_sprite("/gfx/sprites/map/tile_end.sprite");
    rdp_draw_sprite_with_texture(slime_y, __width / 2 - 32, 200, 0);
    rdp_draw_sprite_with_texture(slime_g, __width / 2, 200, 0);
    free(slime_g);
    free(slime_y);

    rdp_detach_display();

    sprite_t *you_win_sp = dfs_load_sprite("/gfx/sprites/ui/you_win.sprite");
    graphics_draw_sprite(disp, __width / 2 - you_win_sp->width / 2, 40, you_win_sp);
    free(you_win_sp);

    sprite_t *thanks_sp = dfs_load_sprite("/gfx/sprites/ui/thanks.sprite");
    graphics_draw_sprite(disp, __width / 2 - thanks_sp->width / 2, 280, thanks_sp);
    free(thanks_sp);

    sprite_t *credits_sp = dfs_load_sprite("/gfx/sprites/ui/credits_selected.sprite");
    graphics_draw_sprite(disp, __width / 2 - credits_sp->width / 2, 360, credits_sp);
    free(credits_sp);

    return (input->A || input->start);
}

// player death screen
bool screen_death(display_context_t disp, input_t *input, screen_t reason)
{
    // animate tiles every other tick
    mirror_t mirror = tick % 2 < 1;

    sprite_t *danger_sp = dfs_load_sprite(reason == death_fire ? "/gfx/sprites/map/tile_f.sprite" : "/gfx/sprites/map/tile_g.sprite");

    rdp_attach(disp);

    rdp_draw_filled_fullscreen(colors[COLOR_BG]);
    rdp_draw_sprite_with_texture(danger_sp, __width / 2 - 16, 200, mirror);
    rdp_draw_sprite_with_texture(danger_sp, __width / 2 - 48, 232, mirror);
    rdp_draw_sprite_with_texture(danger_sp, __width / 2 + 16, 232, mirror);
    rdp_draw_sprite_with_texture(danger_sp, __width / 2 - 16, 264, mirror);

    rdp_detach_display();

    free(danger_sp);

    sprite_t *reason_sp = dfs_load_sprite(reason == death_fire ? "/gfx/sprites/ui/fire.sprite" : "/gfx/sprites/ui/grid.sprite");
    graphics_draw_sprite(disp, __width / 2 - reason_sp->width / 2, 100, reason_sp);
    free(reason_sp);

    sprite_t *restart_sp = dfs_load_sprite("/gfx/sprites/ui/restart_selected.sprite");
    graphics_draw_sprite(disp, __width / 2 - restart_sp->width / 2, 350, restart_sp);
    free(restart_sp);

    return (input->A || input->start);
}

// game over screen
bool screen_game_over(display_context_t disp, input_t *input)
{
    sprite_t *heart_sp = dfs_load_sprite("/gfx/sprites/map/tile_h_empty.sprite");

    rdp_attach(disp);

    rdp_draw_filled_fullscreen(colors[COLOR_BG]);
    rdp_draw_sprite_with_texture(heart_sp, __width / 2 - 48 - 2, 200, 0);
    rdp_draw_sprite_with_texture(heart_sp, __width / 2 - 16, 200, 0);
    rdp_draw_sprite_with_texture(heart_sp, __width / 2 + 16 + 2, 200, 0);

    rdp_detach_display();

    free(heart_sp);

    sprite_t *game_over_sp = dfs_load_sprite("/gfx/sprites/ui/game_over.sprite");
    graphics_draw_sprite(disp, __width / 2 - game_over_sp->width / 2, 10, game_over_sp);
    free(game_over_sp);

    sprite_t *quit_sp = dfs_load_sprite("/gfx/sprites/ui/quit_selected.sprite");
    graphics_draw_sprite(disp, __width / 2 - quit_sp->width / 2, 350, quit_sp);
    free(quit_sp);

    return (input->A || input->start);
}

// options screen
bool screen_options(display_context_t disp, input_t *input)
{
    static uint8_t selected = 1;

    if (input->up)
        selected = (selected == 1 ? 2 : selected - 1);
    else if (input->down)
        selected = (selected == 2 ? 1 : selected + 1);

    if (selected == 0 && (input->left || input->right))
        sound_switch_mode();

    if (selected == 1 && (input->left || input->right))
        sound_switch_volume_music(input->left);

    if (selected == 2 && (input->left || input->right))
        sound_switch_volume_sfx(input->left);

    rdp_attach(disp);

    rdp_draw_filled_fullscreen(colors[COLOR_BG]);

    rdp_detach_display();

    sprite_t *options_sp = dfs_load_sprite("/gfx/sprites/ui/options_big.sprite");
    graphics_draw_sprite(disp, __width / 2 - options_sp->width / 2, 10, options_sp);
    free(options_sp);

    sprite_t *sound_sp = dfs_load_sprite("/gfx/sprites/ui/sound.sprite");
    graphics_draw_sprite(disp, 150, 180, sound_sp);
    free(sound_sp);

    sprite_t *mode_sp = dfs_load_sprite((sound_mode == STEREO ? (selected == 0 ? "/gfx/sprites/ui/stereo_selected.sprite" : "/gfx/sprites/ui/stereo.sprite") : (selected == 0 ? "/gfx/sprites/ui/mono_selected.sprite" : "/gfx/sprites/ui/mono.sprite")));
    graphics_draw_sprite(disp, __width - 150 - mode_sp->width, 180, mode_sp);
    free(mode_sp);

    sprite_t *music_sp = dfs_load_sprite("/gfx/sprites/ui/music.sprite");
    graphics_draw_sprite(disp, 150, 260, music_sp);
    free(music_sp);

    sprite_t *sound_vol_sp = NULL;
    switch (volume_music)
    {
    case VOL_MUSIC_100:
        sound_vol_sp = dfs_load_sprite(selected == 1 ? "/gfx/sprites/ui/100_selected.sprite" : "/gfx/sprites/ui/100.sprite");
        break;
    case VOL_MUSIC_75:
        sound_vol_sp = dfs_load_sprite(selected == 1 ? "/gfx/sprites/ui/75_selected.sprite" : "/gfx/sprites/ui/75.sprite");
        break;
    case VOL_MUSIC_50:
        sound_vol_sp = dfs_load_sprite(selected == 1 ? "/gfx/sprites/ui/50_selected.sprite" : "/gfx/sprites/ui/50.sprite");
        break;
    case VOL_MUSIC_25:
        sound_vol_sp = dfs_load_sprite(selected == 1 ? "/gfx/sprites/ui/25_selected.sprite" : "/gfx/sprites/ui/25.sprite");
        break;
    case VOL_MUSIC_0:
        sound_vol_sp = dfs_load_sprite(selected == 1 ? "/gfx/sprites/ui/0_selected.sprite" : "/gfx/sprites/ui/0.sprite");
        break;
    }
    graphics_draw_sprite(disp, __width - 150 - sound_vol_sp->width, 260, sound_vol_sp);
    free(sound_vol_sp);

    sprite_t *sfx_sp = dfs_load_sprite("/gfx/sprites/ui/sfx.sprite");
    graphics_draw_sprite(disp, 150, 340, sfx_sp);
    free(sfx_sp);

    switch (volume_sfx)
    {
    case VOL_SFX_100:
        sound_vol_sp = dfs_load_sprite(selected == 2 ? "/gfx/sprites/ui/100_selected.sprite" : "/gfx/sprites/ui/100.sprite");
        break;
    case VOL_SFX_75:
        sound_vol_sp = dfs_load_sprite(selected == 2 ? "/gfx/sprites/ui/75_selected.sprite" : "/gfx/sprites/ui/75.sprite");
        break;
    case VOL_SFX_50:
        sound_vol_sp = dfs_load_sprite(selected == 2 ? "/gfx/sprites/ui/50_selected.sprite" : "/gfx/sprites/ui/50.sprite");
        break;
    case VOL_SFX_25:
        sound_vol_sp = dfs_load_sprite(selected == 2 ? "/gfx/sprites/ui/25_selected.sprite" : "/gfx/sprites/ui/25.sprite");
        break;
    case VOL_SFX_0:
        sound_vol_sp = dfs_load_sprite(selected == 2 ? "/gfx/sprites/ui/0_selected.sprite" : "/gfx/sprites/ui/0.sprite");
        break;
    }
    graphics_draw_sprite(disp, __width - 150 - sound_vol_sp->width, 340, sound_vol_sp);
    free(sound_vol_sp);

    return (input->A || input->start);
}

// credits screen
bool screen_credits(display_context_t disp, input_t *input)
{
    rdp_attach(disp);

    rdp_draw_filled_fullscreen(colors[COLOR_BG]);

    rdp_detach_display();

    sprite_t *credits_sp = dfs_load_sprite("/gfx/sprites/ui/credits_big.sprite");
    graphics_draw_sprite(disp, __width / 2 - credits_sp->width / 2, 10, credits_sp);
    free(credits_sp);

    sprite_t *programming_sp = dfs_load_sprite("/gfx/sprites/ui/programming.sprite");
    graphics_draw_sprite(disp, __width / 2 - programming_sp->width / 2, 160, programming_sp);
    free(programming_sp);
    sprite_t *isabel_victor_sp = dfs_load_sprite("/gfx/sprites/ui/isabel_victor.sprite");
    graphics_draw_sprite(disp, __width / 2 - isabel_victor_sp->width / 2, 210, isabel_victor_sp);
    free(isabel_victor_sp);
    sprite_t *vrgl117games_sp = dfs_load_sprite("/gfx/sprites/ui/vrgl117games.sprite");
    graphics_draw_sprite(disp, __width / 2 - vrgl117games_sp->width / 2, 250, vrgl117games_sp);
    free(vrgl117games_sp);

    sprite_t *art_sp = dfs_load_sprite("/gfx/sprites/ui/art.sprite");
    graphics_draw_sprite(disp, 140 - art_sp->width / 2, 320, art_sp);
    free(art_sp);
    sprite_t *kenney_sp = dfs_load_sprite("/gfx/sprites/ui/kenney.sprite");
    graphics_draw_sprite(disp, 140 - kenney_sp->width / 2, 370, kenney_sp);
    free(kenney_sp);
    sprite_t *kenneynl_sp = dfs_load_sprite("/gfx/sprites/ui/kenneynl.sprite");
    graphics_draw_sprite(disp, 140 - kenneynl_sp->width / 2, 410, kenneynl_sp);
    free(kenneynl_sp);

    sprite_t *music_sp = dfs_load_sprite("/gfx/sprites/ui/music.sprite");
    graphics_draw_sprite(disp, __width - 180 - music_sp->width / 2, 320, music_sp);
    free(music_sp);
    sprite_t *mod_sp = dfs_load_sprite("/gfx/sprites/ui/mod.sprite");
    graphics_draw_sprite(disp, __width - 180 - mod_sp->width / 2, 370, mod_sp);
    free(mod_sp);
    sprite_t *modarchive_sp = dfs_load_sprite("/gfx/sprites/ui/modarchive.sprite");
    graphics_draw_sprite(disp, __width - 180 - modarchive_sp->width / 2, 410, modarchive_sp);
    free(modarchive_sp);

    return (input->A || input->start);
}

// rumble screen
bool screen_rumble(display_context_t disp, input_t *input)
{
    rdp_attach(disp);

    rdp_draw_filled_fullscreen(colors[COLOR_BG]);

    rdp_detach_display();

    sprite_t *pak_sp = dfs_load_sprite(is_memory_expanded() ? "/gfx/sprites/ui/pak_detected.sprite" : "/gfx/sprites/ui/pak_not_detected.sprite");
    graphics_draw_sprite(disp, __width / 2 - pak_sp->width / 2, 80, pak_sp);
    free(pak_sp);

    if (identify_accessory(0) != ACCESSORY_RUMBLEPAK)
    {
        sprite_t *rumble_sp = dfs_load_sprite("/gfx/sprites/ui/rumble.sprite");
        graphics_draw_sprite(disp, __width / 2 - rumble_sp->width / 2, 180, rumble_sp);
        free(rumble_sp);
    }

    sprite_t *continue_sp = dfs_load_sprite("/gfx/sprites/ui/continue.sprite");
    graphics_draw_sprite(disp, __width / 2 - continue_sp->width / 2, 350, continue_sp);
    free(continue_sp);

    return ((identify_accessory(0) == ACCESSORY_RUMBLEPAK && is_memory_expanded()) || input->A || input->start);
}