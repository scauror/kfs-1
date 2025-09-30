#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

#define KB_BUFFER_SIZE 128

void keyboard_init(void);
char keyboard_getchar(void);
int keyboard_haschar(void);
void keyboard_poll(void);

#endif
