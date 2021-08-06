#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <stdint.h>
#include "screens.h"

#define PLAYER_NUM_ANIMS 2
#define INVENTORY_NUM_KEYS 2

#define KEY_YELLOW 0
#define KEY_RED 1

typedef struct inventory
{
    bool keys[INVENTORY_NUM_KEYS];
} inventory_t;

typedef struct player
{
    // coordinates in map
    uint8_t x;
    uint8_t y;

    // coordinates on screen
    uint8_t sx;
    uint8_t sy;

    // size offset (0 or 1)
    uint8_t h_of;
    uint8_t w_of;

    // size (1, 3 or 9)
    uint8_t size;

    // "frame" of the animaton
    uint8_t h_of_anim;
    uint8_t w_of_anim;

    // player lives (0 to PLAYER_MAX_LIVES)
    uint8_t lives;

    // inventory
    inventory_t inventory;

    // nb of frames the controler should rumble
    uint8_t rumble;

} player_t;

#define SLIME_SINGLE 0
#define SLIME_VER_0 1
#define SLIME_VER_0_ANIM SLIME_VER_0
#define SLIME_VER_1 2
#define SLIME_VER_2 3
#define SLIME_VER_1_ANIM SLIME_VER_2
#define SLIME_HOR_0 4
#define SLIME_HOR_1 5
#define SLIME_HOR_2 6
#define SLIME_SQ_0 7
#define SLIME_SQ_1 8
#define SLIME_SQ_2 9
#define SLIME_SQ_3 10
#define SLIME_SQ_2_ANIM SLIME_SQ_3
#define SLIME_SQ_4 11
#define SLIME_SQ_5 12
#define SLIME_SQ_3_ANIM SLIME_SQ_5
#define SLIME_SQ_6 13
#define SLIME_SQ_7 14
#define SLIME_SQ_8 15
#define SLIME_HOR_0_ANIM 16
#define SLIME_HOR_1_ANIM 17
#define SLIME_SQ_0_ANIM 18
#define SLIME_SQ_1_ANIM 19
#define SLIME_SQ_4_ANIM 20
#define SLIME_SQ_5_ANIM 21

#define PLAYER_MAX_LIVES 3

void player_draw();
void player_init();
char player_move(input_t *input);
void player_reset_in_map();
void player_reset_inventory();
void player_reset();
void player_start_rumble(uint8_t n);
bool player_stop_rumble();

#endif // __PLAYER_H__