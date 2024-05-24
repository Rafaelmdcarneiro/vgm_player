#ifndef ADLIB_H
#define ADLIB_H

unsigned char adlib_detect();
void adlib_init();

unsigned char adlib_read_status_register();
void adlib_write_register(int register_num, unsigned char value);

#endif
