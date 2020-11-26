#include "player.h"

#include <string.h>

#include "dfs.h"
#include "map.h"
#include "rdp.h"
#include "sounds.h"
#include "debug.h"

extern int map_idx;
extern char map[MAP_LAYERS][MAP_HEIGHT][MAP_WIDTH];
extern sprite_t *tiles[255];

player_t player = {0, 0, 0, 0};
static sprite_t *body[22] = {0};

static inline bool isPlayer(uint8_t x, uint8_t y)
{
    return ((y == player.y || y == player.y - player.h_of || y == player.y + player.h_of) && (x == player.x || x == player.x - player.w_of || x == player.x + player.w_of));
}

void player_init()
{
    body[SLIME_SINGLE] = dfs_load_sprite("/gfx/sprites/slime/tile_single_0.sprite");
    body[SLIME_VER_0] = dfs_load_sprite("/gfx/sprites/slime/tile_ver_0.sprite");
    body[SLIME_VER_1] = dfs_load_sprite("/gfx/sprites/slime/tile_ver_1.sprite");
    body[SLIME_VER_2] = dfs_load_sprite("/gfx/sprites/slime/tile_ver_2.sprite");
    body[SLIME_HOR_0] = dfs_load_sprite("/gfx/sprites/slime/tile_hor_0.sprite");
    body[SLIME_HOR_1] = dfs_load_sprite("/gfx/sprites/slime/tile_hor_1.sprite");
    body[SLIME_HOR_2] = dfs_load_sprite("/gfx/sprites/slime/tile_hor_2.sprite");
    body[SLIME_SQ_0] = dfs_load_sprite("/gfx/sprites/slime/tile_sq_0.sprite");
    body[SLIME_SQ_1] = dfs_load_sprite("/gfx/sprites/slime/tile_sq_1.sprite");
    body[SLIME_SQ_2] = dfs_load_sprite("/gfx/sprites/slime/tile_sq_2.sprite");
    body[SLIME_SQ_3] = dfs_load_sprite("/gfx/sprites/slime/tile_sq_3.sprite");
    body[SLIME_SQ_4] = dfs_load_sprite("/gfx/sprites/slime/tile_sq_4.sprite");
    body[SLIME_SQ_5] = dfs_load_sprite("/gfx/sprites/slime/tile_sq_5.sprite");
    body[SLIME_SQ_6] = dfs_load_sprite("/gfx/sprites/slime/tile_sq_6.sprite");
    body[SLIME_SQ_7] = dfs_load_sprite("/gfx/sprites/slime/tile_sq_7.sprite");
    body[SLIME_SQ_8] = dfs_load_sprite("/gfx/sprites/slime/tile_sq_8.sprite");
    body[SLIME_HOR_0_ANIM] = dfs_load_sprite("/gfx/sprites/slime/tile_hor_0_alt.sprite");
    body[SLIME_HOR_1_ANIM] = dfs_load_sprite("/gfx/sprites/slime/tile_hor_1_alt.sprite");
    body[SLIME_SQ_0_ANIM] = dfs_load_sprite("/gfx/sprites/slime/tile_sq_0_alt.sprite");
    body[SLIME_SQ_1_ANIM] = dfs_load_sprite("/gfx/sprites/slime/tile_sq_1_alt.sprite");
    body[SLIME_SQ_4_ANIM] = dfs_load_sprite("/gfx/sprites/slime/tile_sq_4_alt.sprite");
    body[SLIME_SQ_5_ANIM] = dfs_load_sprite("/gfx/sprites/slime/tile_sq_5_alt.sprite");

    player_reset();
}

