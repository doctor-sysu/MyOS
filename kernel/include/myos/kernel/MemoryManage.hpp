#pragma once

#include <cstdint>

namespace myos{
namespace kernel{

#define PAGE_SIZE 4096
#define BUNDARY 256

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
    uint32_t PageTableAddress:20;
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
    uint32_t PageTable:20;
};

struct UserMemoryManage{
    PageDirectoryEntry* PDE = nullptr;
    uint32_t numberOfPage = 0;
};

class MemoryManager{
public:
    void initial();
    uint32_t PageDirectoryAllocate();
    void allocate(uint32_t,UserMemoryManage*, bool);
    void free(UserMemoryManage*);
private:
    uint32_t allocateOnePage();
    uint32_t freeOnePage(uint32_t);
    PageDirectoryEntry* PDE;
};


}
}