#pragma once

#include <cstdint>

namespace myos {
namespace kernel {

class IDT
{
private:
#pragma pack(push, 1)
    struct IDTEntry {
        uint16_t offset1;
        uint16_t selector;
        uint8_t zero;
        uint8_t flags;
        uint16_t offset2;
    };

    struct IDTR {
        uint16_t limit;
        uint32_t base;
    };

#pragma pack(pop)
private:
    IDTR idtr;
    IDTEntry idt[64];
public:
    IDT();
    void AddInterrupt(uint8_t interrupt, uint8_t dpl, void(* handler));

};


}
}

