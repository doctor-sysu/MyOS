#pragma once

#include <cstdint>

namespace myos{
namespace kernel{

#pragma pack(push, 1)
struct PCB {
    uint32_t gs;
    uint32_t fs;
    uint32_t es;
    uint32_t ds;
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;

    uint32_t ebx;
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;

    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t esp;
};

#pragma pack(pop)
class Process{
public:
    bool create();
    void exchange(PCB*&);
private:
    Process();

    PCB PCBList[10];
    unsigned int Process_Count;
};

}
}