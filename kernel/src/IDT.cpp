#include <myos/kernel/IDT.hpp>

namespace myos {
namespace kernel {

extern "C" void Clock_interrupt();
extern "C" void Keyboard_interrupt();
extern "C" void Floppy_interrupt();
extern "C" void User_Int1();
extern "C" void User_Int2();
extern "C" void User_Int3();
extern "C" void User_Int4();
extern "C" void Syscall();

void defaulthandler() {}


IDT::IDT() {
    idtr.base = (uint32_t) &idt[0];
    idtr.limit = sizeof(IDTEntry) * MAXHandler;
    for (int i = 0; i < MAXHandler; ++i) {
        idt[i].Reserved = 0;

        idt[i].LowOffset = static_cast<uint16_t>(
                reinterpret_cast<uint32_t>(defaulthandler) & 0x0000FFFFu);
        idt[i].HiOffset = static_cast<uint16_t>(
                (reinterpret_cast<uint32_t>(defaulthandler) & 0xFFFF0000u) >> 16);
        idt[i].Selector = 0x08;
        idt[i].flag = 0b10001110;
    }
}

//lowest 2 bit of dpl is valid
void IDT::AddInterrupt(uint8_t interrupt, uint8_t dpl, void (*handler)()) {
    uint32_t pt;
    pt = (uint32_t) handler;
    uint16_t offset1 = static_cast<uint16_t>(pt & 0x0000ffffu);
    pt = pt >> 16;
    uint16_t offset2 = static_cast<uint16_t>(pt & 0x0000ffffu);
    idt[interrupt].LowOffset = offset1;
    idt[interrupt].HiOffset = offset2;
}

void IDT::Install() {
    AddInterrupt(32, 0, Clock_interrupt);
    AddInterrupt(33, 0, Keyboard_interrupt);
    AddInterrupt(38, 0, Floppy_interrupt);
    AddInterrupt(48, 0, User_Int1);
    AddInterrupt(49, 0, User_Int2);
    AddInterrupt(50, 0, User_Int3);
    AddInterrupt(51, 0, User_Int4);
    AddInterrupt(0x80, 0, Syscall);

    asm volatile (
    "lidt [%0]\n"
    "sti\n"
    : : "r"(&idtr)
    );
}

}
}