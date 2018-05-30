#pragma once

#include <cstdint>

namespace myos {
namespace kernel {
#define MAXHandler 256

class IDT {
private:
#pragma pack(push, 1)
    struct IDTEntry {
        uint16_t LowOffset;
        uint16_t Selector;
        unsigned char Reserved;
        unsigned char flag; //(P-1) : (DPL-2) : (S-1) : (TYPE-4)
        uint16_t HiOffset;
    };

    struct IDTR {
        uint16_t limit;
        uint32_t base;
    };

#pragma pack(pop)
private:
    IDTR idtr;
    IDTEntry idt[MAXHandler];
public:
    IDT();

    void Install();

    void AddInterrupt(uint8_t interrupt, uint8_t dpl, void(*handler)());

};


}
}