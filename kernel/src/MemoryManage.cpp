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
        now->PageTableAddress = static_cast<uint32_t>((0x200000 >> 12) + (i + 1));
        PageTableEntry *PTE = reinterpret_cast<PageTableEntry *>(now->PageTableAddress << 12);
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
            pte->PageTable = (0x200000 >> 12) + 1 + BUNDARY
                             + 1024 * i + j;
        }
    }
}

uint32_t MemoryManager::PageDirectoryAllocate() { // return user's cr3
    uint32_t address = allocateOnePage();
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
void MemoryManager::allocate(uint32_t size, UserMemoryManage *UserPageTable, bool privilageLevel) {
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

    uint32_t nowPageDirectoryToA = UserPageTable->numberOfPage / 1024;
    uint32_t nowPageTableToA = UserPageTable->numberOfPage % 1024;
    PageDirectoryEntry *nowPDE = UserPageTable->PDE + nowPageDirectoryToA;
    PageTableEntry *nowPTE = reinterpret_cast<PageTableEntry *>
    (nowPDE->PageTableAddress << 12 + nowPageTableToA);
    uint32_t pageCount = nowPageTableToA;
    while (numberOfPage) {
        if (pageCount % 1024 == 0 && pageCount != 0) {
            nowPDE++;
            nowPDE->P = 1;
            nowPDE->U_S = privilageLevel;
            nowPDE->PageTableAddress = allocateOnePage();
            nowPTE = reinterpret_cast<PageTableEntry *>
                     (nowPDE->PageTableAddress << 12);
        }
        nowPTE->P = 1;
        nowPTE->U_S = privilageLevel;
        nowPTE->PageTable = allocateOnePage();
        nowPTE->R_W = 1;
        nowPTE++;
        UserPageTable->numberOfPage++;
        numberOfPage--;
        pageCount++;
    }
}

void MemoryManager::free(UserMemoryManage *UserPageTable) {
    uint32_t numberPageDirectoryToDelete = UserPageTable->numberOfPage / 1024;
    uint32_t numberPageTableToDelete = UserPageTable->numberOfPage % 1024;
    for (uint32_t i = 0; i <= numberPageDirectoryToDelete; i++) {
        PageTableEntry *PageToDelete = reinterpret_cast<PageTableEntry *>
        ((UserPageTable->PDE + i)->PageTableAddress << 12);
        if (i != numberPageTableToDelete) {
            for (uint32_t j = 0; j < 1024; j++) {
                PageToDelete += j;
                uint32_t address = PageToDelete->PageTable;
                freeOnePage(address);
            }
        } else {
            for (uint32_t j = 0; j < numberPageTableToDelete; j++) {
                PageToDelete += j;
                uint32_t address = PageToDelete->PageTable;
                freeOnePage(address);
            }
        }

        uint32_t address = (UserPageTable->PDE + i)->PageTableAddress;
        freeOnePage(address);
    }
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

uint32_t MemoryManager::freeOnePage(uint32_t address) {
    uint32_t nowPageDirectoryToDelete = address / 1024;
    uint32_t nowPageTableToDelete = address % 1024;
    PageTableEntry *PageToReset = reinterpret_cast<PageTableEntry *>
    ((PDE + nowPageDirectoryToDelete)->PageTableAddress << 12);
    (PageToReset + nowPageTableToDelete)->P = 0;
}

}
}


