#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include <libdragon.h>

sprite_t *dfs_load_sprite(const char *const path);
sprite_t *dfs_load_spritef(const char *const format, ...);

#endif //__FILESYSTEM_H__