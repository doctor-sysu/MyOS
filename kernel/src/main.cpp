#include <myos/kernel/IDT.hpp>
#include <myos/kernel/FAT12.hpp>
#include <myos/kernel/Process.hpp>
#include <System.hpp>

extern "C" void cpu_initialize();
extern myos::kernel::Process processes;

//void showA() {
//    char *video = reinterpret_cast<char *>(0xb8000);
//    *(video) = 'A';
//}

using myos::kernel::FAT12::FAT12;
using myos::kernel::FAT12::Load_RD;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

void sleep() {
    for (int i = 0; i < 100000; ++i) {
        asm volatile("nop");
    }
}

void LoadKernelProcess(){
    asm volatile(
            "int 0x7f"
            );
}

void Clear_Screen()
{
    for(int i = 0; i < 40; ++i)
    {
        for(int j = 0; j < 80; ++j)
            *(reinterpret_cast<char*>(0xb8000 + 2*(80 * i + j))) = ' ';
    }
}

extern "C" int main() {
    cpu_initialize();
    myos::kernel::IDT idt;
    idt.Install();
    processes.initial();
    //char *video = reinterpret_cast<char *>(0xb8000);
    //*(video) = 'A';
    Load_RD();
    Clear_Screen();
    LoadKernelProcess();
//    for(int i = 0; i < 100000; ++i) {
//        for (int j = 1; j < 5; ++j)
//               SysC(j);
//        sleep();
//    }

    while (true) {
        asm volatile("nop");
        //((void (*)()) entry)();
    }

    return 0;
}

#pragma clang diagnostic pop

#pragma clang diagnostic pop
