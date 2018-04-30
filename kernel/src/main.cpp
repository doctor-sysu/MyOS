#include <myos/kernel/IDT.hpp>
#include <myos/kernel/FAT12.hpp>
#include <myos/kernel/Process.hpp>

extern "C" void cpu_initialize();
extern uint32_t kernel_sp;

//void showA() {
//    char *video = reinterpret_cast<char *>(0xb8000);
//    *(video) = 'A';
//}

using myos::kernel::FAT12::FAT12;
using myos::kernel::FAT12::Load_RD;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

extern "C" int main() {
    cpu_initialize();
    myos::kernel::IDT idt;
    asm volatile(
        "mov %0, esp"
        :"=r"(kernel_sp)
        );
    idt.Install();
    //char *video = reinterpret_cast<char *>(0xb8000);
    //*(video) = 'A';
    Load_RD();

    while (true) {
        asm volatile("nop");
        //((void (*)()) entry)();
    }

    return 0;
}

#pragma clang diagnostic pop

#pragma clang diagnostic pop
