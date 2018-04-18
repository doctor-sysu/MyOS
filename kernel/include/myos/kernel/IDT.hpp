#pragma once

#include <cstdint>

namespace myos {
namespace kernel {

class IDT
{
private:
#pragma pack(push, 1)
    struct IDTEntry {
        uint16_t LowOffset;
        uint16_t selector;
        unsigned char zero;
        unsigned char segment_type:4;
        unsigned char system_segment_flag:1;
        unsigned char DPL:2;
        unsigned char P:1;
        uint16_t HiOffset;
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

