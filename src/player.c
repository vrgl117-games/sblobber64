#include "player.h"

#include <stdbool.h>
#include <string.h>

#include "dfs.h"
#include "map.h"
#include "rdp.h"

extern char map[MAP_HEIGHT][MAP_WIDTH];

player_t player = {0, 0, 0, 0};
static sprite_t *body[16] = {0};

static inline bool isPlayer(uint8_t x, uint8_t y)
{
    return ((y == player.y || y == player.y - player.h_of || y == player.y + player.h_of) && (x == player.x || x == player.x - player.w_of || x == player.x + player.w_of));
}

void player_init()
{
    body[0] = dfs_load_sprite("/gfx/sprites/slime/tile_single_0.sprite");
    body[1] = dfs_load_sprite("/gfx/sprites/slime/tile_ver_0.sprite");
    body[2] = dfs_load_sprite("/gfx/sprites/slime/tile_ver_1.sprite");
    body[3] = dfs_load_sprite("/gfx/sprites/slime/tile_ver_2.sprite");
    body[4] = dfs_load_sprite("/gfx/sprites/slime/tile_hor_0.sprite");
    body[5] = dfs_load_sprite("/gfx/sprites/slime/tile_hor_1.sprite");
    body[6] = dfs_load_sprite("/gfx/sprites/slime/tile_hor_2.sprite");
    body[7] = dfs_load_sprite("/gfx/sprites/slime/tile_sq_0.sprite");
    body[8] = dfs_load_sprite("/gfx/sprites/slime/tile_sq_1.sprite");
    body[9] = dfs_load_sprite("/gfx/sprites/slime/tile_sq_2.sprite");
    body[10] = dfs_load_sprite("/gfx/sprites/slime/tile_sq_3.sprite");
    body[11] = dfs_load_sprite("/gfx/sprites/slime/tile_sq_4.sprite");
    body[12] = dfs_load_sprite("/gfx/sprites/slime/tile_sq_5.sprite");
    body[13] = dfs_load_sprite("/gfx/sprites/slime/tile_sq_6.sprite");
    body[14] = dfs_load_sprite("/gfx/sprites/slime/tile_sq_7.sprite");
    body[15] = dfs_load_sprite("/gfx/sprites/slime/tile_sq_8.sprite");

    player_reset();
}

void player_draw()
{
    if (player.h_of == 0 && player.w_of == 0)
    {
        rdp_draw_sprite_with_texture(body[0], MAP_CELL_SIZE * player.x, MAP_CELL_SIZE * player.y, 0);
    }
    else if (player.h_of == 1 && player.w_of == 0)
    {
        rdp_draw_sprite_with_texture(body[1], MAP_CELL_SIZE * player.x, MAP_CELL_SIZE * (player.y - 1), 0);
        rdp_draw_sprite_with_texture(body[2], MAP_CELL_SIZE * player.x, MAP_CELL_SIZE * player.y, 0);
        rdp_draw_sprite_with_texture(body[3], MAP_CELL_SIZE * player.x, MAP_CELL_SIZE * (player.y + 1), 0);
    }
    else if (player.h_of == 0 && player.w_of == 1)
    {
        rdp_draw_sprite_with_texture(body[4], MAP_CELL_SIZE * (player.x - 1), MAP_CELL_SIZE * player.y, 0);
        rdp_draw_sprite_with_texture(body[5], MAP_CELL_SIZE * player.x, MAP_CELL_SIZE * player.y, 0);
        rdp_draw_sprite_with_texture(body[6], MAP_CELL_SIZE * (player.x + 1), MAP_CELL_SIZE * player.y, 0);
    }
    else
    {
        rdp_draw_sprite_with_texture(body[7], MAP_CELL_SIZE * (player.x - 1), MAP_CELL_SIZE * (player.y - 1), 0);
        rdp_draw_sprite_with_texture(body[8], MAP_CELL_SIZE * player.x, MAP_CELL_SIZE * (player.y - 1), 0);
        rdp_draw_sprite_with_texture(body[9], MAP_CELL_SIZE * (player.x + 1), MAP_CELL_SIZE * (player.y - 1), 0);
        rdp_draw_sprite_with_texture(body[10], MAP_CELL_SIZE * (player.x - 1), MAP_CELL_SIZE * player.y, 0);
        rdp_draw_sprite_with_texture(body[11], MAP_CELL_SIZE * player.x, MAP_CELL_SIZE * player.y, 0);
        rdp_draw_sprite_with_texture(body[12], MAP_CELL_SIZE * (player.x + 1), MAP_CELL_SIZE * player.y, 0);
        rdp_draw_sprite_with_texture(body[13], MAP_CELL_SIZE * (player.x - 1), MAP_CELL_SIZE * (player.y + 1), 0);
        rdp_draw_sprite_with_texture(body[14], MAP_CELL_SIZE * player.x, MAP_CELL_SIZE * (player.y + 1), 0);
        rdp_draw_sprite_with_texture(body[15], MAP_CELL_SIZE * (player.x + 1), MAP_CELL_SIZE * (player.y + 1), 0);
    }
}

static inline bool detect_tile(char *tiles)
{
    for (int h = player.y - player.h_of; h <= player.y + player.h_of; h++)
    {
        for (int w = player.x - player.w_of; w <= player.x + player.w_of; w++)
        {
            if (strchr(tiles, map[h][w]))
            {
                return true;
            }
        }
    }
    return false;
}

void player_move(input_t *input)
{
    player_t save_player = player;

    if (input->up)
    {
        if (player.h_of == 0)
        {
            player.h_of = 1;
        }
        player.y -= 1;
    }
    if (input->down)
    {
        if (player.h_of == 1)
        {
            player.h_of = 0;
        }
        player.y += 1;
    }
    if (input->left)
    {
        if (player.w_of == 1)
        {
            player.w_of = 0;
        }
        player.x -= 1;
    }
    if (input->right)
    {
        if (player.w_of == 0)
        {
            player.w_of = 1;
        }
        player.x += 1;
    }

    if (input->up || input->down || input->left || input->right)
    {
        if (detect_tile("-|[]{}y<>^"))
        {
            player.x = save_player.x;
            player.y = save_player.y;
            if (detect_tile("-|[]{}y<>^"))
            {
                player.h_of = save_player.h_of;
                player.w_of = save_player.w_of;
            }
        }
    }
}

void player_reset()
{
    player.h_of = 0;
    player.w_of = 0;
    for (uint8_t y = 0; y < MAP_HEIGHT; y++)
    {
        for (uint8_t x = 0; x < MAP_WIDTH; x++)
        {
            if (map[y][x] == 's')
            {
                player.x = x;
                player.y = y;
            }
        }
    }
}