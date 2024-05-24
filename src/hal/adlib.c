#include <conio.h>
#include <stdio.h>
#include "hal.h"

#define ADLIB_ADDR_PORT 0x0388
#define ADLIB_DATA_PORT 0x0389

#define ADLIB_STATUS_TIMER_EXPIRED 0x80
#define ADLIB_STATUS_TIMER1_EXPIRED 0x40
#define ADLIB_STATUS_TIMER2_EXPIRED 0x20

unsigned char adlib_read_status_register()
{
    return inp(ADLIB_ADDR_PORT);
}

// After writing to the register port, you must wait twelve cycles (3.3 microseconds) before sending the data;
// after writing the data, eighty-four cycles (23 microseconds) must elapse before any other sound card operation may be performed.
void adlib_write_register(int register_num, unsigned char value)
{
    // Need to accomodate time requirements in emulated dosbox environment?
    outp(ADLIB_ADDR_PORT, register_num);
    outp(ADLIB_DATA_PORT, value);
}

unsigned char adlib_detect()
{
    unsigned long long start_time;
    unsigned char retry;
    for (retry = 3; retry > 0; retry--)
    {
        int status1 = 0, status2 = 0;
        int test1 = 0xFF, test2 = 0xFF;

        // Reset both timers and enable interrupts
        adlib_write_register(4, 0x60);  // reset both timers
        start_time = hal_ticks;
        while(1) { if(hal_ticks != start_time) break; }
        adlib_write_register(4, 0x80);  // enable interrupts
        start_time = hal_ticks;
        while(1) { if(hal_ticks != start_time) break; }

        status1 = adlib_read_status_register();

        // Start Timer1 from 0xff
        adlib_write_register(2, 0xff);  // write 0xff to Timer 1
        start_time = hal_ticks;
        while(1) { if(hal_ticks != start_time) break; }
        adlib_write_register(4, 0x21);  // start Timer 1
        start_time = hal_ticks;
        while(1) { if(hal_ticks != start_time) break; }

        status2 = adlib_read_status_register();

        // Reset both timers and enable interrupts
        adlib_write_register(4, 0x60);  // reset both timers
        start_time = hal_ticks;
        while(1) { if(hal_ticks != start_time) break; }
        adlib_write_register(4, 0x80);  // enable interrupts
        start_time = hal_ticks;
        while(1) { if(hal_ticks != start_time) break; }

        // Check that the expected status values were encountered after each step
        test1 = status1 & 0xe0;
        test2 = status2 & 0xe0;
        if (test1 == 0x00 && test2 == 0xc0)
        {
            break;
        }
    }

    if (retry == 0)
        return 0;

    return 1;
}

void adlib_init()
{
    // TODO: Refactor all of this to use a channel abstraction

    int i;
    for (i=0; i<244; i++)
    {
        adlib_write_register(i, 0x00);
    }
}

