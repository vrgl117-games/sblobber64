#include <malloc.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "dfs.h"

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
