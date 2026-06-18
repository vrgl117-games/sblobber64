
#ifndef __RDPQ_H__
#define __RDPQ_H__

#include <libdragon.h>

#include "dfs.h"

void rdpq_attach_display_game(display_context_t disp);
void rdpq_draw_filled_fullscreen(uint32_t color);
void rdpq_draw_filled_rectangle_size(int x, int y, int width, int height, uint32_t color);
void rdpq_draw_sprite_with_texture(sprite_t *sp, int x, int y, mirror_t mirror);
void rdpq_draw_sprites_with_texture(sprites_t *sprites, int x, int y, mirror_t mirror);

#endif //__RDPQ_H__
