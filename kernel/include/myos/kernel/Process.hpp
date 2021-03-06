#pragma once

#include <cstdint>
#include <myos/kernel/MemoryManage.hpp>

namespace myos{
namespace kernel {

#define SIZE_OF_PCBList 30

#pragma pack(push, 1)

struct PCB {
    PCB() : gs(0), fs(0), es(0), ds(0), ss(0), edi(0), esi(0), ebp(0),
            ebx(0), eax(0), ecx(0), edx(0), Error_code(0), eip(0),
            cs(0), eflags(0), esp(0) {}

    PCB &operator=(const PCB &copy) {
            gs = copy.gs;
            fs = copy.fs;
            es = copy.es;
            ds = copy.ds;
            ss = copy.ss;
            edi = copy.edi;
            esi = copy.esi;
            ebp = copy.ebp;
            ebx = copy.ebx;
            eax = copy.eax;
            ecx = copy.ecx;
            edx = copy.edx;
            Error_code = copy.Error_code;
            eip = copy.eip;
            cs = copy.cs;
            eflags = copy.eflags;
            esp = copy.esp;
    }

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

    uint32_t Error_code;
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t esp;
    uint32_t ss;
};

struct Processblock
{
    PCB pcb;
    int priority;
    int video_page;     //the index of this progress's tty
    int pid;        //progress's id
    PageDirectoryEntry* CR3;    //process's cr0
};

#pragma pack(pop)
class Process{
public:
    uintptr_t create(char*);
    void exchange(PCB*);
    void initial();
    void kill(PCB*);        //kill the running process
    const int32_t get_running_page();
    Process();
private:
//    PCB PCBList[SIZE_OF_PCBList + 1];
//    UserMemoryManage MemoryList[SIZE_OF_PCBList + 1];
    void change(PCB*);
    Processblock PCBList[SIZE_OF_PCBList + 1];
    int32_t running;
    int32_t Process_Count;
};

void changeCR3(uintptr_t);

}
}