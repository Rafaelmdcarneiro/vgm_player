#ifndef TIMER_H
#define TIMER_H

void timer_configure(unsigned int hertz);
void timer_restore();

#define hal_ticks (*(unsigned long far *)0x0000046CL)

#endif
