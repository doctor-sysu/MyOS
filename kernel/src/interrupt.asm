BITS 32
global callinterrupt

[SECTION .text]
callinterrupt:

global Clock_interrupt
extern printA
global Keyboard_interrupt
extern printOuch
global Floppy_interrupt
extern read_finished
global User_Int1
extern windmill
global User_Int2
extern print_info
global User_Int3
extern print_Hello
global User_Int4
extern print_World


[SECTION .text]
Keyboard_interrupt:
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

    call printOuch

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

    ; send EOI
    push eax
    mov al, 0x20
    out 0x20, al
    pop eax
    sti ; Re-enable interrupts

    iret

Clock_interrupt:
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
    ;call the handler
    call printA
    int 30h
    int 31h
    int 32h
    int 33h
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
    ; send EOI
    push eax
    in al, 60h
    mov al, 0x20
    out 0x20, al
    pop eax
    sti ; Re-enable interrupts
    iret

Floppy_interrupt:
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

    call read_finished

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

User_Int1:
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

    call windmill

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

 User_Int2:
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

     call print_info

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

User_Int3:
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
    mov eax, 0xb8000
    mov byte [eax], 'A'
    pop eax
    iret
