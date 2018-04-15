#include <myos/kernel/IDT.hpp>

namespace myos {
namespace kernel {

IDT::IDT() {
    interrupt_count = 0;
    idtr.base = reinterpret_cast<uint32_t>(idt);
    idtr.limit = sizeof(IDTEntry) * 64;
    for (int i = 0; i < 64; ++i) {
        idt[i].flags = 0;
        idt[i].offset1 = 0;
        idt[i].offset2 = 0;
        idt[i].selector = 0;
        idt[i].zero = 0;
    }
}

//lowest 2 bit of dpl is valid
void IDT::AddInterrupt(uint8_t interrupt, uint8_t dpl, void *handler) {
    uint32_t pt;
    pt = (uint32_t)handler;
    uint16_t offset1 = pt & 0xffff;
    pt >> 16;
    uint16_t offset2 =  pt & 0xffff;
    idt[interrupt_count].offset1 = offset1;
    idt[interrupt_count].offset2 = offset2;
    ++interrupt_count;
}

}
}
