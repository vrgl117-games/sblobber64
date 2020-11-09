#include "colors.h"

uint32_t colors[2];

void colors_init()
{
    colors[COLOR_BG] = graphics_make_color(0x22, 0x23, 0x23, 0xff);
    colors[COLOR_BLACK] = 0;
}