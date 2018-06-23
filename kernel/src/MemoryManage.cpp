#include <myos/kernel/MemoryManage.hpp>

namespace myos {
namespace kernel {

MemoryManager memManage;

void MemoryManager::initial() {
    //uint32_t bundary = 0x100;
    PDE = reinterpret_cast<PageDirectoryEntry *>(0x200000);
    for (auto i = 0; i < BUNDARY; i++) {
        auto now = PDE + i;
        now->P = 0;
        now->R_W = 0;
        now->U_S = 0;
        now->PWT = 0;
        now->PCD = 0;
        now->A = 0;
        now->reserved = 0;
        now->PS = 0;
        now->G = 0;
        now->Avail = 0;
        now->PageTableAddress = static_cast<uint32_t>((0x200000 >> OFFSET) + (i + 1));
        PageTableEntry *PTE = reinterpret_cast<PageTableEntry *>(now->PageTableAddress << OFFSET);
        for (uint32_t j = 0; j < 1024; j++) {
            auto pte = PTE + j;
            pte->P = 0;
            pte->R_W = 0;
            pte->U_S = 0;
            pte->PWT = 0;
            pte->PCD = 0;
            pte->A = 0;
            pte->D = 0;
            pte->PAT = 0;
            pte->G = 0;
            pte->Avail = 0;
            pte->PageTable = (0x200000 >> OFFSET) + 1 + BUNDARY
                             + 1024 * i + j;
        }
    }
}

uintptr_t MemoryManager::PageDirectoryAllocate() { // return user's cr3
    uintptr_t address = allocateOnePage();
    PageDirectoryEntry* pde = reinterpret_cast<PageDirectoryEntry *>(address);
    for (uint32_t j = 0; j < BUNDARY; j++) {
        pde->P = 0;
        pde->R_W = 0;
        pde->U_S = 1;
        pde->PWT = 0;
        pde->PCD = 0;
        pde->A = 0;
        pde->reserved = 0;
        pde->PS = 0;
        pde->G = 0;
        pde->Avail = 0;
        pde->PageTableAddress = 0;
    }
    return address;
}

//the frame address
void MemoryManager::allocate(uint32_t size, PageDirectoryEntry *cr3
        ,uintptr_t logicalAddress, bool privilageLevel) {
//    uint8_t privilageLevel = 0;
//    asm volatile(
//    "mov eax, cs\n"
//    "mov %0, eax\n"
//    :"=r"(privilageLevel)
//    );
//    privilageLevel == 0x10 ? privilageLevel = 0
//        : privilageLevel = 3;
    uint32_t numberOfPage = size % PAGE_SIZE ? size / PAGE_SIZE + 1
                                             : size / PAGE_SIZE;
    uintptr_t nowPageDirectoryToA = logicalAddress / 1024;
    uintptr_t nowPageTableToA = logicalAddress % 1024;
    PageDirectoryEntry *nowPDE = cr3 + nowPageDirectoryToA;     //user_page_table's pde
    PageTableEntry *nowPTE = reinterpret_cast<PageTableEntry *>
    ((nowPDE->PageTableAddress << OFFSET) + nowPageTableToA);
    uintptr_t pageCount = nowPageTableToA;
    while (numberOfPage) {
        if (pageCount % 1024 == 0 && pageCount != 0) {
            nowPDE++;
            nowPDE->P = 1;
            nowPDE->U_S = privilageLevel;
            nowPDE->PageTableAddress = allocateOnePage();
            nowPTE = reinterpret_cast<PageTableEntry *>
                     (nowPDE->PageTableAddress << OFFSET);
        }
        nowPTE->P = 1;
        nowPTE->U_S = privilageLevel;
        nowPTE->PageTable = allocateOnePage();
        nowPTE->R_W = 1;
        nowPTE++;
        numberOfPage--;
        pageCount++;
    }
}

void MemoryManager::free(PageDirectoryEntry *cr3) {
    for (uint32_t i = 0; i <= BUNDARY; i++) {
        if ((cr3 + i)->P) {
            PageTableEntry *PageToDelete = reinterpret_cast<PageTableEntry *>
            ((cr3 + i)->PageTableAddress << OFFSET);
            for (uint32_t j = 0; j < 1024; j++) {
                PageToDelete += j;
                // free every page
                if (PageToDelete->P) {
                    uintptr_t address = PageToDelete->PageTable;
                    freeOnePage(address);
                }
            }
        }
        // free pageTable
        uintptr_t address = (cr3 + i)->PageTableAddress;
        freeOnePage(address);
    }
    // free page directory table
    freeOnePage(reinterpret_cast<uintptr_t>(cr3) >> OFFSET);
}

uint32_t MemoryManager::allocateOnePage() {
    for (auto i = 0; i < 0x100; i++) {
        PageTableEntry *pte = reinterpret_cast<PageTableEntry *>((PDE + i)->PageTableAddress << 12);
        for (uint32_t j = 0; j < 1024; j++) {
            if (!pte->P) {
                pte->P = 1;
                return pte->PageTable;
            }
            pte++;
        }
        (PDE + i)->P = 1;
    }
}

uint32_t MemoryManager::freeOnePage(uintptr_t address) { //address is the beginning of a Page
    uintptr_t nowPageDirectoryToDelete = address / 1024;
    uintptr_t nowPageTableToDelete = address % 1024;
    PageTableEntry *PageToReset = reinterpret_cast<PageTableEntry *>
    ((PDE + nowPageDirectoryToDelete)->PageTableAddress << OFFSET);
    (PageToReset + nowPageTableToDelete)->P = 0;
}

}
}


