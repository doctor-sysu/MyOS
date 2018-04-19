BITS 32
global callinterrupt

[SECTION .text]
callinterrupt:
    push eax
    mov eax, 0xb8000
    mov byte [eax], 'A'
    pop eax
    iret
