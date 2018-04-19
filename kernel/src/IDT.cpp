#include <myos/kernel/IDT.hpp>

namespace myos {
namespace kernel {

void defaulthandler() {}


IDT::IDT() {
    idtr.base = (uint32_t) &idt[0];
    idtr.limit = sizeof(IDTEntry) * 64;
    for (int i = 0; i < 64; ++i) {
        idt[i].Reserved = 0;

        idt[i].LowOffset = static_cast<uint16_t>(
                reinterpret_cast<uint32_t>(defaulthandler) & 0x0000FFFFu);
        idt[i].HiOffset = static_cast<uint16_t>(
                (reinterpret_cast<uint32_t>(defaulthandler) & 0xFFFF0000u) >> 16);
        idt[i].Selector = 0;
        idt[i].Reserved = 0;
        idt[i].Segment_type = 0;
        idt[i].System_segment_flag = 0;
        idt[i].DPL = 0;
        idt[i].P = 0;
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
    asm volatile (
    "lidt [%0]\n"
    "sti\n"
    : : "r"(&idtr)
    );

}


}
}
