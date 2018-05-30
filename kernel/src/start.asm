BITS 32

global kernel_start
extern main

[SECTION .text]

kernel_start:
    mov ebp, 0x1fffff
    mov esp, ebp

    call main

    cli
    hlt
