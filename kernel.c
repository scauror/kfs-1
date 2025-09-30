#include "io.h"
#include "keyboard.h"
#include "utils.h"

void kernel_main(void){
    io_init();
    keyboard_init();

    printk("Hello KFS!\nTry typing something:\n");

    while(1){
        keyboard_poll();
        if (keyboard_haschar()) {
            char c = keyboard_getchar();
            put_char(c);
        }
    }
}
