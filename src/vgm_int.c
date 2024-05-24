#include "hal/hal.h"
#include "vgmdata.h"
#include "vgm_int.h"

unsigned char vgm_interpreter_init()
{
    unsigned char adlib_detected;

    // Detect adlib card
    adlib_detected = adlib_detect();
    if(!adlib_detected)
    {
        printf("Error: No AdLib-compatible sound board was detected!\n");
        return 0;
    }

    return 1;
}

player_state_t vgm_interpreter_tick(vgm_stream_t *stream)
{
    player_state_t state;
    unsigned char cmd;

    cmd = vgm_next_byte(stream);

    state.last_command = cmd;
    state.delay = 0;
    state.is_playing = 1;
    state.is_error = 0;

    if (cmd == 0x5A)
    {
        unsigned char reg, val;

        reg = vgm_next_byte(stream);
        val = vgm_next_byte(stream);

        adlib_write_register(reg, val);
        state.delay = 0;
    }

    else if (cmd == 0x61)
    {
        state.delay = vgm_next_short(stream);   
    }

    else if (cmd == 0x62)
    {
        state.delay = 735;
    }

    else if (cmd == 0x66)
    {
        // done playing song
        state.is_playing = 0;
    }

    else if ((unsigned char)(cmd & 0x70) == (unsigned char)0x70)
    {
        state.delay = (cmd & 0x0F) + 1;
    }

    else if ((unsigned char)(cmd & 0xA0) == (unsigned char)0xA0)
    {
        vgm_next_short(stream);
    }

    // Unknown command
    else
    {
        state.is_playing = 0;
        state.is_error = 1;
    }

    return state;
}

