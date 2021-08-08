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

    if (player.y >= map->height - SCREEN_HEIGHT_2)
        player.sy = SCREEN_HEIGHT - (map->height - player.y);
    else if (player.y >= SCREEN_HEIGHT_2)
        player.sy = SCREEN_HEIGHT_2;

    if (player.x >= map->width - SCREEN_WIDTH_2)
        player.sx = SCREEN_WIDTH - (map->width - player.x);
    else if (player.x >= SCREEN_WIDTH_2)
        player.sx = SCREEN_WIDTH_2;
}

// warp player to other end of warp
static inline void player_warp(char on_warp)
{
    char warp_end;

    switch (on_warp)
    {
    case 'w':
        warp_end = 'v';
        break;
    case 'v':
        warp_end = 'w';
        break;
    case 't':
        warp_end = 'l';
        break;
    case 'l':
        warp_end = 't';
        break;
    default:
        return;
    }
    for (uint8_t y = 0; y < map->height; y++)
    {
        for (uint8_t x = 0; x < map->width; x++)
        {
            if (map->grid[map->layer_idx][y][x] == warp_end)
            {
                player.y = y;
                player.x = x;
                player_update_screen_coordinates();
                return;
            }
        }
    }
}

// detect if player is on a soecific tile and returns that tile
static inline char player_detect_tile(char *tiles, bool can_open_doors)
{
    int found = 0;
    for (int h = player.y - player.h_of; h <= player.y + player.h_of; h++)
    {
        for (int w = player.x - player.w_of; w <= player.x + player.w_of; w++)
        {
            char on = map->grid[map->layer_idx][h][w];
            if (strchr(tiles, on))
            {
                found++;
                if (on == TILES_BUTTONS[1])
                {
                    if (found == 2)
                        return on;
                }
                else if (on == TILES_BUTTONS[2])
                {
                    if (found == 3)
                        return on;
                }
                else if (strchr(TILES_GRID, on))
                {
                    if (found == player.size)
                        return on;
                }
                else if (on == TILES_HEART[0] && player.lives < PLAYER_MAX_LIVES)
                {
                    // hide heart in all layers
                    for (uint8_t idx = 0; idx <= map->layers - 1; idx++)
                        map->grid[idx][h][w] = ' ';
                    return on;
                }
                else if (strchr(TILES_KEYS, on))
                {
                    // hide key in all layers
                    for (uint8_t idx = 0; idx <= map->layers - 1; idx++)
                        map->grid[idx][h][w] = ' ';
                    return on;
                }
                else if (strchr(TILES_POTIONS, on))
                {
                    // hide potion in all layers
                    for (uint8_t idx = 0; idx <= map->layers - 1; idx++)
                        map->grid[idx][h][w] = -map->grid[idx][h][w];

                    player.x = w;
                    player.y = h;
                    player_update_screen_coordinates();
                    return on;
                }
                else if (strchr(TILES_DOORS_LOCKED, on))
                {
                    uint8_t key_idx = on - TILES_DOORS_LOCKED[0];

                    // unlock door in all layers if key present in inventory
                    if (player.inventory.keys[key_idx])
                    {
                        for (uint8_t idx = 0; idx <= map->layers - 1; idx++)
                            map->grid[idx][h][w] = TILES_DOORS_UNLOCKED[0];

                        // remove corresponding key from inventory
                        player.inventory.keys[key_idx] = false;
                        return TILES_DOORS_UNLOCKED[0]; // explicitly return TILES_DOORS_UNLOCKED so we can avoid rumble
                    }
                    return on;
                }
                else if (on == TILES_DOORS_UNLOCKED[0])
                {
                    if (can_open_doors)
                    {
                        // open door in all layers
                        for (uint8_t idx = 0; idx <= map->layers - 1; idx++)
                            map->grid[idx][h][w] = TILES_DOORS_OPENED[0];
                    }
                    return on;
                }
                else
                    return on;
            }
        }
    }
    return 0;
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
    body[SLIME_SQ_6] = body[SLIME_SQ_3];
    body[SLIME_SQ_7] = dfs_load_sprite("/gfx/sprites/slime/tile_sq_7.sprite");
    body[SLIME_SQ_8] = body[SLIME_SQ_5];
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
    int8_t rumble_offset = (player.rumble > 0 ? (player.rumble % 3) - 1 : 0);

    if (player.h_of == 0 && player.w_of == 0) // single
    {
        if (player.h_of_anim != 0) // shrinking horizontally
        {
            rdp_draw_sprite_with_texture(body[SLIME_VER_0_ANIM], MAP_CELL_SIZE * player.sx + rumble_offset, MAP_CELL_SIZE * (player.sy - 1), 0);
            rdp_draw_sprite_with_texture(body[SLIME_VER_1_ANIM], MAP_CELL_SIZE * player.sx + rumble_offset, MAP_CELL_SIZE * player.sy, 0);
        }
        else if (player.w_of_anim != 0) // shrinking vertically
        {
            rdp_draw_sprite_with_texture(body[SLIME_HOR_0_ANIM], MAP_CELL_SIZE * player.sx + rumble_offset, MAP_CELL_SIZE * player.sy, 0);
            rdp_draw_sprite_with_texture(body[SLIME_HOR_1_ANIM], MAP_CELL_SIZE * (player.sx + 1) + rumble_offset, MAP_CELL_SIZE * player.sy, 0);
        }
        else // full
            rdp_draw_sprite_with_texture(body[SLIME_SINGLE], MAP_CELL_SIZE * player.sx + rumble_offset, MAP_CELL_SIZE * player.sy, 0);
    }
    else if (player.h_of == 1 && player.w_of == 0) // vertical
    {
        if (player.w_of_anim != 0) // shrinking horizontally
        {
            rdp_draw_sprite_with_texture(body[SLIME_SQ_0_ANIM], MAP_CELL_SIZE * player.sx + rumble_offset, MAP_CELL_SIZE * (player.sy - 1), 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_1_ANIM], MAP_CELL_SIZE * (player.sx + 1) + rumble_offset, MAP_CELL_SIZE * (player.sy - 1), 0);

            rdp_draw_sprite_with_texture(body[SLIME_SQ_2_ANIM], MAP_CELL_SIZE * player.sx + rumble_offset, MAP_CELL_SIZE * player.sy, 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_3_ANIM], MAP_CELL_SIZE * (player.sx + 1) + rumble_offset, MAP_CELL_SIZE * player.sy, 0);

            rdp_draw_sprite_with_texture(body[SLIME_SQ_4_ANIM], MAP_CELL_SIZE * player.sx + rumble_offset, MAP_CELL_SIZE * (player.sy + 1), 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_5_ANIM], MAP_CELL_SIZE * (player.sx + 1) + rumble_offset, MAP_CELL_SIZE * (player.sy + 1), 0);
        }
        else if (player.h_of_anim == 0) // full
        {
            rdp_draw_sprite_with_texture(body[SLIME_VER_0], MAP_CELL_SIZE * player.sx + rumble_offset, MAP_CELL_SIZE * (player.sy - 1), 0);
            rdp_draw_sprite_with_texture(body[SLIME_VER_1], MAP_CELL_SIZE * player.sx + rumble_offset, MAP_CELL_SIZE * player.sy, 0);
            rdp_draw_sprite_with_texture(body[SLIME_VER_2], MAP_CELL_SIZE * player.sx + rumble_offset, MAP_CELL_SIZE * (player.sy + 1), 0);
        }
        else // growing vertivally
        {
            rdp_draw_sprite_with_texture(body[SLIME_VER_0_ANIM], MAP_CELL_SIZE * player.sx + rumble_offset, MAP_CELL_SIZE * player.sy, 0);
            rdp_draw_sprite_with_texture(body[SLIME_VER_1_ANIM], MAP_CELL_SIZE * player.sx + rumble_offset, MAP_CELL_SIZE * (player.sy + 1), 0);
        }
    }
    else if (player.h_of == 0 && player.w_of == 1) // horizontal
    {
        if (player.h_of_anim != 0) // shrinking vertically
        {
            rdp_draw_sprite_with_texture(body[SLIME_SQ_0], MAP_CELL_SIZE * (player.sx - 1) + rumble_offset, MAP_CELL_SIZE * (player.sy - 1), 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_1], MAP_CELL_SIZE * player.sx + rumble_offset, MAP_CELL_SIZE * (player.sy - 1), 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_2], MAP_CELL_SIZE * (player.sx + 1) + rumble_offset, MAP_CELL_SIZE * (player.sy - 1), 0);

            rdp_draw_sprite_with_texture(body[SLIME_SQ_6], MAP_CELL_SIZE * (player.sx - 1) + rumble_offset, MAP_CELL_SIZE * player.sy, 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_7], MAP_CELL_SIZE * player.sx + rumble_offset, MAP_CELL_SIZE * player.sy, 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_8], MAP_CELL_SIZE * (player.sx + 1) + rumble_offset, MAP_CELL_SIZE * player.sy, 0);
        }
        else if (player.w_of_anim == 0) // full
        {
            rdp_draw_sprite_with_texture(body[SLIME_HOR_0], MAP_CELL_SIZE * (player.sx - 1) + rumble_offset, MAP_CELL_SIZE * player.sy, 0);
            rdp_draw_sprite_with_texture(body[SLIME_HOR_1], MAP_CELL_SIZE * player.sx + rumble_offset, MAP_CELL_SIZE * player.sy, 0);
            rdp_draw_sprite_with_texture(body[SLIME_HOR_2], MAP_CELL_SIZE * (player.sx + 1) + rumble_offset, MAP_CELL_SIZE * player.sy, 0);
        }
        else // growing horizontally
        {
            rdp_draw_sprite_with_texture(body[SLIME_HOR_0_ANIM], MAP_CELL_SIZE * (player.sx - 1) + rumble_offset, MAP_CELL_SIZE * player.sy, 0);
            rdp_draw_sprite_with_texture(body[SLIME_HOR_1_ANIM], MAP_CELL_SIZE * player.sx + rumble_offset, MAP_CELL_SIZE * player.sy, 0);
        }
    }
    else // square
    {
        if (player.h_of_anim != 0) // growing vertically
        {
            rdp_draw_sprite_with_texture(body[SLIME_SQ_0], MAP_CELL_SIZE * (player.sx - 1) + rumble_offset, MAP_CELL_SIZE * player.sy, 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_1], MAP_CELL_SIZE * player.sx + rumble_offset, MAP_CELL_SIZE * player.sy, 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_2], MAP_CELL_SIZE * (player.sx + 1) + rumble_offset, MAP_CELL_SIZE * player.sy, 0);

            rdp_draw_sprite_with_texture(body[SLIME_SQ_6], MAP_CELL_SIZE * (player.sx - 1) + rumble_offset, MAP_CELL_SIZE * (player.sy + 1), 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_7], MAP_CELL_SIZE * player.sx + rumble_offset, MAP_CELL_SIZE * (player.sy + 1), 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_8], MAP_CELL_SIZE * (player.sx + 1) + rumble_offset, MAP_CELL_SIZE * (player.sy + 1), 0);
        }
        else if (player.w_of_anim != 0) // growing horizontally
        {
            rdp_draw_sprite_with_texture(body[SLIME_SQ_0_ANIM], MAP_CELL_SIZE * (player.sx - 1) + rumble_offset, MAP_CELL_SIZE * (player.sy - 1), 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_1_ANIM], MAP_CELL_SIZE * player.sx + rumble_offset, MAP_CELL_SIZE * (player.sy - 1), 0);

            rdp_draw_sprite_with_texture(body[SLIME_SQ_2_ANIM], MAP_CELL_SIZE * (player.sx - 1) + rumble_offset, MAP_CELL_SIZE * player.sy, 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_3_ANIM], MAP_CELL_SIZE * player.sx + rumble_offset, MAP_CELL_SIZE * player.sy, 0);

            rdp_draw_sprite_with_texture(body[SLIME_SQ_4_ANIM], MAP_CELL_SIZE * (player.sx - 1) + rumble_offset, MAP_CELL_SIZE * (player.sy + 1), 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_5_ANIM], MAP_CELL_SIZE * player.sx + rumble_offset, MAP_CELL_SIZE * (player.sy + 1), 0);
        }
        else // full
        {
            rdp_draw_sprite_with_texture(body[SLIME_SQ_0], MAP_CELL_SIZE * (player.sx - 1) + rumble_offset, MAP_CELL_SIZE * (player.sy - 1), 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_1], MAP_CELL_SIZE * player.sx + rumble_offset, MAP_CELL_SIZE * (player.sy - 1), 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_2], MAP_CELL_SIZE * (player.sx + 1) + rumble_offset, MAP_CELL_SIZE * (player.sy - 1), 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_3], MAP_CELL_SIZE * (player.sx - 1) + rumble_offset, MAP_CELL_SIZE * player.sy, 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_4], MAP_CELL_SIZE * player.sx + rumble_offset, MAP_CELL_SIZE * player.sy, 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_5], MAP_CELL_SIZE * (player.sx + 1) + rumble_offset, MAP_CELL_SIZE * player.sy, 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_6], MAP_CELL_SIZE * (player.sx - 1) + rumble_offset, MAP_CELL_SIZE * (player.sy + 1), 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_7], MAP_CELL_SIZE * player.sx + rumble_offset, MAP_CELL_SIZE * (player.sy + 1), 0);
            rdp_draw_sprite_with_texture(body[SLIME_SQ_8], MAP_CELL_SIZE * (player.sx + 1) + rumble_offset, MAP_CELL_SIZE * (player.sy + 1), 0);
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

