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