#include "player.h"

#include <string.h>

#include "dfs.h"
#include "map.h"
#include "rdp.h"
#include "sounds.h"
#include "debug.h"

extern map_t *map;

player_t player = {0};
static sprite_t *body[22] = {0};

// return true if player is on {x,y}
static inline bool isPlayer(uint8_t x, uint8_t y)
{
    return ((y == player.y || y == player.y - player.h_of || y == player.y + player.h_of) && (x == player.x || x == player.x - player.w_of || x == player.x + player.w_of));
}

// update player {x,y} inside the screen
static inline void player_update_screen_coordinates()
{
    player.sy = player.y;
    player.sx = player.x;

    if (player.y >= (map->height - SCREEN_HEIGHT / 2))
        player.sy = SCREEN_HEIGHT - (map->height - player.y);
    else if (player.y >= SCREEN_HEIGHT / 2)
        player.sy = SCREEN_HEIGHT / 2;

    if (player.x >= (map->width - SCREEN_WIDTH / 2))
        player.sx = SCREEN_WIDTH - (map->width - player.x);
    else if (player.x >= SCREEN_WIDTH / 2)
        player.sx = SCREEN_WIDTH / 2;
}

// warp player to other end of warp
static inline void player_warp(char on_warp)
{
    for (uint8_t y = 0; y < map->height; y++)
    {
        for (uint8_t x = 0; x < map->width; x++)
        {
            if (map->grid[map->layer_idx][y][x] == (on_warp == 'w' ? 'v' : 'w'))
            {
                player.y = y;
                player.x = x;
                player_update_screen_coordinates();
                return;
            }
        }
    }
}

// init player "package", load all tiles in memory and place player on start tile in map
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
    player_reset_in_map();
}

