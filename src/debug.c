#include <malloc.h>
#include <stdarg.h>

#include "debug.h"
#include "graphics.h"

static char *debug = NULL;

void debug_clear()
{
    free(debug);
    debug = NULL;
}
void debug_draw(display_context_t disp)
{
    if (debug != NULL)
        graphics_draw_textf_with_background(disp, 4, 4, debug);
}

void debug_setf(const char *const format, ...)
{
    if (debug != NULL)
        debug_clear();

    debug = malloc(256);
    va_list args;
    va_start(args, format);
    vsprintf(debug, format, args);
    va_end(args);
}