// move the play if input is given and map is not animating
// if damaged, return the tile causing the damage, otherwise 0
char player_move(input_t *input)
{
    // if screen is black, check early if we happen to be on the end
    if (map->anim == 0)
        return player_detect_tile(TILES_END, false);

    // prevent move during animation
    if (map->anim != SCREEN_WIDTH)
        return 0;

    // return early if no button is pressed
    if (!input->up && !input->down && !input->left && !input->right)
        return 0;

    // use potion on each move
    for (uint8_t idx = 0; idx < INVENTORY_NUM_POTIONS; idx++)
        if (player.inventory.potions[idx] > 0)
            player.inventory.potions[idx]--;

    player_t save_player = player;

    char on_warp = player_detect_tile(TILES_WARP, false);

    if (input->up)
    {
        if (player.inventory.potions[POTION_GREEN] == 0 && player.h_of == 0)
        {
            player.h_of = 1;
            player.size *= 3;
            player.h_of_anim = PLAYER_NUM_ANIMS;
        }
        player.y -= 1;
    }
    else if (input->down)
    {
        if (player.inventory.potions[POTION_GREEN] == 0 && player.h_of == 1)
        {
            player.h_of = 0;
            player.size /= 3;
            player.h_of_anim = PLAYER_NUM_ANIMS;
        }
        player.y += 1;
    }
    else if (input->left)
    {
        if (player.inventory.potions[POTION_RED] == 0 && player.w_of == 1)
        {
            player.w_of = 0;
            player.size /= 3;
            player.w_of_anim = PLAYER_NUM_ANIMS;
        }
        player.x -= 1;
    }
    else
    {
        if (player.inventory.potions[POTION_RED] == 0 && player.w_of == 0)
        {
            player.w_of = 1;
            player.size *= 3;
            player.w_of_anim = PLAYER_NUM_ANIMS;
        }
        player.x += 1;
    }

    char on = player_detect_tile(TILES_WALLS TILES_DOORS_LOCKED, false);
    if (on)
    {
        player.x = save_player.x;
        player.y = save_player.y;
        if (player_detect_tile(TILES_WALLS TILES_DOORS_LOCKED TILES_DOORS_UNLOCKED, false))
        {
            player.h_of = save_player.h_of;
            player.w_of = save_player.w_of;
            player.size = save_player.size;
            player.h_of_anim = 0;
            player.w_of_anim = 0;
        }
        if (on != TILES_DOORS_UNLOCKED[0]) // if on == TILES_DOORS_UNLOCKED means we just unlocked a the door, don't rumble
            player_start_rumble(3);
    }

    player_update_screen_coordinates();

    on = player_detect_tile(TILES_BUTTONS TILES_KEYS TILES_POTIONS TILES_DOORS_UNLOCKED TILES_WARP TILES_DANGER TILES_HEART TILES_END, true);
    switch (on)
    {
    // fire or grids
    case 'f':
        player.lives--;
        player_start_rumble(6);
        return TILES_FIRE[0];
    case 'g':
    case 'm':
    case 'r':
    case 'n':
    case 'q':
    case 'j':
    case 'z':
    case 'o':
    case 'x':
        player.lives--;
        player_start_rumble(6);
        return TILES_GRID[0];

    // heart
    case 'h':
        if (player.lives < PLAYER_MAX_LIVES)
        {
            player.lives++;
            if (player.lives < PLAYER_MAX_LIVES && player_detect_tile(TILES_HEART, false))
                player.lives++;
        }
        break;

    // buttons
    case 'A':
    case 'B':
    case 'C':
        sound_start_sfx(SFX_BUTTON);
        map_layer_next();
        break;

    // keys
    case '0':
    case '1':
    case '2':
        sound_start_sfx(SFX_KEY);
        player.inventory.keys[on - '0'] = true;
        break;

    // potions
    case 'Z':
        if (player.w_of == 1) // if big, shrink down
        {
            player.w_of = 0;
            player.size /= 3;
            player.w_of_anim = PLAYER_NUM_ANIMS - 1;
        }
        player.inventory.potions[POTION_RED] = POTION_EFFECT_DURATION;
        break;
    case 'Y':
        if (player.h_of == 1) // if big, shrink down
        {
            player.h_of = 0;
            player.size /= 3;
            player.h_of_anim = PLAYER_NUM_ANIMS - 1;
        }
        player.inventory.potions[POTION_GREEN] = POTION_EFFECT_DURATION;
        break;

    // warp
    case 'v':
    case 'w':
    case 't':
    case 'l':
        if (!on_warp)
        {
            sound_start_sfx(SFX_WARP);
            player_warp(on);
        }
        break;

    // end
    case 'e':
    case '!':
        return TILES_END[0];
    }

    return 0;
}

// set the player back to the start tile and resize it to small
void player_reset_in_map()
{
    // reset player size to small
    player.h_of = 0;
    player.w_of = 0;
    player.h_of_anim = 0;
    player.w_of_anim = 0;
    player.size = 1;

    // reset potions
    memset(&player.inventory.potions, 0, sizeof(player.inventory.potions));

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
    player_reset_inventory();
}

// inventory reset player
void player_reset_inventory()
{
    // reset player inventory to empty
    memset(&player.inventory, 0, sizeof(player.inventory));
}

void player_start_rumble(uint8_t n)
{
    rumble_start(0);
    player.rumble = n;
}

bool player_stop_rumble()
{
    // rumble already stopped
    if (player.rumble == 0)
        return false;

    player.rumble--;

    return (player.rumble == 0);
}