// draw player on screen
void player_draw()
{
    if (player.h_of == 0 && player.w_of == 0) // single
    {
        if (player.h_of_anim != 0) // shrinking horizontally
        {
            rdp_draw_sprite_with_texture(body[SLIME_VER_0_ANIM], MAP_CELL_SIZE * player.sx, MAP_CELL_SIZE * (player.sy - 1), 0);
            rdp_draw_sprite_with_texture(body[SLIME_VER_1_ANIM], MAP_CELL_SIZE * player.sx, MAP_CELL_SIZE * player.sy, 0);
        }
        else if (player.w_of_anim != 0) // shrinking vertically
        {
            rdp_draw_sprite_with_texture(body[SLIME_HOR_0_ANIM], MAP_CELL_SIZE * player.sx, MAP_CELL_SIZE * player.sy, 0);
            rdp_draw_sprite_with_texture(body[SLIME_HOR_1_ANIM], MAP_CELL_SIZE * (player.sx + 1), MAP_CELL_SIZE * player.sy, 0);
        }
        else // full
            rdp_draw_sprite_with_texture(body[SLIME_SINGLE], MAP_CELL_SIZE * player.sx, MAP_CELL_SIZE * player.sy, 0);
    }
    else if (player.h_of == 1 && player.w_of == 0) // vertical
    {
        if (player.w_of_anim != 0) // shrinking horizontally
        {
            rdp_draw_sprite_with_texture(body[SLIME_SQ_0_ANIM], MAP_CELL_SIZE * player.sx, MAP_CELL_SIZE * (player.sy - 1), 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_1_ANIM], MAP_CELL_SIZE * (player.sx + 1), MAP_CELL_SIZE * (player.sy - 1), 0);

            rdp_draw_sprite_with_texture(body[SLIME_SQ_2_ANIM], MAP_CELL_SIZE * player.sx, MAP_CELL_SIZE * player.sy, 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_3_ANIM], MAP_CELL_SIZE * (player.sx + 1), MAP_CELL_SIZE * player.sy, 0);

            rdp_draw_sprite_with_texture(body[SLIME_SQ_4_ANIM], MAP_CELL_SIZE * player.sx, MAP_CELL_SIZE * (player.sy + 1), 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_5_ANIM], MAP_CELL_SIZE * (player.sx + 1), MAP_CELL_SIZE * (player.sy + 1), 0);
        }
        else if (player.h_of_anim == 0) // full
        {
            rdp_draw_sprite_with_texture(body[SLIME_VER_0], MAP_CELL_SIZE * player.sx, MAP_CELL_SIZE * (player.sy - 1), 0);
            rdp_draw_sprite_with_texture(body[SLIME_VER_1], MAP_CELL_SIZE * player.sx, MAP_CELL_SIZE * player.sy, 0);
            rdp_draw_sprite_with_texture(body[SLIME_VER_2], MAP_CELL_SIZE * player.sx, MAP_CELL_SIZE * (player.sy + 1), 0);
        }
        else // growing vertivally
        {
            rdp_draw_sprite_with_texture(body[SLIME_VER_0_ANIM], MAP_CELL_SIZE * player.sx, MAP_CELL_SIZE * player.sy, 0);
            rdp_draw_sprite_with_texture(body[SLIME_VER_1_ANIM], MAP_CELL_SIZE * player.sx, MAP_CELL_SIZE * (player.sy + 1), 0);
        }
    }
    else if (player.h_of == 0 && player.w_of == 1) // horizontal
    {
        if (player.h_of_anim != 0) // shrinking vertically
        {
            rdp_draw_sprite_with_texture(body[SLIME_SQ_0], MAP_CELL_SIZE * (player.sx - 1), MAP_CELL_SIZE * (player.sy - 1), 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_1], MAP_CELL_SIZE * player.sx, MAP_CELL_SIZE * (player.sy - 1), 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_2], MAP_CELL_SIZE * (player.sx + 1), MAP_CELL_SIZE * (player.sy - 1), 0);

            rdp_draw_sprite_with_texture(body[SLIME_SQ_6], MAP_CELL_SIZE * (player.sx - 1), MAP_CELL_SIZE * player.sy, 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_7], MAP_CELL_SIZE * player.sx, MAP_CELL_SIZE * player.sy, 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_8], MAP_CELL_SIZE * (player.sx + 1), MAP_CELL_SIZE * player.sy, 0);
        }
        else if (player.w_of_anim == 0) // full
        {
            rdp_draw_sprite_with_texture(body[SLIME_HOR_0], MAP_CELL_SIZE * (player.sx - 1), MAP_CELL_SIZE * player.sy, 0);
            rdp_draw_sprite_with_texture(body[SLIME_HOR_1], MAP_CELL_SIZE * player.sx, MAP_CELL_SIZE * player.sy, 0);
            rdp_draw_sprite_with_texture(body[SLIME_HOR_2], MAP_CELL_SIZE * (player.sx + 1), MAP_CELL_SIZE * player.sy, 0);
        }
        else // growing horizontally
        {
            rdp_draw_sprite_with_texture(body[SLIME_HOR_0_ANIM], MAP_CELL_SIZE * (player.sx - 1), MAP_CELL_SIZE * player.sy, 0);
            rdp_draw_sprite_with_texture(body[SLIME_HOR_1_ANIM], MAP_CELL_SIZE * player.sx, MAP_CELL_SIZE * player.sy, 0);
        }
    }
    else // square
    {
        if (player.h_of_anim != 0) // growing vertically
        {
            rdp_draw_sprite_with_texture(body[SLIME_SQ_0], MAP_CELL_SIZE * (player.sx - 1), MAP_CELL_SIZE * player.sy, 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_1], MAP_CELL_SIZE * player.sx, MAP_CELL_SIZE * player.sy, 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_2], MAP_CELL_SIZE * (player.sx + 1), MAP_CELL_SIZE * player.sy, 0);

            rdp_draw_sprite_with_texture(body[SLIME_SQ_6], MAP_CELL_SIZE * (player.sx - 1), MAP_CELL_SIZE * (player.sy + 1), 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_7], MAP_CELL_SIZE * player.sx, MAP_CELL_SIZE * (player.sy + 1), 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_8], MAP_CELL_SIZE * (player.sx + 1), MAP_CELL_SIZE * (player.sy + 1), 0);
        }
        else if (player.w_of_anim != 0) // growing horizontally
        {
            rdp_draw_sprite_with_texture(body[SLIME_SQ_0_ANIM], MAP_CELL_SIZE * (player.sx - 1), MAP_CELL_SIZE * (player.sy - 1), 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_1_ANIM], MAP_CELL_SIZE * player.sx, MAP_CELL_SIZE * (player.sy - 1), 0);

            rdp_draw_sprite_with_texture(body[SLIME_SQ_2_ANIM], MAP_CELL_SIZE * (player.sx - 1), MAP_CELL_SIZE * player.sy, 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_3_ANIM], MAP_CELL_SIZE * player.sx, MAP_CELL_SIZE * player.sy, 0);

            rdp_draw_sprite_with_texture(body[SLIME_SQ_4_ANIM], MAP_CELL_SIZE * (player.sx - 1), MAP_CELL_SIZE * (player.sy + 1), 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_5_ANIM], MAP_CELL_SIZE * player.sx, MAP_CELL_SIZE * (player.sy + 1), 0);
        }
        else // full
        {
            rdp_draw_sprite_with_texture(body[SLIME_SQ_0], MAP_CELL_SIZE * (player.sx - 1), MAP_CELL_SIZE * (player.sy - 1), 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_1], MAP_CELL_SIZE * player.sx, MAP_CELL_SIZE * (player.sy - 1), 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_2], MAP_CELL_SIZE * (player.sx + 1), MAP_CELL_SIZE * (player.sy - 1), 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_3], MAP_CELL_SIZE * (player.sx - 1), MAP_CELL_SIZE * player.sy, 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_4], MAP_CELL_SIZE * player.sx, MAP_CELL_SIZE * player.sy, 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_5], MAP_CELL_SIZE * (player.sx + 1), MAP_CELL_SIZE * player.sy, 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_6], MAP_CELL_SIZE * (player.sx - 1), MAP_CELL_SIZE * (player.sy + 1), 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_7], MAP_CELL_SIZE * player.sx, MAP_CELL_SIZE * (player.sy + 1), 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_8], MAP_CELL_SIZE * (player.sx + 1), MAP_CELL_SIZE * (player.sy + 1), 0);
        }
    }

    extern sprite_t *tiles[255];

    // draw any transparant tile over the player
    for (int y_of = -player.h_of; y_of <= player.h_of; y_of++)
    {
        for (int x_of = -player.w_of; x_of <= player.w_of; x_of++)
        {
            if (map->grid[map->layer_idx][player.y + y_of][player.x + x_of] == 'd')
                rdp_draw_sprite_with_texture(tiles['E'], MAP_CELL_SIZE * (player.sx + x_of), MAP_CELL_SIZE * (player.sy + y_of), 0);
        }
    }

    if (player.h_of_anim > 0)
        player.h_of_anim--;
    if (player.w_of_anim > 0)
        player.w_of_anim--;
}