void player_draw()
{
    uint8_t save_y = player.y;
    uint8_t save_x = player.x;
    if (player.y >= (MAP_HEIGHT - SCREEN_HEIGHT / 2))
    {
        player.y = SCREEN_HEIGHT - (MAP_HEIGHT - player.y);
    }
    else if (player.y >= SCREEN_HEIGHT / 2)
    {
        player.y = SCREEN_HEIGHT / 2;
    }
    if (player.x >= (MAP_WIDTH - SCREEN_WIDTH / 2))
    {
        player.x = SCREEN_WIDTH - (MAP_WIDTH - player.x);
    }
    else if (player.x >= SCREEN_WIDTH / 2)
    {
        player.x = SCREEN_WIDTH / 2;
    }
    if (player.h_of == 0 && player.w_of == 0) // single
    {
        if (player.h_of_anim != 0) // shrinking horizontally
        {
            rdp_draw_sprite_with_texture(body[SLIME_VER_0_ANIM], MAP_CELL_SIZE * player.x, MAP_CELL_SIZE * (player.y - 1), 0);
            rdp_draw_sprite_with_texture(body[SLIME_VER_1_ANIM], MAP_CELL_SIZE * player.x, MAP_CELL_SIZE * player.y, 0);
        }
        else if (player.w_of_anim != 0) // shrinking vertically
        {
            rdp_draw_sprite_with_texture(body[SLIME_HOR_0_ANIM], MAP_CELL_SIZE * player.x, MAP_CELL_SIZE * player.y, 0);
            rdp_draw_sprite_with_texture(body[SLIME_HOR_1_ANIM], MAP_CELL_SIZE * (player.x + 1), MAP_CELL_SIZE * player.y, 0);
        }
        else // full
            rdp_draw_sprite_with_texture(body[SLIME_SINGLE], MAP_CELL_SIZE * player.x, MAP_CELL_SIZE * player.y, 0);
    }
    else if (player.h_of == 1 && player.w_of == 0) // vertical
    {
        if (player.w_of_anim != 0) // shrinking horizontally
        {
            rdp_draw_sprite_with_texture(body[SLIME_SQ_0_ANIM], MAP_CELL_SIZE * player.x, MAP_CELL_SIZE * (player.y - 1), 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_1_ANIM], MAP_CELL_SIZE * (player.x + 1), MAP_CELL_SIZE * (player.y - 1), 0);

            rdp_draw_sprite_with_texture(body[SLIME_SQ_2_ANIM], MAP_CELL_SIZE * player.x, MAP_CELL_SIZE * player.y, 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_3_ANIM], MAP_CELL_SIZE * (player.x + 1), MAP_CELL_SIZE * player.y, 0);

            rdp_draw_sprite_with_texture(body[SLIME_SQ_4_ANIM], MAP_CELL_SIZE * player.x, MAP_CELL_SIZE * (player.y + 1), 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_5_ANIM], MAP_CELL_SIZE * (player.x + 1), MAP_CELL_SIZE * (player.y + 1), 0);
        }
        else if (player.h_of_anim == 0) // full
        {
            rdp_draw_sprite_with_texture(body[SLIME_VER_0], MAP_CELL_SIZE * player.x, MAP_CELL_SIZE * (player.y - 1), 0);
            rdp_draw_sprite_with_texture(body[SLIME_VER_1], MAP_CELL_SIZE * player.x, MAP_CELL_SIZE * player.y, 0);
            rdp_draw_sprite_with_texture(body[SLIME_VER_2], MAP_CELL_SIZE * player.x, MAP_CELL_SIZE * (player.y + 1), 0);
        }
        else // growing vertivally
        {
            rdp_draw_sprite_with_texture(body[SLIME_VER_0_ANIM], MAP_CELL_SIZE * player.x, MAP_CELL_SIZE * player.y, 0);
            rdp_draw_sprite_with_texture(body[SLIME_VER_1_ANIM], MAP_CELL_SIZE * player.x, MAP_CELL_SIZE * (player.y + 1), 0);
        }
    }
    else if (player.h_of == 0 && player.w_of == 1) // horizontal
    {
        if (player.h_of_anim != 0) // shrinking vertically
        {
            rdp_draw_sprite_with_texture(body[SLIME_SQ_0], MAP_CELL_SIZE * (player.x - 1), MAP_CELL_SIZE * (player.y - 1), 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_1], MAP_CELL_SIZE * player.x, MAP_CELL_SIZE * (player.y - 1), 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_2], MAP_CELL_SIZE * (player.x + 1), MAP_CELL_SIZE * (player.y - 1), 0);

            rdp_draw_sprite_with_texture(body[SLIME_SQ_6], MAP_CELL_SIZE * (player.x - 1), MAP_CELL_SIZE * player.y, 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_7], MAP_CELL_SIZE * player.x, MAP_CELL_SIZE * player.y, 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_8], MAP_CELL_SIZE * (player.x + 1), MAP_CELL_SIZE * player.y, 0);
        }
        else if (player.w_of_anim == 0) // full
        {
            rdp_draw_sprite_with_texture(body[SLIME_HOR_0], MAP_CELL_SIZE * (player.x - 1), MAP_CELL_SIZE * player.y, 0);
            rdp_draw_sprite_with_texture(body[SLIME_HOR_1], MAP_CELL_SIZE * player.x, MAP_CELL_SIZE * player.y, 0);
            rdp_draw_sprite_with_texture(body[SLIME_HOR_2], MAP_CELL_SIZE * (player.x + 1), MAP_CELL_SIZE * player.y, 0);
        }
        else // growing horizontally
        {
            rdp_draw_sprite_with_texture(body[SLIME_HOR_0_ANIM], MAP_CELL_SIZE * (player.x - 1), MAP_CELL_SIZE * player.y, 0);
            rdp_draw_sprite_with_texture(body[SLIME_HOR_1_ANIM], MAP_CELL_SIZE * player.x, MAP_CELL_SIZE * player.y, 0);
        }
    }
    else // square
    {
        if (player.h_of_anim != 0) // growing vertically
        {
            rdp_draw_sprite_with_texture(body[SLIME_SQ_0], MAP_CELL_SIZE * (player.x - 1), MAP_CELL_SIZE * player.y, 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_1], MAP_CELL_SIZE * player.x, MAP_CELL_SIZE * player.y, 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_2], MAP_CELL_SIZE * (player.x + 1), MAP_CELL_SIZE * player.y, 0);

            rdp_draw_sprite_with_texture(body[SLIME_SQ_6], MAP_CELL_SIZE * (player.x - 1), MAP_CELL_SIZE * (player.y + 1), 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_7], MAP_CELL_SIZE * player.x, MAP_CELL_SIZE * (player.y + 1), 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_8], MAP_CELL_SIZE * (player.x + 1), MAP_CELL_SIZE * (player.y + 1), 0);
        }
        else if (player.w_of_anim != 0) // growing horizontally
        {
            rdp_draw_sprite_with_texture(body[SLIME_SQ_0_ANIM], MAP_CELL_SIZE * (player.x - 1), MAP_CELL_SIZE * (player.y - 1), 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_1_ANIM], MAP_CELL_SIZE * player.x, MAP_CELL_SIZE * (player.y - 1), 0);

            rdp_draw_sprite_with_texture(body[SLIME_SQ_2_ANIM], MAP_CELL_SIZE * (player.x - 1), MAP_CELL_SIZE * player.y, 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_3_ANIM], MAP_CELL_SIZE * player.x, MAP_CELL_SIZE * player.y, 0);

            rdp_draw_sprite_with_texture(body[SLIME_SQ_4_ANIM], MAP_CELL_SIZE * (player.x - 1), MAP_CELL_SIZE * (player.y + 1), 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_5_ANIM], MAP_CELL_SIZE * player.x, MAP_CELL_SIZE * (player.y + 1), 0);
        }
        else // full
        {
            rdp_draw_sprite_with_texture(body[SLIME_SQ_0], MAP_CELL_SIZE * (player.x - 1), MAP_CELL_SIZE * (player.y - 1), 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_1], MAP_CELL_SIZE * player.x, MAP_CELL_SIZE * (player.y - 1), 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_2], MAP_CELL_SIZE * (player.x + 1), MAP_CELL_SIZE * (player.y - 1), 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_3], MAP_CELL_SIZE * (player.x - 1), MAP_CELL_SIZE * player.y, 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_4], MAP_CELL_SIZE * player.x, MAP_CELL_SIZE * player.y, 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_5], MAP_CELL_SIZE * (player.x + 1), MAP_CELL_SIZE * player.y, 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_6], MAP_CELL_SIZE * (player.x - 1), MAP_CELL_SIZE * (player.y + 1), 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_7], MAP_CELL_SIZE * player.x, MAP_CELL_SIZE * (player.y + 1), 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_8], MAP_CELL_SIZE * (player.x + 1), MAP_CELL_SIZE * (player.y + 1), 0);
        }
    }

    // draw any transparant tile over the player
    for (int y = player.y - player.h_of; y <= player.y + player.h_of; y++)
    {
        for (int x = player.x - player.w_of; x <= player.x + player.w_of; x++)
        {
            if (map[map_idx][y][x] == 'd')
                rdp_draw_sprite_with_texture(tiles[(int)map[map_idx][y][x]], MAP_CELL_SIZE * x, MAP_CELL_SIZE * y, 0);
        }
    }

    if (player.h_of_anim > 0)
        player.h_of_anim--;
    if (player.w_of_anim > 0)
        player.w_of_anim--;
    player.y = save_y;
    player.x = save_x;
}

