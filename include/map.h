#ifndef __MAP_H__
#define __MAP_H__

#define MAP_CELL_SIZE 32

#define MAP_HEIGHT 24 //15
#define MAP_WIDTH 24  //20

#define MAP_LAYERS 1 //6

#define SCREEN_HEIGHT 15
#define SCREEN_WIDTH 20

#define TILES_WALL "-|[]{}y<>^p()i,L"
#define TILES_VEGETATION ".%&+*"

void map_draw();
void map_init();
void map_next();
void map_reset();

#endif // __MAP_H__