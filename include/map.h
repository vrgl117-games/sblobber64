#ifndef __MAP_H__
#define __MAP_H__

#define MAP_LAYERS 6
#define MAP_WIDTH 20
#define MAP_HEIGHT 15
#define MAP_CELL_SIZE 32

#define TILES_WALL "-|[]{}y<>^p()i,L"
#define TILES_VEGETATION ".%&+*"

void map_draw();
void map_init();
void map_next();
void map_reset();

#endif // __MAP_H__