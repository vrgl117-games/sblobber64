#include "map.h"

#include <stdlib.h>
#include <string.h>

#include "dfs.h"
#include "player.h"
#include "rdp.h"

#include "debug.h"

extern player_t player;

map_t *map;

sprite_t *tiles[255] = {0};

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
    tiles['L'] = dfs_load_sprite("/gfx/sprites/map/tile_d_lock.sprite");

    //start
    tiles['s'] = dfs_load_sprite("/gfx/sprites/map/tile_s.sprite");

    //end
    tiles['e'] = dfs_load_sprite("/gfx/sprites/map/tile_e.sprite");

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

    //misc
    tiles['k'] = dfs_load_sprite("/gfx/sprites/map/tile_k.sprite");
    tiles['w'] = dfs_load_sprite("/gfx/sprites/map/tile_w.sprite");
    tiles['v'] = tiles['w'];

    map_reset();
}

void map_draw()
{
    int8_t sy = player.y - (SCREEN_HEIGHT / 2);
    int8_t sx = player.x - (SCREEN_WIDTH / 2);
    if (sy < 0)
    {
        sy = 0;
    }
    if (sx < 0)
    {
        sx = 0;
    }
    if (sy + SCREEN_HEIGHT > map->height)
    {
        sy -= (sy + SCREEN_HEIGHT) - map->height;
    }
    if (sx + SCREEN_WIDTH > map->width)
    {
        sx -= (sx + SCREEN_WIDTH) - map->width;
    }
    for (uint8_t y = 0; y < SCREEN_HEIGHT; y++)
    {
        for (uint8_t x = 0; x < SCREEN_WIDTH; x++)
        {
            sprite_t *tile = tiles[(int)map->grid[map->layer_idx][sy + y][sx + x]];
            if (tile != NULL)
                rdp_draw_sprite_with_texture(tile, MAP_CELL_SIZE * x, MAP_CELL_SIZE * y, 0);
        }
    }
}

void map_next()
{
    map->layer_idx++;
}

map_t *map_load(char *path)
{
    int fp = dfs_open(path);
    map_t *_map = NULL;
    if (fp > 0)
    {
        _map = calloc(1, sizeof(map_t));
        // getting grid dimensions
        char buffer[GRID_CHARS];
        dfs_read(buffer, 1, GRID_CHARS, fp);

        int h, w;
        sscanf(buffer, "%dx%d", &h, &w);
        _map->width = (uint8_t)w;
        _map->height = (uint8_t)h;
        dfs_read(buffer, 1, LAYER_CHARS, fp);
        _map->layers = buffer[0] - '0';

        _map->grid = calloc(_map->layers, sizeof(char **));
        for (uint8_t l = 0; l < _map->layers; l++)
        {
            _map->grid[l] = calloc(_map->height, sizeof(char *));
            for (uint8_t y = 0; y < _map->height; y++)
            {
                _map->grid[l][y] = calloc(_map->width, sizeof(char));
                dfs_read(_map->grid[l][y], 1, _map->width, fp);
                dfs_seek(fp, 1, SEEK_CUR);
            }
        }

        dfs_close(fp);
    }

    return _map;
}

void map_reset()
{
    map = map_load("/maps/layers.map");
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
                    map->grid[i][y][x] = out;
            }
        }
    }
}