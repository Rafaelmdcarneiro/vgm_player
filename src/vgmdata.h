#ifndef VGMDATA_H
#define VGMDATA_H

typedef struct
{
    unsigned char *data;
    unsigned char *data_ptr;
    unsigned long num_samples;
    unsigned long data_offset;
    unsigned long data_end;
    unsigned long file_end;
} vgm_stream_t;

vgm_stream_t* vgm_open_stream(char *filename);
void vgm_close_stream(vgm_stream_t *stream);
unsigned char vgm_next_byte(vgm_stream_t *stream);
unsigned short vgm_next_short(vgm_stream_t *stream);

#endif
