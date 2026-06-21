#include <rdpq.h>
#include <rdpq_mode.h>
#include <rdpq_sprite.h>

#include "rdpq.h"

void rdpq_draw_sprite_with_texture(sprite_t *sp, int x, int y, mirror_t mirror)
{
    rdpq_set_mode_standard();
    rdpq_mode_alphacompare(1);
    rdpq_blitparms_t parms = {
        .flip_x = (mirror == MIRROR_X || mirror == MIRROR_XY),
        .flip_y = (mirror == MIRROR_Y || mirror == MIRROR_XY),
    };
    rdpq_sprite_blit(sp, x, y, &parms);
}

void rdpq_draw_sprites_with_texture(sprites_t *sprites, int x, int y, mirror_t mirror)
{
    int xx = 0;
    int yy = 0;

    for (int i = 0; i < sprites->slices; i++)
    {
        int ii = (mirror == MIRROR_XY ? sprites->slices - 1 - i : i);
        rdpq_draw_sprite_with_texture(sprites->sprites[ii], x + xx, y + yy, mirror);
        if (i % sprites->mod == sprites->mod - 1)
        {
            yy += sprites->sprites[ii]->height;
            xx = 0;
        }
        else
            xx += sprites->sprites[ii]->width;
    }
}
