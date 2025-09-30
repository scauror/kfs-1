; boot.s
section .multiboot
    align 4
    dd 0x1BADB002 ; magic number for the bootloader
    dd 0x00 ; no flags asked
    dd -(0x1BADB002 + 0x00) ; checksum : if this equals 0 -> the header is valid

section .bss ; this section allocates the stack
    align 4
stack_bottom:    resb 16384
stack_top:

section .text
    global _start
    extern kernel_main

_start:
    mov esp, stack_top
    call kernel_main

.hang:
    hlt
    jmp .hang
