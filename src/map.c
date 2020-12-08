#include "map.h"

#include "colors.h"
#include "dfs.h"
#include "player.h"
#include "rdp.h"
#include "sounds.h"

#include "debug.h"

extern uint32_t colors[];
extern player_t player;

map_t *map;

sprite_t *tiles[255] = {0};
char *level_paths[NUM_MAPS] = {
    "/maps/00_title.map",
    "/maps/01_tutorial.map",
    "/maps/02_level1.map",
    "/maps/03_level2.map",
    "/maps/04_maze.map",
    "/maps/05_end.map",

};

// init map "package", load all tiles in memory
void map_init()
{
    //walls
    tiles['{'] = dfs_load_sprite("/gfx/sprites/map/tile_w_tl.sprite");
    tiles['}'] = dfs_load_sprite("/gfx/sprites/map/tile_w_tr.sprite");
    tiles['['] = dfs_load_sprite("/gfx/sprites/map/tile_w_bl.sprite");
    tiles[']'] = dfs_load_sprite("/gfx/sprites/map/tile_w_br.sprite");
    tiles['>'] = dfs_load_sprite("/gfx/sprites/map/tile_w_ml.sprite");
    tiles['<'] = dfs_load_sprite("/gfx/sprites/map/tile_w_mr.sprite");
    tiles['y'] = dfs_load_sprite("/gfx/sprites/map/tile_w_tm.sprite");
    tiles['^'] = dfs_load_sprite("/gfx/sprites/map/tile_w_bm.sprite");
    tiles['-'] = dfs_load_sprite("/gfx/sprites/map/tile_w_hor.sprite");
    tiles['('] = dfs_load_sprite("/gfx/sprites/map/tile_w_hor_l.sprite");
    tiles[')'] = dfs_load_sprite("/gfx/sprites/map/tile_w_hor_r.sprite");
    tiles['|'] = dfs_load_sprite("/gfx/sprites/map/tile_w_ver.sprite");
    tiles['i'] = dfs_load_sprite("/gfx/sprites/map/tile_w_ver_t.sprite");
    tiles[','] = dfs_load_sprite("/gfx/sprites/map/tile_w_ver_b.sprite");
    tiles['p'] = dfs_load_sprite("/gfx/sprites/map/tile_p.sprite");

    //door
    tiles['d'] = dfs_load_sprite("/gfx/sprites/map/tile_d_open.sprite");
    tiles['D'] = dfs_load_sprite("/gfx/sprites/map/tile_d_close.sprite");
    tiles['E'] = dfs_load_sprite("/gfx/sprites/map/tile_d_top.sprite");
    tiles['L'] = dfs_load_sprite("/gfx/sprites/map/tile_d_lock.sprite");

    //start
    tiles['s'] = dfs_load_sprite("/gfx/sprites/map/tile_s.sprite");

    //end
    tiles['e'] = dfs_load_sprite("/gfx/sprites/map/tile_e.sprite");
    tiles['#'] = dfs_load_sprite("/gfx/sprites/map/tile_end.sprite");

    //buttons
    tiles['A'] = dfs_load_sprite("/gfx/sprites/map/tile_a_up.sprite");
    tiles['a'] = dfs_load_sprite("/gfx/sprites/map/tile_a_down.sprite");
    tiles['B'] = dfs_load_sprite("/gfx/sprites/map/tile_b_up.sprite");
    tiles['b'] = dfs_load_sprite("/gfx/sprites/map/tile_b_down.sprite");
    tiles['C'] = dfs_load_sprite("/gfx/sprites/map/tile_c_up.sprite");
    tiles['c'] = dfs_load_sprite("/gfx/sprites/map/tile_c_down.sprite");

    //grass
    tiles['+'] = dfs_load_sprite("/gfx/sprites/map/tile_grass.sprite");

    //trees
    tiles['&'] = dfs_load_sprite("/gfx/sprites/map/tile_tree_0.sprite");
    tiles['*'] = dfs_load_sprite("/gfx/sprites/map/tile_tree_1.sprite");
    tiles['%'] = dfs_load_sprite("/gfx/sprites/map/tile_tree_2.sprite");

    //grids
    tiles['g'] = dfs_load_sprite("/gfx/sprites/map/tile_g.sprite");
    tiles['m'] = dfs_load_sprite("/gfx/sprites/map/tile_g_ver_t.sprite");
    tiles['r'] = dfs_load_sprite("/gfx/sprites/map/tile_g_ver.sprite");
    tiles['n'] = dfs_load_sprite("/gfx/sprites/map/tile_g_ver_b.sprite");
    tiles['q'] = dfs_load_sprite("/gfx/sprites/map/tile_g_hor_l.sprite");
    tiles['j'] = dfs_load_sprite("/gfx/sprites/map/tile_g_hor.sprite");
    tiles['z'] = dfs_load_sprite("/gfx/sprites/map/tile_g_hor_r.sprite");
    tiles['o'] = dfs_load_sprite("/gfx/sprites/map/tile_g_tr.sprite");
    tiles['x'] = dfs_load_sprite("/gfx/sprites/map/tile_g_bl.sprite");

    //misc
    tiles['k'] = dfs_load_sprite("/gfx/sprites/map/tile_k.sprite");
    tiles['w'] = dfs_load_sprite("/gfx/sprites/map/tile_w.sprite");
    tiles['v'] = tiles['w'];
    tiles['t'] = tiles['w'];
    tiles['l'] = tiles['w'];
    tiles['f'] = dfs_load_sprite("/gfx/sprites/map/tile_f.sprite");
    tiles['h'] = dfs_load_sprite("/gfx/sprites/map/tile_h.sprite");

    //arrow
    tiles['@'] = dfs_load_sprite("/gfx/sprites/map/tile_arrow_start.sprite");
    tiles['='] = dfs_load_sprite("/gfx/sprites/map/tile_arrow.sprite");
    tiles['$'] = dfs_load_sprite("/gfx/sprites/map/tile_arrow_end.sprite");

    map_select(0);
}

