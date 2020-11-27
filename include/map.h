#ifndef __MAP_H__
#define __MAP_H__

#include <stdint.h>
#include <stdio.h>

#define MAP_CELL_SIZE 32

#define SCREEN_HEIGHT 15
#define SCREEN_WIDTH 20

#define TILES_WALL "-|[]{}y<>^p()i,L"
#define TILES_WARP "vw"
#define TILES_VEGETATION ".%&+*"

#define GRID_CHARS 6  // HHxWW\n
#define LAYER_CHARS 2 // L\n

typedef struct coordinates
{
    uint8_t x;
    uint8_t y;
} coords_t;

typedef struct map
{
    uint8_t layers;
    uint8_t height;
    uint8_t width;
    uint8_t layer_idx;
    coords_t *tiles_pos[255];
    char ***grid;
} map_t;

void map_draw();
void map_init();
void map_next();
void map_reset();

#endif // __MAP_H__