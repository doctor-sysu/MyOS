#include <myos/kernel/IDT.hpp>
#include <myos/kernel/FAT12.hpp>
#include <myos/kernel/Process.hpp>

extern "C" void cpu_initialize();

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
    idt.Install();
    //char *video = reinterpret_cast<char *>(0xb8000);
    //*(video) = 'A';
    Load_RD();

//    void *load = reinterpret_cast<void *>(FAT12((char) 1));
//    uint32_t entry;
//    //加载用户程序
//    if (load >= 0) {
//        entry = *(uint32_t *) ((uint8_t *) load + 0x18);
//        ((void (*)()) entry)();
//    }

    while (true) {
        asm volatile("nop");
        //((void (*)()) entry)();
    }

    return 0;
}

#pragma clang diagnostic pop

#pragma clang diagnostic pop
