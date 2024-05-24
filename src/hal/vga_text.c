#include <i86.h>
#include <string.h>
#include "vga_text.h"

unsigned char far *vga_text_buffer = (char far*)0xB8000000L;
unsigned int res_x;
unsigned int res_y;

void vga_set_mode(unsigned char mode)
{
    union REGS in_regs, out_regs;
    in_regs.h.ah = 0;     // Set subfunction
    in_regs.h.al = mode;  // VGA mode 3 (16 color text)

    switch(mode)
    {
        case VGA_40x25_16_COLOR_TEXT_MODE:
            res_x = 40;
            res_y = 25;
            break;
        case VGA_80x25_16_COLOR_TEXT_MODE:
            res_x = 80;
            res_y = 25;
            break;
        case VGA_320x200_16_COLOR_MODE:
        case VGA_320x200_256_COLOR_MODE:
            res_x = 320;
            res_y = 200;
            break;
    }

    int86(0x10, &in_regs, &out_regs);
}

void vga_write_text(char *str, unsigned char fg, unsigned char bg, unsigned int x, unsigned int y)
{
    int i;
    char* ptr = str;
    unsigned char far *v_ptr = vga_text_buffer + ((y*res_x*2)+(x*2));
    unsigned char colors = (bg << 4) | (fg & 0x0F);
    while(*ptr != '\0')
    {
        *v_ptr++ = *ptr++;
        *v_ptr++ = colors;
    }
}

void vga_clear_screen(unsigned char color)
{
    //memset(vga_text_buffer, color, 4000);
    int i;
    unsigned char far *v_ptr = vga_text_buffer;
    unsigned char colors = color << 4;

    for (i=0; i<res_x*res_y; i++)
    {
            *v_ptr++ = ' ';
            *v_ptr++ = colors;        
    }
}

void vga_paint_rect(unsigned char c, unsigned char fg, unsigned char bg, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
    int i, j;
    unsigned char far *v_ptr;
    unsigned char colors = (bg << 4) | (fg & 0x0F);

    for (i=y; i<y+height; i++)
    {
        v_ptr = vga_text_buffer + ((i*res_x*2)+(x*2));
        for (j=x; j<x+width; j++)
        {
            *v_ptr++ = c;
            *v_ptr++ = colors;
        }
    }
}

void vga_text_disable_cursor()
{
	outp(0x3D4, 0x0A);
	outp(0x3D5, 0x20);
}

void vga_text_enable_cursor()
{
    // Enable cursor
    outp(0x3D4, 0x0A);
    outp(0x3D5, inp(0x3D5) & 0xC0);
    outp(0x3D4, 0x0B);
    outp(0x3D5, inp(0x3D5) & 0xE0);

    // Set cursor position to (0,0)
    outp(0x3D4, 0x0F);
    outp(0x3D5, 0xFF);
    outp(0x3D4, 0x0E);
    outp(0x3D5, 0xFF);
}