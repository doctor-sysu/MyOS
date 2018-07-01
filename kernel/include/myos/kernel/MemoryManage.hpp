#pragma once

#include <cstdint>

namespace myos{
namespace kernel{

#define PAGE_SIZE 4096
#define BUNDARY 10
#define OFFSET 12
#define USER_START 0x300

struct PageDirectoryEntry{
    bool P:1;
    bool R_W:1;   //0 = read only , 1 = read and write
    bool U_S:1;   //Privilege Level
    bool PWT:1;   //write-through
    bool PCD:1;   //Cache 0 = enable , 1 = disabel
    bool A:1;     //Accessed
    bool reserved:1;
    bool PS:1;    //Page size 1 = 4MB. 0 = 4KB
    bool G:1;     //Global
    uint8_t Avail:3;
    uintptr_t PageTableAddress:20;
};

struct PageTableEntry{
    bool P:1;
    bool R_W:1;   //0 = read only , 1 = read and write
    bool U_S:1;   //Privilege Level
    bool PWT:1;   //write-through
    bool PCD:1;   //Cache 0 = enable , 1 = disabel
    bool A:1;     //Accessed
    bool D:1;     //Dirty
    bool PAT:1;    // SET IT 0
    bool G:1;     //Global
    uint8_t Avail:3;
    uintptr_t PageTable:20;
};

//struct UserMemoryManage{
//    PageDirectoryEntry* PDE = nullptr;
//    uint32_t numberOfPage = 0;
//};

class MemoryManager{
public:
    void initial();
    uintptr_t PageDirectoryAllocate();
    void copyKernelPageTable(PageDirectoryEntry*,bool);
    void allocate(uint32_t,PageDirectoryEntry*,uintptr_t ,bool);
    void free(PageDirectoryEntry*);
    uint32_t getPage(uintptr_t,PageDirectoryEntry*);
private:
    uint32_t allocateOnePage();     //return the Page Entry
    uint32_t freeOnePage(uintptr_t);
    PageDirectoryEntry* internalPDE;
    PageDirectoryEntry* PDE;
};


}
}