#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include <libdragon.h>

typedef struct sprites
{
    sprite_t *sprites[32];
    int slices;

    int height;
    int width;

    int mod;
} sprites_t;

void dfs_free_sprites(sprites_t *sprites);
sprite_t *dfs_load_sprite(const char *const path);
sprite_t *dfs_load_spritef(const char *const format, ...);
sprites_t *dfs_load_sprites(const char *const path, char *lang);

#endif //__FILESYSTEM_H__