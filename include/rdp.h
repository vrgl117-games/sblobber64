
#ifndef __RDP_H__
#define __RDP_H__

#include <libdragon.h>

void rdp_attach(display_context_t disp);
void rdp_draw_filled_fullscreen(uint32_t color);
void rdp_draw_filled_rectangle_size(int x, int y, int width, int height, uint32_t color);

#endif //__RDP_H__