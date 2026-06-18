#include "input.h"

static volatile int tick = 0;

joypad_buttons_t input_get()
{
    joypad_poll();
    joypad_buttons_t down = joypad_get_buttons_pressed(JOYPAD_PORT_1);
    joypad_buttons_t input = {
        .d_up = down.d_up,
        .d_down = down.d_down,
        .d_left = down.d_left,
        .d_right = down.d_right,
        .a = down.a,
        .start = down.start,
    };
    if (input.d_up || input.d_down || input.d_left || input.d_right)
        tick = 0;

    joypad_buttons_t pressed = joypad_get_buttons(JOYPAD_PORT_1);

    if (pressed.d_up && tick == 3)
    {
        input.d_up = 1;
        tick = 1;
    }
    if (pressed.d_down && tick == 3)
    {
        input.d_down = 1;
        tick = 1;
    }
    if (pressed.d_left && tick == 3)
    {
        input.d_left = 1;
        tick = 1;
    }
    if (pressed.d_right && tick == 3)
    {
        input.d_right = 1;
        tick = 1;
    }
    return input;
}

void input_timer()
{
    tick++;
}
