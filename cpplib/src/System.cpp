#include <System.hpp>

void SysC(int Funmem){
    asm volatile(
    "int 0x80\n"
    ::"a"(Funmem)
    );
}