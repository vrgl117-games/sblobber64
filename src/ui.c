#include "ui.h"

#include <stdint.h>

#include "player.h"
#include "rdp.h"

extern player_t player;

static sprite_t *heart_full;
static sprite_t *heart_empty;
static sprite_t *key_sprites[INVENTORY_NUM_KEYS];

void ui_init()
{
    heart_full = dfs_load_sprite("/gfx/sprites/ui/heart_full.sprite");
    heart_empty = dfs_load_sprite("/gfx/sprites/ui/heart_empty.sprite");
    key_sprites[KEY_YELLOW] = dfs_load_sprite("/gfx/sprites/ui/key_yellow.sprite");
    key_sprites[KEY_RED] = dfs_load_sprite("/gfx/sprites/ui/key_red.sprite");
}

void ui_draw()
{
    rdp_draw_sprite_with_texture(heart_full, 10, 10, 0);
    rdp_draw_sprite_with_texture(player.lives > 1 ? heart_full : heart_empty, 10 + UI_ITEM_SIZE + 2, 10, 0);
    rdp_draw_sprite_with_texture(player.lives > 2 ? heart_full : heart_empty, 10 + (UI_ITEM_SIZE + 2) * 2, 10, 0);

    uint8_t offset = 0;
    for (uint8_t idx = 0; idx < INVENTORY_NUM_KEYS; idx++)
    {
        if (player.inventory.keys[idx])
        {
            rdp_draw_sprite_with_texture(key_sprites[idx], 10 + (UI_ITEM_SIZE + 2) * (offset + 4), 10, 0);
            offset++;
        }
    }
}