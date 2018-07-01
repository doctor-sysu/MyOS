#include <myos/kernel/Keyboard.hpp>
#include <myos/kernel/FAT12.hpp>
#include "Syscall.hpp"

//using myos::kernel::Process;
using myos::kernel::PCB;
//using myos::kernel::FAT12::FAT12;
using myos::kernel::Syscall::syscall;

extern myos::kernel::Process processes;
extern myos::kernel::Keyboard keyboard;
extern myos::kernel::Terminals terminal;
extern bool finishReadSector;
extern uint32_t driving;

extern "C" void cpu_enterUserCode(uint32_t address, uint32_t stack_base);

namespace myos {
namespace kernel {
namespace interrupt {

//keyboard interrupt
//void __cpp_create_new_process() {
//    static unsigned int now_process = 0;
//    keyboard.kb_in();
//    if (keyboard.size() < 2) return;
//    if (now_process >= 3)
//        return;
//    char *userName = const_cast<char *>("LETTER0 EXE\0");
//    userName[6] = static_cast<char>(49 + now_process);
//    uintptr_t load = reinterpret_cast<uintptr_t>
//    (myos::kernel::FAT12::FAT12(userName));
//    unsigned int entry;
//    //加载用户程序
//    if (load > 0) {
//        //entry = *(unsigned int *) ((unsigned int *) load + 0x18);
//        processes.create(*(reinterpret_cast<uint32_t *>(load + 0x18)));
//        //((void (*)()) entry)();
//    }
//    now_process++;
//    keyboard.clean();
//}

//page fault
void PageFaultHandle(myos::kernel::PCB *progress){
    uint32_t cr3,cr2;
    asm volatile(
            "mov %0, cr2\n"
            "mov %1, cr3\n"
            :"=r"(cr2),"=r"(cr3)
            );
    switch (progress->Error_code){
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        default:
            break;
    }
}

//clock interrupt
void callprocess(myos::kernel::PCB *progress) {
    if (!driving)
        processes.exchange(progress);
}

//aimming keyboard interrupt
void keyboard_input() {
    keyboard.kb_in();
    uint8_t scancode = keyboard.kb_read();
    kernel_keyboard_key_Key key = kernel_keyboard_scanCode_toKey(scancode);
    if (scancode < 5 && scancode > 1)
        terminal.switch_video_page(scancode - 1);
    char *videomem = reinterpret_cast<char *>(0xb8000 + 2770);
    keyboard.kb_read();
    //*(videomem) = charater;
}

//floppy interrupt
void read_finished() {
    finishReadSector = true;
}

void enterKernelUser(PCB *progress) {
    char name[13] = "KERP    EXE\0";

    uintptr_t userCR3 = processes.create(name);
    //myos::kernel::FAT12::FAT12(name, userCR3);
    
    //enter the kernel process, this will not exit except terminate the OS
    //cpu_enterUserCode(entry, userStack);
    processes.exchange(progress);
}


void _interruptHandle(uint32_t interruptNumber, PCB *progress) {
    switch (interruptNumber) {
        case 0xe:   //page fault
            PageFaultHandle(progress);
            break;
        case 0x20:  //clock interrupt
            callprocess(progress);
            break;
        case 0x21:
            keyboard_input();
            break;
        case 0x26:  //floppy interrupt
            read_finished();
            break;
        case 0x7f:  //call kernel process
            enterKernelUser(progress);
            break;
        case 0x80:  //syscall
            syscall(progress);
            break;
        default: //default handle
            break;
    }
}

}
}
}

extern "C" void interruptHandle(uint32_t interruptNumber, myos::kernel::PCB *process) {
    myos::kernel::interrupt::_interruptHandle(interruptNumber, process);
}

//printA
//static char *start = reinterpret_cast<char *>(0xb8000);
//static int x = 0;
//static int y = 0;
//static int count = 40;
//static char letter = 'A' - 1;
//
////printOuch
//static int flag = 5;
//
//extern "C" void printA() {
//    --count;
//    if (count) {
//        return;
//    }
//    if (x == 0 && y == 0) {
//        //clear the screen
//        for (int i = 0; i < 2000; ++i)
//            *(start + 2 * i) = ' ';
//        if (letter == 'Z')
//            letter = 'A';
//        else
//            ++letter;
//    }
//    char *videomemory = start + 2 * (80 * y + x);
//    *(videomemory) = letter;
//    if (y == 0 && x < 79)
//        ++x;
//    else if (x == 79 && y < 24)
//        ++y;
//    else if (y == 24 && x > 0)
//        --x;
//    else
//        --y;
//    count = 40;
//    //clear "OUCH!"
//    if (flag == 0)
//        return;
//    else if (flag == 1) {
//        char *video = reinterpret_cast<char *>(0xb8000 + 2 * (13 * 80 + 35));
//        for (int i = 0; i < 10; ++i) {
//            *(video) = ' ';
//            video += 2;
//        }
//        flag = 0;
//    } else if (flag > 1)
//        --flag;
//}
//
//extern "C" void printOuch() {
//    char *videomem = reinterpret_cast<char *>(0xb8000 + 2 * (13 * 80 + 35));
//    char str[11] = "OUCH!OUCH!";
//    for (int i = 0; i < 10; ++i) {
//        *(videomem) = str[i];
//        videomem += 2;
//    }
//    flag = 10;
//}