// detect it player is on a soecific tile and returns that tile
char player_detect_tile(char *tiles)
{
    int found = 0;
    for (int h = player.y - player.h_of; h <= player.y + player.h_of; h++)
    {
        for (int w = player.x - player.w_of; w <= player.x + player.w_of; w++)
        {
            if (strchr(tiles, map->grid[map->layer_idx][h][w]))
            {
                found++;
                if (map->grid[map->layer_idx][h][w] == 'B')
                {
                    if (found == 2)
                        return map->grid[map->layer_idx][h][w];
                }
                else if (map->grid[map->layer_idx][h][w] == 'C')
                {
                    if (found == 3)
                        return map->grid[map->layer_idx][h][w];
                }
                else if (map->grid[map->layer_idx][h][w] == 'g')
                {
                    if (found == player.size)
                        return map->grid[map->layer_idx][h][w];
                }
                else if (map->grid[map->layer_idx][h][w] == 'h')
                {
                    map->grid[map->layer_idx][h][w] = 0;
                    return 'h';
                }
                else
                    return map->grid[map->layer_idx][h][w];
            }
        }
    }
    return 0;
}

// move the play if input is given and map is not animating
// if damaged, return the tile causing the damage, otherwise 0
char player_move(input_t *input)
{
    // prevent move during animation
    if (map->anim != SCREEN_WIDTH)
        return 0;

    // return early if no button is pressed
    if (!input->up && !input->down && !input->left && !input->right)
        return 0;

    player_t save_player = player;

    char on_warp = player_detect_tile(TILES_WARP);

    if (input->up)
    {
        if (player.h_of == 0)
        {
            player.h_of = 1;
            player.size *= 3;
            player.h_of_anim = PLAYER_NUM_ANIMS;
        }
        player.y -= 1;
    }
    else if (input->down)
    {
        if (player.h_of == 1)
        {
            player.h_of = 0;
            player.size /= 3;
            player.h_of_anim = PLAYER_NUM_ANIMS;
        }
        player.y += 1;
    }
    else if (input->left)
    {
        if (player.w_of == 1)
        {
            player.w_of = 0;
            player.size /= 3;
            player.w_of_anim = PLAYER_NUM_ANIMS;
        }
        player.x -= 1;
    }
    else
    {
        if (player.w_of == 0)
        {
            player.w_of = 1;
            player.size *= 3;
            player.w_of_anim = PLAYER_NUM_ANIMS;
        }
        player.x += 1;
    }

    if (player_detect_tile(TILES_WALL))
    {
        player.x = save_player.x;
        player.y = save_player.y;
        if (player_detect_tile(TILES_WALL))
        {
            player.h_of = save_player.h_of;
            player.w_of = save_player.w_of;
            player.h_of_anim = 0;
            player.w_of_anim = 0;
        }
    }

    // fire or grids
    char danger = player_detect_tile("gf");
    if (danger)
    {
        player.lives--;
        player_update_screen_coordinates();
        return danger;
    }

    // heart
    if (player.lives < PLAYER_MAX_LIVES && player_detect_tile("h"))
    {
        player.lives++;
        if (player.lives < PLAYER_MAX_LIVES && player_detect_tile("h"))
            player.lives++;
    }

    // buttons
    if (player_detect_tile("ABC"))
    {
        sound_start("button");
        map_layer_next();
    }
    // key & closed door
    char kD = player_detect_tile("kD");
    if (kD == 'k')
        sound_start("key");
    if (kD)
        map_layer_next();

    // warp
    if (!on_warp)
    {
        on_warp = player_detect_tile(TILES_WARP);
        if (on_warp)
        {
            sound_start("warp");
            player_warp(on_warp);
        }
    }

    player_update_screen_coordinates();
    return 0;
}

// set the player back to the start tile and resize it to small
void player_reset_in_map()
{
    // reset player size to small
    player.h_of = 0;
    player.w_of = 0;
    player.size = 1;

    // set player start position
    for (uint8_t y = 0; y < map->height; y++)
    {
        for (uint8_t x = 0; x < map->width; x++)
        {
            if (map->grid[0][y][x] == 's' || map->grid[0][y][x] == 'S')
            {
                player.x = x;
                player.y = y;

                player_update_screen_coordinates();
                return;
            }
        }
    }
}

// reset player attributes
void player_reset()
{
    player.lives = PLAYER_MAX_LIVES;
}
