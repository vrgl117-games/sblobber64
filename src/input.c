#include "input.h"

static volatile int tick = 0;

input_t input_get()
{
    controller_scan();
    input_t input = get_keys_down().c[0];
    if (input.up || input.down || input.left || input.right)
        tick = 0;

    input_t pressed = get_keys_pressed().c[0];

    if (pressed.up && tick == 3)
    {
        input.up = true;
        tick = 1;
    }
    if (pressed.down && tick == 3)
    {
        input.down = true;
        tick = 1;
    }
    if (pressed.left && tick == 3)
    {
        input.left = true;
        tick = 1;
    }
    if (pressed.right && tick == 3)
    {
        input.right = true;
        tick = 1;
    }
    return input;
}

void input_timer()
{
    tick++;
}