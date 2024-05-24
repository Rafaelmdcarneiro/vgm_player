#include <conio.h>
#include <dos.h>
#include "hal.h"

#define CTRL_PORT 0x43
#define SET_CMD   0x3C  // 0011 1100
#define TIMER0    0x40

#define PIC_IC_REGISTER 0x20
#define PIC_IMR 0x21
#define PIC_EOI 0x20

#define TIMER_INTERRUPT_NUM 0x08
#define DEFAULT_INPUT_FREQ ((unsigned long)0x001234DC)

void timer_configure(unsigned int hertz)
{
    unsigned int rate = DEFAULT_INPUT_FREQ / hertz;
    unsigned char low_byte =  (rate & 0xFF);
    unsigned char high_byte = (rate >> 8);

    _asm cli

     // Configure the 8253 with new rate
    outp(CTRL_PORT, SET_CMD);
    outp(TIMER0, low_byte);
    outp(TIMER0, high_byte);

    _asm sti
}

void timer_restore()
{
    unsigned char low_byte = 0xFF;
    unsigned char high_byte = 0xFF;

    _asm cli

    outp(CTRL_PORT, SET_CMD);
    outp(TIMER0, low_byte);
    outp(TIMER0, high_byte);

    _asm sti
}
