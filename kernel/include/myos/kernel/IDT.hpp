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
//        unsigned char Segment_type:4;
//        unsigned char System_segment_flag:1;
//        unsigned char DPL:2;
//        unsigned char P:1;
        unsigned char flag;
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