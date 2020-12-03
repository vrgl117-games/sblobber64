#include "ui.h"

#include <stdint.h>

#include "player.h"
#include "rdp.h"

extern player_t player;

static sprite_t *heart_full;
static sprite_t *heart_empty;

void ui_init()
{
    heart_full = dfs_load_sprite("/gfx/sprites/ui/heart_full.sprite");
    heart_empty = dfs_load_sprite("/gfx/sprites/ui/heart_empty.sprite");
}

void ui_draw()
{
    rdp_draw_sprite_with_texture(heart_full, 10, 10, 0);
    rdp_draw_sprite_with_texture(player.lives > 1 ? heart_full : heart_empty, 10 + UI_LIVES_SIZE + 2, 10, 0);
    rdp_draw_sprite_with_texture(player.lives > 2 ? heart_full : heart_empty, 10 + (UI_LIVES_SIZE + 2) * 2, 10, 0);
}