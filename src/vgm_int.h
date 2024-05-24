#ifndef PLAYER_H
#define PLAYER_H

typedef struct
{
    unsigned char last_command;
    unsigned char is_playing;
    unsigned char is_error;
    unsigned int delay;
} player_state_t;

unsigned char vgm_interpreter_init();
player_state_t vgm_interpreter_tick(vgm_stream_t *stream);

#endif
