#ifndef VGA_TEXT_H
#define VGA_TEXT_H

#define VGA_40x25_16_COLOR_TEXT_MODE 0x01
#define VGA_80x25_16_COLOR_TEXT_MODE 0x03
#define VGA_320x200_16_COLOR_MODE 0x0D
#define VGA_320x200_256_COLOR_MODE 0x13

#define VGA_TEXT_COLOR_BLACK 0
#define VGA_TEXT_COLOR_BLUE 1
#define VGA_TEXT_COLOR_GREEN 2
#define VGA_TEXT_COLOR_CYAN 3
#define VGA_TEXT_COLOR_RED 4
#define VGA_TEXT_COLOR_MAGENTA 5
#define VGA_TEXT_COLOR_BROWN 6
#define VGA_TEXT_COLOR_LIGHT_GRAY 7

#define VGA_TEXT_COLOR_LIGHT_BIT 0x08

void vga_set_mode(unsigned char mode);
void vga_write_text(char *str, unsigned char fg, unsigned char bg, unsigned int x, unsigned int y);
void vga_clear_screen(unsigned char color);
void vga_paint_rect(unsigned char c, unsigned char fg, unsigned char bg, unsigned int x, unsigned int y, unsigned int width, unsigned int height);
void vga_text_disable_cursor();
void vga_text_enable_cursor();

#endif
