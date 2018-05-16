BITS 32

global Syscall
extern _syscall

[SECTION .text]
Syscall:
    ;protect register
        push gs
        push fs
        push es
        push ds
        push ebp
        push edi
        push esi
        push edx
        push ecx
        push ebx
        push eax

        call _syscall

        ;reset register
        pop eax
        pop ebx
        pop ecx
        pop edx
        pop esi
        pop edi
        pop ebp
        pop ds
        pop es
        pop fs
        pop gs

        iret