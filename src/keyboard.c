#include "keyboard.h"
#include "utils.h"
#include "io.h"
#include <stddef.h>
#include <stdint.h>

#define KB_PORT 0x60
#define KB_STATUS 0x64

static char kb_buffer[KB_BUFFER_SIZE];
static size_t kb_head = 0;
static size_t kb_tail = 0;

static const char scancode_to_ascii[128] = {
    0,27,'1','2','3','4','5','6','7','8','9','0','-','=','\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',0,
    'a','s','d','f','g','h','j','k','l',';','\'','`',0,'\\',
    'z','x','c','v','b','n','m',',','.','/',0,'*',0,' ',0
};

void keyboard_init(void) {
    kb_head = kb_tail = 0;
}

int keyboard_haschar(void) {
    return kb_head != kb_tail;
}

char keyboard_getchar(void) {
    while (kb_head == kb_tail) { __asm__("hlt"); }
    char c = kb_buffer[kb_tail];
    kb_tail = (kb_tail + 1) % KB_BUFFER_SIZE;
    return c;
}

void keyboard_poll(void) {
    while (inb(KB_STATUS) & 0x01) {
        uint8_t sc = inb(KB_PORT);
        if (sc < 128 && scancode_to_ascii[sc]) {
            size_t next = (kb_head + 1) % KB_BUFFER_SIZE;
            if (next != kb_tail) {
                kb_buffer[kb_head] = scancode_to_ascii[sc];
                kb_head = next;
            }
        }
    }
}
