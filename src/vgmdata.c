#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hal/hal.h"
#include "vgmdata.h"

typedef struct
{
    unsigned long vgm_indent;
    unsigned long eof_offset;
    unsigned long version;
    unsigned long sn76489_clock;
    unsigned long ym2413_clock;
    unsigned long gd3_offset;
    unsigned long total_num_samples;
    unsigned long loop_offset;
    unsigned long loop_num_samples;
    unsigned long rate;
    unsigned long sn_fb_snw_sf;
    unsigned long ym2612_clock;
    unsigned long ym2151_clock;
    unsigned long vgm_data_offset;
    unsigned long sega_pcm_clock;
    unsigned long spcm_interface;
    unsigned long rf5C68_clock;
    unsigned long ym2203_clock;
    unsigned long ym2608_clock;
    unsigned long ym2610b_clock;
    unsigned long ym3812_clock;
    unsigned long ym3526_clock;
    unsigned long y8950_clock;
    unsigned long ymF262_clock;
} vgm_header_t;

vgm_stream_t* vgm_open_stream(char *filename)
{
    vgm_stream_t *stream;
    vgm_header_t *vgm_header;
    FILE *vgm_file;
    unsigned long int vgm_file_size;

    // Create the stream
    stream = (vgm_stream_t *)malloc(sizeof(vgm_stream_t));

    // Open the vgm file
    vgm_file = fopen(filename, "rb");

    // Determine the size of the file
    fseek(vgm_file, 0L, SEEK_END);
    vgm_file_size = ftell(vgm_file);
    rewind(vgm_file);

    // Allocate memory for stream data
    stream->data = (unsigned char*)malloc(vgm_file_size);

    // Read in vgm data
    fread(stream->data, vgm_file_size, 1, vgm_file);
    fclose(vgm_file);

    vgm_header = (vgm_header_t*)stream->data;

    stream->data_offset = (unsigned int)(&vgm_header->vgm_data_offset);
    stream->data_offset -= (unsigned int)vgm_header;
    stream->data_offset += vgm_header->vgm_data_offset;
    stream->data_end = vgm_header->eof_offset + 4;

    stream->data_ptr = (unsigned char*)(stream->data + stream->data_offset);
    stream->file_end = ((unsigned long)stream->data) + stream->data_end;

    return stream;
}

void vgm_close_stream(vgm_stream_t *stream)
{
    free(stream->data);
    stream->data = 0;

    free(stream);
    stream = 0;    
}

unsigned char vgm_next_byte(vgm_stream_t *stream)
{
    unsigned char next_byte = *(stream->data_ptr);
    stream->data_ptr++;
    return next_byte;
}

unsigned short vgm_next_short(vgm_stream_t *stream)
{
    unsigned short next_short = *((unsigned short*)stream->data_ptr);
    stream->data_ptr += 2;
    return next_short;
}
