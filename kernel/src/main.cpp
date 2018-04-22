#include <myos/kernel/IDT.hpp>
#include <myos/kernel/FAT12.hpp>

extern "C" void cpu_initialize();

extern "C" void callinterrupt();

void showA() {
    char *video = reinterpret_cast<char *>(0xb8000);
    *(video) = 'A';
}

using myos::kernel::FAT12::FAT12;
using myos::kernel::FAT12::Load_RD;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
extern "C" int main() {
    cpu_initialize();
    myos::kernel::IDT idt;
    idt.AddInterrupt(32, 0, callinterrupt);
    idt.Install();
    //char *video = reinterpret_cast<char *>(0xb8000);
    //*(video) = 'A';
    Load_RD();
    
    int load = FAT12((char)1);
    //加载用户程序
    if (load >= 0)
        ((void (*)()) load)();
    while (true) {
        asm volatile("nop");
    }

    return 0;
}
#pragma clang diagnostic pop


#pragma clang diagnostic pop
