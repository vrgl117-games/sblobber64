#include "rdp.h"

extern uint32_t __width;
extern uint32_t __height;

void rdp_attach(display_context_t disp)
{
    rdp_attach_display(disp);
    rdp_set_default_clipping();
}

void rdp_draw_filled_fullscreen(uint32_t color)
{
    rdp_draw_filled_rectangle_size(0, 0, __width, __height, color);
}

void rdp_draw_filled_rectangle_size(int x, int y, int width, int height, uint32_t color)
{
    rdp_enable_primitive_fill();
    rdp_sync(SYNC_PIPE);
    rdp_set_primitive_color(color);
    rdp_draw_filled_rectangle(x, y, x + width, y + height);
}

void rdp_draw_sprite_with_texture(sprite_t *sp, int x, int y, mirror_t mirror)
{
    rdp_enable_texture_copy();
    rdp_sync(SYNC_PIPE);
    rdp_load_texture(0, 0, mirror, sp);
    rdp_draw_sprite(0, x, y, mirror);
}

void rdp_draw_sprites_with_texture(sprites_t *sprites, int x, int y, mirror_t mirror)
{
    int xx = 0;
    int yy = 0;

    for (int i = 0; i < sprites->slices; i++)
    {
        int ii = (mirror == MIRROR_XY ? sprites->slices - 1 - i : i);
        rdp_draw_sprite_with_texture(sprites->sprites[ii], x + xx, y + yy, mirror);
        if (i % sprites->mod == sprites->mod - 1)
        {
            yy += sprites->sprites[ii]->height;
            xx = 0;
        }
        else
            xx += sprites->sprites[ii]->width;
    }
}