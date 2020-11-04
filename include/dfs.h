#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include <libdragon.h>

typedef struct map
{
    sprite_t *sprites[12];
    int slices;

    int height;
    int width;

    int mod;
} map_t;

void dfs_free_map(map_t *map);
sprite_t *dfs_load_sprite(const char *const path);
sprite_t *dfs_load_spritef(const char *const format, ...);
map_t *dfs_load_map(const char *const path, char *lang);

#endif //__FILESYSTEM_H__