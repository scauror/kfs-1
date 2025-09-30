#ifndef IO_H
#define IO_H

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

#define VGA_COLOR_BLACK   0x0
#define VGA_COLOR_BLUE    0x1
#define VGA_COLOR_GREEN   0x2
#define VGA_COLOR_CYAN    0x3
#define VGA_COLOR_RED     0x4
#define VGA_COLOR_MAGENTA 0x5
#define VGA_COLOR_BROWN   0x6
#define VGA_COLOR_LIGHTGRAY 0x7
#define VGA_COLOR_DARKGRAY 0x8
#define VGA_COLOR_LIGHTBLUE 0x9
#define VGA_COLOR_LIGHTGREEN 0xA
#define VGA_COLOR_LIGHTCYAN 0xB
#define VGA_COLOR_LIGHTRED 0xC
#define VGA_COLOR_LIGHTMAGENTA 0xD
#define VGA_COLOR_YELLOW 0xE
#define VGA_COLOR_WHITE 0xF

#define VGA_COLOR_DEFAULT VGA_COLOR_LIGHTBLUE

extern uint8_t current_color;

void io_init(void);
void put_char(char c);
void put_string(const char* str);
void printk(const char* fmt, ...);

#endif
