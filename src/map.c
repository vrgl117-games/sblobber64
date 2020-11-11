#include "map.h"

#include "dfs.h"
#include "rdp.h"

int map_idx = 0;
char map[3][MAP_HEIGHT][MAP_WIDTH] = {
    {
        {"...................."},
        {"..{--------------}.."},
        {"..|s            B|.."},
        {"..|              |.."},
        {"..|              |.."},
        {"..>------y-------<.."},
        {"..|     e|       |.."},
        {"..>-     >---} --<.."},
        {"..|      |  B|   |.."},
        {"..|    {-]   |   |.."},
        {"..|    |     |   |.."},
        {"..>----] |   |   |.."},
        {"..|      |       |.."},
        {"..[------^-------].."},
        {"...................."},
    },
    {
        {"...................."},
        {"..{--------------}.."},
        {"..|s            b|.."},
        {"..|              |.."},
        {"..|              |.."},
        {"..>------y- -----<.."},
        {"..|     e|       |.."},
        {"..>-     >---} --<.."},
        {"..|      |  B|   |.."},
        {"..|    {-]   |   |.."},
        {"..|    |     |   |.."},
        {"..>----] |   |   |.."},
        {"..|      |       |.."},
        {"..[------^-------].."},
        {"...................."},
    },
    {
        {"...................."},
        {"..{--------------}.."},
        {"..|s            b|.."},
        {"..|              |.."},
        {"..|              |.."},
        {"..>------y- -----<.."},
        {"..|     e|       |.."},
        {"..>-     >---} --<.."},
        {"..|      |  b|   |.."},
        {"..|    {-]   |   |.."},
        {"..|    |     |   |.."},
        {"..| ---] |   |   |.."},
        {"..|      |       |.."},
        {"..[------^-------].."},
        {"...................."},
    }};

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
    tiles['B'] = dfs_load_sprite("/gfx/sprites/map/tile_b_up.sprite");
    tiles['b'] = dfs_load_sprite("/gfx/sprites/map/tile_b_down.sprite");

    map_reset();
}

void map_draw()
{
    for (uint8_t y = 0; y < MAP_HEIGHT; y++)
    {
        for (uint8_t x = 0; x < MAP_WIDTH; x++)
        {
            sprite_t *tile = tiles[(int)map[map_idx][y][x]];
            if (tile != NULL)
            {
                rdp_draw_sprite_with_texture(tile, MAP_CELL_SIZE * x, MAP_CELL_SIZE * y, 0);
            }
        }
    }
}

void map_next()
{
    map_idx++;
}

void map_reset()
{
    map_idx = 0;
}