

#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include <stdio.h>
#include <libdragon.h>

void graphics_draw_textf_with_background(display_context_t disp, int x, int y, const char *const format, ...);

#endif //__GRAPHICS_H__