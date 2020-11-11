#ifndef __MAP_H__
#define __MAP_H__

#define MAP_LAYERS 4
#define MAP_WIDTH 20
#define MAP_HEIGHT 15
#define MAP_CELL_SIZE 32

void map_draw();
void map_init();
void map_next();
void map_reset();

#endif // __MAP_H__