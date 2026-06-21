
#ifndef __RDPQ_H__
#define __RDPQ_H__

#include <libdragon.h>

#include "dfs.h"

void rdpq_draw_sprite_with_texture(sprite_t *sp, int x, int y, mirror_t mirror);
void rdpq_draw_sprites_with_texture(sprites_t *sprites, int x, int y, mirror_t mirror);

#endif //__RDPQ_H__
