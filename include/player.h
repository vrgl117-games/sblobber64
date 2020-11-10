#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <stdint.h>
#include "screens.h"

typedef struct player
{
    uint8_t x;
    uint8_t y;
    uint8_t h_of;
    uint8_t w_of;
} player_t;

void player_draw();
void player_init();
bool player_move(input_t *input);
void player_reset();

#endif // __PLAYER_H__