static inline bool detect_tile(char *tiles)
{
    int found = 0;
    for (int h = player.y - player.h_of; h <= player.y + player.h_of; h++)
    {
        for (int w = player.x - player.w_of; w <= player.x + player.w_of; w++)
        {
            if (strchr(tiles, map[map_idx][h][w]))
            {
                found++;
                if (map[map_idx][h][w] == 'B')
                {
                    if (found == 2)
                        return true;
                }
                else if (map[map_idx][h][w] == 'C')
                {
                    if (found == 3)
                        return true;
                }
                else
                    return true;
            }
        }
    }
    return false;
}

bool player_move(input_t *input)
{
    if (!input->up && !input->down && !input->left && !input->right)
        return false;

    player_t save_player = player;

    if (input->up)
    {
        if (player.h_of == 0)
        {
            player.h_of = 1;
            player.h_of_anim = NUM_ANIMS;
        }
        player.y -= 1;
    }
    else if (input->down)
    {
        if (player.h_of == 1)
        {
            player.h_of = 0;
            player.h_of_anim = NUM_ANIMS;
        }
        player.y += 1;
    }
    else if (input->left)
    {
        if (player.w_of == 1)
        {
            player.w_of = 0;
            player.w_of_anim = NUM_ANIMS;
        }
        player.x -= 1;
    }
    else
    {
        if (player.w_of == 0)
        {
            player.w_of = 1;
            player.w_of_anim = NUM_ANIMS;
        }
        player.x += 1;
    }

    if (detect_tile(TILES_WALL))
    {
        player.x = save_player.x;
        player.y = save_player.y;
        if (detect_tile(TILES_WALL))
        {
            player.h_of = save_player.h_of;
            player.w_of = save_player.w_of;
            player.h_of_anim = 0;
            player.w_of_anim = 0;
        }
    }

    // buttons
    if (detect_tile("ABC"))
    {
        sound_start(0);
        map_next();
    }
    // key & closed door
    if (detect_tile("kD"))
        map_next();

    // end
    return detect_tile("e");
}

void player_reset()
{
    // reset player size to small
    player.h_of = 0;
    player.w_of = 0;

    // set player start position
    for (uint8_t y = 0; y < MAP_HEIGHT; y++)
    {
        for (uint8_t x = 0; x < MAP_WIDTH; x++)
        {
            if (map[map_idx][y][x] == 's')
            {
                player.x = x;
                player.y = y;
            }
        }
    }
}