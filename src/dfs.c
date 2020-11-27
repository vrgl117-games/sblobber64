#include <malloc.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "dfs.h"

void dfs_free_sprites(sprites_t *sprites)
{
    if (sprites == NULL)
        return;

    for (int i = 0; sprites->sprites[i] != 0; i++)
        free(sprites->sprites[i]);
    free(sprites);
}

sprite_t *dfs_load_sprite(const char *const path)
{
    int fp = dfs_open(path);

    if (fp > 0)
    {
        int s = dfs_size(fp);
        sprite_t *sp = malloc(s);
        dfs_read(sp, 1, s, fp);
        dfs_close(fp);

        // Invalidate data associated with sprite in cache
        if (sp->bitdepth > 0)
            data_cache_hit_writeback_invalidate(sp->data, sp->width * sp->height * sp->bitdepth);
        else
            data_cache_hit_writeback_invalidate(sp->data, (sp->width * sp->height) >> 1);

        return sp;
    }

    return NULL;
}

sprite_t *dfs_load_spritef(const char *const format, ...)
{
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

    return dfs_load_sprite(buffer);
}

sprites_t *dfs_load_sprites(const char *const path, char *lang)
{
    char buffer[256];

    sprites_t *data = calloc(1, sizeof(sprites_t));

    int x = 0;
    int y = 0;
    int i = 0;
    while (true)
    {
        if (lang == NULL)
            sprintf(buffer, path, x, y);
        else
            sprintf(buffer, path, lang, x, y);

        sprite_t *sp = dfs_load_sprite(buffer);
        if (sp == NULL)
        {
            if (x == 0)
                break;
            else
            {
                y++;
                if (data->mod == 0)
                    data->mod = x;
                x = 0;
                continue;
            }
        }
        data->sprites[i] = sp;

        if (x == 0)
            data->height += data->sprites[i]->height;
        if (y == 0)
            data->width += data->sprites[i]->width;

        i++;
        x++;
    }
    data->slices = i;
    return data;
}

int dfs_openf(const char *const format, ...)
{
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

    return dfs_open(buffer);
}
