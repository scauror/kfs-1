#include "io.h"
#include "utils.h"
#include <stdarg.h>

volatile uint16_t* const VGA_BUFFER = (volatile uint16_t*)0xB8000;
static size_t cursor_x = 0;
static size_t cursor_y = 0;

static void move_cursor(void) {
    uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

static void show_cursor(uint8_t start, uint8_t end) {
    outb(0x3D4, 0x0A);
    outb(0x3D5, (inb(0x3D5) & 0xC0) | (start & 0x1F));
    outb(0x3D4, 0x0B);
    outb(0x3D5, (inb(0x3D5) & 0xE0) | (end & 0x1F));
}

static void scroll(void) {
    if (cursor_y >= VGA_HEIGHT) {
        for (size_t y = 1; y < VGA_HEIGHT; y++)
            for (size_t x = 0; x < VGA_WIDTH; x++)
                VGA_BUFFER[(y-1)*VGA_WIDTH + x] = VGA_BUFFER[y*VGA_WIDTH + x];
        for (size_t x = 0; x < VGA_WIDTH; x++)
            VGA_BUFFER[(VGA_HEIGHT-1)*VGA_WIDTH + x] = (' ' | (VGA_COLOR_DEFAULT << 8));
        cursor_y = VGA_HEIGHT - 1;
    }
}

void io_init(void) {
    cursor_x = 0;
    cursor_y = 0;
    for (size_t y = 0; y < VGA_HEIGHT; y++)
        for (size_t x = 0; x < VGA_WIDTH; x++)
            VGA_BUFFER[y*VGA_WIDTH + x] = (' ' | (VGA_COLOR_DEFAULT << 8));
    show_cursor(0, 15);
    move_cursor();
}

void put_char(char c) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else if (c == '\b') {
        if (cursor_x > 0) {
            cursor_x--;
            VGA_BUFFER[cursor_y*VGA_WIDTH + cursor_x] = (' ' | (VGA_COLOR_DEFAULT << 8));
        }
    } else {
        VGA_BUFFER[cursor_y*VGA_WIDTH + cursor_x] = ((uint16_t)VGA_COLOR_DEFAULT << 8) | (uint8_t)c;
        cursor_x++;
        if (cursor_x >= VGA_WIDTH) {
            cursor_x = 0;
            cursor_y++;
        }
    }
    scroll();
    move_cursor();
}

void put_string(const char* str) {
    for (size_t i = 0; str[i]; i++)
        put_char(str[i]);
}

void printk(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    for (size_t i=0; fmt[i]; i++) {
        if (fmt[i]=='%') {
            i++;
            switch(fmt[i]) {
                case 'c': put_char((char)va_arg(args,int)); break;
                case 's': put_string(va_arg(args,const char*)); break;
                case 'd': {
                    int val = va_arg(args,int);
                    char buf[12];
                    int j=0;
                    if (val==0) buf[j++]='0';
                    else {
                        if(val<0){ put_char('-'); val=-val; }
                        int tmp=val; while(tmp){ tmp/=10; j++; }
                        buf[j]='\0';
                        while(val){ buf[--j]='0'+(val%10); val/=10; }
                    }
                    put_string(buf);
                    break;
                }
                case 'x': {
                    unsigned int val=va_arg(args,unsigned int);
                    char buf[9]; buf[8]='\0';
                    for(int k=7;k>=0;k--){ int n=val&0xF; buf[k]=n<10?'0'+n:'A'+(n-10); val>>=4; }
                    put_string(buf);
                    break;
                }
            }
        } else put_char(fmt[i]);
    }
    va_end(args);
}
