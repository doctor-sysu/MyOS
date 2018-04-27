BITS 32

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

    call Turn_to_kernel_stack

    ;now the it is the kernel's stack
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
    ;int 30h
    ;int 31h
    ;int 32h
    ;int 33h
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

    ; send EOI
    push eax
    mov al, 0x20
    out 0x20, al
    pop eax
    sti ; Re-enable interrupts

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


; Below are the auxiliary functions
extern kernel_sp
Turn_to_kernel_stack:
    push eax
    mov eax, dword [esp+4]  ;put eip to kernel stack
    mov dword [kernel_sp], eax
    mov eax, dword [esp +8] ;put cs to kernel stack
    mov dword [kernel_sp], eax
    mov eax, dword [esp +12]    ;put eflags to kernel stack
    mov dword [kernel_sp], eax
    mov eax, esp
    add eax, 12
    mov dword [kernel_sp + 16], eax
    mov eax, kernel_sp
    add eax, 16
    mov dword [esp], eax
    pop eax
    ret