// draw current map on screen
uint8_t map_draw(int tick)
{
    // animate some tiles every other tick
    mirror_t mirror = tick % 2 < 1;

    //calculate top/left coordinates of visible map
    uint8_t sy = (SCREEN_HEIGHT_2 > player.y ? 0 : player.y - SCREEN_HEIGHT_2);
    uint8_t sx = (SCREEN_WIDTH_2 > player.x ? 0 : player.x - SCREEN_WIDTH_2);

    if (sy + SCREEN_HEIGHT > map->height)
        sy -= (sy + SCREEN_HEIGHT) - map->height;

    if (sx + SCREEN_WIDTH > map->width)
        sx -= (sx + SCREEN_WIDTH) - map->width;

    uint8_t start_y = (map->anim > player.sy ? 0 : player.sy - map->anim);
    uint8_t end_y = MIN(player.sy + map->anim, SCREEN_HEIGHT);
    for (uint8_t y = start_y; y < end_y; y++)
    {

        uint8_t start_x = (map->anim > player.sy ? 0 : player.sx - map->anim);
        uint8_t end_x = MIN(player.sx + map->anim, SCREEN_WIDTH);
        for (uint8_t x = start_x; x < end_x; x++)
        {
            char c = map->grid[map->layer_idx][sy + y][sx + x];
            sprite_t *tile = tiles[(int)c];
            if (tile != NULL)
                rdp_draw_sprite_with_texture(tile, MAP_CELL_SIZE * x, MAP_CELL_SIZE * y, (strchr(TILES_ANIMATED, c) ? mirror : MIRROR_DISABLED));
            else
                rdp_draw_filled_rectangle_size(MAP_CELL_SIZE * x, MAP_CELL_SIZE * y, MAP_CELL_SIZE, MAP_CELL_SIZE, colors[COLOR_BG]);
        }
    }

    if ((map->anim_direction == 1 && map->anim < MAP_NUM_ANIMS) || (map->anim_direction == -1 && map->anim > 0))
        map->anim += map->anim_direction;

    return map->id;
}

// move to the next layer in current map
void map_layer_next()
{
    map->layer_idx++;
}

// reset to the 1st layer (after death or restart)
void map_layer_reset()
{
    map->layer_idx = 0;
}

// load map from disk and returns it
bool map_load(uint8_t map_id)
{
    if (map)
        map_free();
    if (map_id >= NUM_MAPS)
        return false;

    int fp = dfs_open(level_paths[map_id]);
    if (fp > 0)
    {
        map = calloc(1, sizeof(map_t));
        // getting grid dimensions
        char buffer[GRID_CHARS];
        dfs_read(buffer, 1, GRID_CHARS, fp);

        int h, w;
        sscanf(buffer, "%dx%d", &h, &w);
        map->width = (uint8_t)w;
        map->height = (uint8_t)h;

        // getting layer count
        dfs_read(buffer, 1, LAYER_CHARS, fp);
        map->layers = atoi(buffer);

        // getting grid
        map->grid = calloc(map->layers, sizeof(char **));
        for (uint8_t l = 0; l < map->layers; l++)
        {
            map->grid[l] = calloc(map->height, sizeof(char *));
            for (uint8_t y = 0; y < map->height; y++)
            {
                map->grid[l][y] = calloc(map->width, sizeof(char));
                dfs_read(map->grid[l][y], 1, map->width, fp);
                dfs_seek(fp, 1, SEEK_CUR);
            }
        }

        map->id = map_id;
        map->anim_direction = 1;
        dfs_close(fp);
    }

    return true;
}

// free current map from memory
void map_free()
{
    for (uint8_t l = 0; l < map->layers; l++)
    {
        for (uint8_t y = 0; y < map->height; y++)
        {
            free(map->grid[l][y]);
        }
        free(map->grid[l]);
    }
    free(map->grid);
    free(map);
    map = NULL;
}

// (re)generate vegetation for current map
void map_regen_vegetation()
{
    map->layer_idx = 0;
    for (uint8_t y = 0; y < map->height; y++)
    {
        for (uint8_t x = 0; x < map->width; x++)
        {
            if (strchr(TILES_VEGETATION, map->grid[map->layer_idx][y][x]))
            {
                int n = rand() % 50;
                char out = '.';
                if (n < 5)
                    out = '+';
                if (n == 6)
                    out = '*';
                if (n == 7)
                    out = '&';
                if (n == 8)
                    out = '%';

                for (int i = 0; i < map->layers; i++)
                {
                    if (map->grid[i][y][x] == '.')
                        map->grid[i][y][x] = out;
                }
            }
        }
    }
}

// select (load, generate vegetation) a new map as current map
// return false when map can't be loaded
bool map_select(uint8_t map_id)
{
    if (map_id == 0)
        screen_load_title_resources();

    if (!map_load(map_id))
        return false;

    map_regen_vegetation();
    player_reset_in_map();
    if (map_id == 0)
        map->anim = MAP_NUM_ANIMS; // do not animate title map
    else
        sound_start("high");
    return true;
}
