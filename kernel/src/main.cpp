#include <myos/kernel/IDT.hpp>

extern "C" void cpu_initialize();

extern "C" void callinterrupt();

void showA() {
    char *video = reinterpret_cast<char *>(0xb8000);
    *(video) = 'A';
}


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
extern "C" int main() {
    cpu_initialize();
    myos::kernel::IDT idt;
    idt.AddInterrupt(32, 0, callinterrupt);
    idt.Install();
    //char *video = reinterpret_cast<char *>(0xb8000);
    //*(video) = 'A';
    while (true) {
        asm volatile("nop");
    }

    return 0;
}
#pragma clang diagnostic pop
