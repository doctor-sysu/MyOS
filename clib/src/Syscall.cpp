#include"Syscall.h"

void Syscall(int FunNum) {
    asm volatile(
    "int 0x80\n"
    : : "a" (FunNum)
    );
    //asm volatile(
    //"mov %0, eax\n"
    //: : "eax"
    //);
}

