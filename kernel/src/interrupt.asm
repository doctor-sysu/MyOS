BITS 32

global Clock_interrupt
;extern printA
global Keyboard_interrupt
;extern printOuch
extern keyboard_input
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
global SC_printSomething
extern callprocess
global New_Process
extern create_new_process
global End_Process
extern kill_process


extern tss_interrupt
extern driving

[SECTION .text]
SC_printSomething:
    pop eax
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
    ;protect register
    cli
    cmp dword [driving], 0
    jnz .End_keyboard_interrupt_handle
    ;Turn_to_kernel_stack
        push eax
        push ebx
        mov ebx, dword [tss_interrupt]
        mov eax, dword [esp+8]  ;put eip to interrupt stack
        mov dword [ebx-16], eax
        mov eax, dword [esp +12] ;put cs to interrupt stack
        mov dword [ebx-12], eax
        mov eax, dword [esp +16]    ;put eflags to interrupt stack
        mov dword [ebx-8], eax
        mov eax, esp
        add eax, 8
        mov dword [ebx - 4], eax
        pop ebx
        pop eax
        ;mov eax, ebx
        ;sub eax, 16
        mov esp, dword [tss_interrupt]
        sub esp, 16

    ;now the it is the kernel's stack
    push 0  ;Error_code
    push edx
    push ecx
    push eax
    push ebx
    push ebp
    push esi
    push edi
    push ss
    push ds
    push es
    push fs
    push gs
    push esp

    ;call print_info
    ;call keyboard_input
    ;int 0x30
    call create_new_process

    ;reset register
    add esp, 4
    pop gs
    pop fs
    pop es
    pop ds
    pop ss
    pop edi
    pop esi
    pop ebp
    pop ebx
    pop eax
    pop ecx
    pop edx
    add esp, 4 ;pop Error_code

    ;Turn_to_user_stack

        mov esp, dword [esp + 12]
        push eax
        push ebx
        mov ebx, dword [tss_interrupt]
        mov eax, dword [ebx - 8]    ;eflags
        mov dword [esp + 16], eax
        mov eax, dword [ebx - 12]   ;cs
        mov dword [esp + 12], eax
        mov eax, dword [ebx - 16]   ;eip
        mov dword [esp + 8], eax
        pop ebx
        pop eax

    .End_keyboard_interrupt_handle:
    ; send EOI
    push eax
    in al, 60h
    mov al, 0x20
    out 0x20, al
    pop eax
    ;set kernel_sp prepare for process call

    sti ; Re-enable interrupts
    iret

Clock_interrupt:
    ;protect register
    cli
    cmp dword [driving], 0
    jnz .End_clock_interrupt_handle

    ;Turn_to_kernel_stack
        push eax
        push ebx
        mov ebx, dword [tss_interrupt]
        mov eax, dword [esp+8]  ;put eip to interrupt stack
        mov dword [ebx-16], eax
        mov eax, dword [esp +12] ;put cs to interrupt stack
        mov dword [ebx-12], eax
        mov eax, dword [esp +16]    ;put eflags to interrupt stack
        mov dword [ebx-8], eax
        mov eax, esp
        add eax, 8
        mov dword [ebx - 4], eax
        pop ebx
        pop eax
        ;mov eax, ebx
        ;sub eax, 16
        mov esp, dword [tss_interrupt]
        sub esp, 16

    ;now the it is the kernel's stack
    push 0  ;Error_code
    push edx
    push ecx
    push eax
    push ebx
    push ebp
    push esi
    push edi
    push ss
    push ds
    push es
    push fs
    push gs
    push esp
    ;call the handler
    call callprocess
    ;reset register
    add esp, 4
    pop gs
    pop fs
    pop es
    pop ds
    pop ss
    pop edi
    pop esi
    pop ebp
    pop ebx
    pop eax
    pop ecx
    pop edx
    add esp, 4 ;pop Error_code

    ;Turn_to_user_stack

        mov esp, dword [esp + 12]
        push eax
        push ebx
        mov ebx, dword [tss_interrupt]
        mov eax, dword [ebx - 8]    ;eflags
        mov dword [esp + 16], eax
        mov eax, dword [ebx - 12]   ;cs
        mov dword [esp + 12], eax
        mov eax, dword [ebx - 16]   ;eip
        mov dword [esp + 8], eax
        pop ebx
        pop eax

    .End_clock_interrupt_handle:
    ; send eoi
    push eax
    in al, 60h
    mov al, 0x20
    out 0x20, al
    pop eax
    sti ; re-enable interrupts
    iret

Floppy_interrupt:
    cli
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
     ;call print_info

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

New_Process:
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

    call create_new_process

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

End_Process:
;protect register
    cli
;.wait_to_continue:
;    cmp dword [driving], 0
;    jnz .wait_to_continue
    ;Turn_to_kernel_stack
        push eax
        push ebx
        mov ebx, dword [tss_interrupt]
        mov eax, dword [esp+8]  ;put eip to interrupt stack
        mov dword [ebx-16], eax
        mov eax, dword [esp +12] ;put cs to interrupt stack
        mov dword [ebx-12], eax
        mov eax, dword [esp +16]    ;put eflags to interrupt stack
        mov dword [ebx-8], eax
        mov eax, esp
        add eax, 8
        mov dword [ebx - 4], eax
        pop ebx
        pop eax
        mov esp, dword [tss_interrupt]
        sub esp, 16

    ;now the it is the kernel's stack
    push 0  ;Error_code
    push edx
    push ecx
    push eax
    push ebx
    push ebp
    push esi
    push edi
    push ss
    push ds
    push es
    push fs
    push gs
    push esp

    call kill_process

;reset register
    add esp, 4
    pop gs
    pop fs
    pop es
    pop ds
    pop ss
    pop edi
    pop esi
    pop ebp
    pop ebx
    pop eax
    pop ecx
    pop edx
    add esp, 4 ;pop Error_code

    ;Turn_to_user_stack

        mov esp, dword [esp + 12]
        push eax
        push ebx
        mov ebx, dword [tss_interrupt]
        mov eax, dword [ebx - 8]    ;eflags
        mov dword [esp + 16], eax
        mov eax, dword [ebx - 12]   ;cs
        mov dword [esp + 12], eax
        mov eax, dword [ebx - 16]   ;eip
        mov dword [esp + 8], eax
        pop ebx
        pop eax

     sti
     iret

; Below are the auxiliary functions

;Turn_to_kernel_stack:
;    push eax
;    mov eax, dword [esp+4]  ;put eip to kernel stack
;    mov dword [kernel_sp-16], eax
;    mov eax, dword [esp +8] ;put cs to kernel stack
;    mov dword [kernel_sp-12], eax
;;    mov eax, dword [esp +12]    ;put eflags to kernel stack
;    mov dword [kernel_sp-8], eax
;    mov eax, esp
;    add eax, 12
;    mov dword [kernel_sp - 4], eax
 ;   mov eax, kernel_sp
;    sub eax, 20
;;    mov esp, eax
;    pop eax
;    ret
;
;Turn_to_user_stack:
;    push eax
;    mov eax, dword [esp + 16]
;    mov esp, eax
 ;   mov eax, dword [kernel_sp - 8]
 ;   push eax
 ;   mov eax, dword [kernel_sp - 12]
 ;   push eax
 ;   mov eax, dword [kernel_sp - 16]
 ;   pop eax
 ;   ret;