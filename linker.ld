/* linker.ld */
ENTRY(_start)

SECTIONS {
    . = 1M; /* charge le kernel à 1MB */

    .text : {
        *(.multiboot)
        *(.text*)
    }

    .rodata : {
        *(.rodata*)
    }

    .data : {
        *(.data*)
    }

    .bss : {
        *(.bss*)
        *(COMMON)
    }
}
