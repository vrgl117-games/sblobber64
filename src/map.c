#include "map.h"

#include "dfs.h"
#include "rdp.h"

char map[MAP_HEIGHT][MAP_WIDTH] = {
    {"...................."},
    {"..{--------------}.."},
    {"..|s             |.."},
    {"..|              |.."},
    {"..|              |.."},
    {"..>------y- -----<.."},
    {"..|     e|       |.."},
    {"..>-     >---} --<.."},
    {"..|      |   |   |.."},
    {"..|    {-]   |   |.."},
    {"..|    |     |   |.."},
    {"..| ---] |   |   |.."},
    {"..|      |       |.."},
    {"..[------^-------].."},
    {"...................."},
};

static sprite_t *tiles[255] = {0};

void map_init()
{
    tiles['{'] = dfs_load_sprite("/gfx/sprites/map/tile_w_tl.sprite");
    tiles['}'] = dfs_load_sprite("/gfx/sprites/map/tile_w_tr.sprite");
    tiles['['] = dfs_load_sprite("/gfx/sprites/map/tile_w_bl.sprite");
    tiles[']'] = dfs_load_sprite("/gfx/sprites/map/tile_w_br.sprite");
    tiles['>'] = dfs_load_sprite("/gfx/sprites/map/tile_w_ml.sprite");
    tiles['<'] = dfs_load_sprite("/gfx/sprites/map/tile_w_mr.sprite");
    tiles['y'] = dfs_load_sprite("/gfx/sprites/map/tile_w_tm.sprite");
    tiles['^'] = dfs_load_sprite("/gfx/sprites/map/tile_w_bm.sprite");
    tiles['-'] = dfs_load_sprite("/gfx/sprites/map/tile_w_hor.sprite");
    tiles['|'] = dfs_load_sprite("/gfx/sprites/map/tile_w_ver.sprite");
    tiles['s'] = dfs_load_sprite("/gfx/sprites/map/tile_s.sprite");
    tiles['e'] = dfs_load_sprite("/gfx/sprites/map/tile_e.sprite");
}

void map_draw()
{
    for (uint8_t y = 0; y < MAP_HEIGHT; y++)
    {
        for (uint8_t x = 0; x < MAP_WIDTH; x++)
        {
            sprite_t *tile = tiles[(int)map[y][x]];
            if (tile != NULL)
            {
                rdp_draw_sprite_with_texture(tile, MAP_CELL_SIZE * x, MAP_CELL_SIZE * y, 0);
            }
        }
    }
}
