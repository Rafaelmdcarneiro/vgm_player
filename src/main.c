#include <stdio.h>

#include "hal/hal.h"
#include "vgmdata.h"
#include "vgm_int.h"

unsigned long long total_ticks;

int main(int argc, char **argv)
{
    unsigned long long last_ticks;
    unsigned long long next_ticks;
    player_state_t player_state;
    vgm_stream_t *data_stream;

    // Configure vgm interpreter
    if(vgm_interpreter_init() == 0)
    {
        printf("Error: unable to initialize vgm interpreter\n");
        return -1;
    }

    // Configure system timer
    timer_configure(44100);

    // Open data stream
    data_stream = vgm_open_stream(argv[1]);

    last_ticks = hal_ticks;
    total_ticks = 0;
    player_state.is_playing = 1;
    player_state.delay = 0;
    while(player_state.is_playing)
    {
        // Do nothing if it isn't yet time for another tick
        next_ticks = hal_ticks;
        if (next_ticks == last_ticks)
           continue;

        last_ticks = next_ticks;
        total_ticks++;

        // If the stream is in the middle of a delay then do nothing
        // during this tick
        if (player_state.delay > 0)
        {
            --player_state.delay;
            continue;
        }

        // Advance the player to the next tick in the stream
        player_state = vgm_interpreter_tick(data_stream);

        // Check the player state to see if the player needs to stop
        if (player_state.is_error)
        {
            player_state.is_playing = 0;
        }
        else if (player_state.is_playing == 0)
        {
            player_state.is_playing = 0;
        }
    }

    adlib_init();
    timer_restore();
    vgm_close_stream(data_stream);
  
    return 0;
}
