BITS 32

global Clock_interrupt
extern printA
global Keyboard_interrupt
;extern printOuch
extern create_new_process
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
global Syscall
extern callprocess

extern kernel_sp

[SECTION .text]
Syscall:
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

    cmp eax, 1
    je Fun1
    cmp eax, 2
    je Fun2
    cmp eax, 3
    je Fun3
    jmp Fun4

Fun1:
    int 30h
    jmp Protect_reg

Fun2:
    int 31h
    jmp Protect_reg

Fun3:
    int 32h
    jmp Protect_reg

Fun4:
    int 33h
    jmp Protect_reg

Protect_reg:
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

    call create_new_process

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
    mov eax, esp
    add eax, 12
    ;mov kernel_sp, eax
    pop eax
    ;set kernel_sp prepare for process call

    sti ; Re-enable interrupts

    iret

Clock_interrupt:
    ;protect register

    call Turn_to_kernel_stack

    ;now the it is the kernel's stack
    push edx
    push ecx
    push eax
    push ebx
    push ebp
    push esi
    push edi
    push ds
    push es
    push fs
    push gs
    ;call the handler
    call callprocess
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

    call Turn_to_user_stack

    ; send eoi
    push eax
    in al, 60h
    mov al, 0x20
    out 0x20, al
    pop eax
    sti ; re-enable interrupts
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

     call print_Hello

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


 User_Int4:
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

     call print_World

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

Turn_to_kernel_stack:
    push eax
    mov eax, dword [esp+4]  ;put eip to kernel stack
    mov dword [kernel_sp-16], eax
    mov eax, dword [esp +8] ;put cs to kernel stack
    mov dword [kernel_sp-12], eax
    mov eax, dword [esp +12]    ;put eflags to kernel stack
    mov dword [kernel_sp-8], eax
    mov eax, esp
    add eax, 12
    mov dword [kernel_sp - 4], eax
    mov eax, kernel_sp
    sub eax, 20
    mov esp, eax
    pop eax
    ret

Turn_to_user_stack:
    push eax
    mov eax, dword [esp + 16]
    mov esp, eax
    mov eax, dword [kernel_sp - 8]
    push eax
    mov eax, dword [kernel_sp - 12]
    push eax
    mov eax, dword [kernel_sp - 16]
    pop eax
    ret