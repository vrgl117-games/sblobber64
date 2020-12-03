#ifndef __MAP_H__
#define __MAP_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAP_CELL_SIZE 32

#define NUM_MAPS 4

#define SCREEN_HEIGHT 15
#define SCREEN_HEIGHT_2 8
#define SCREEN_WIDTH 20
#define SCREEN_WIDTH_2 10
#define MAP_NUM_ANIMS SCREEN_WIDTH

#define TILES_WALL "-|[]{}y<>^p()i,L"
#define TILES_WARP "vw"
#define TILES_VEGETATION ".%&+*"
#define TILES_END "eE"
#define TILES_ANIMATED "fvw"

#define GRID_CHARS 6  // HHxWW\n
#define LAYER_CHARS 2 // L\n

#define MIN(a, b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

typedef struct coordinates
{
    uint8_t x;
    uint8_t y;
} coords_t;

typedef struct map
{
    uint8_t id;
    uint8_t layers;
    uint8_t height;
    uint8_t width;
    uint8_t layer_idx;
    coords_t *tiles_pos[255];

    uint8_t anim;          // "frame of the transition animation"
    int8_t anim_direction; // +1 or -1

    char ***grid;
} map_t;

uint8_t map_draw(int tick);
void map_free();
void map_init();
void map_layer_next();
bool map_select(uint8_t map_id);
void map_regen_vegetation();

#endif // __MAP_H__