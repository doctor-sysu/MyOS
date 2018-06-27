#include <myos/kernel/MemoryManage.hpp>

myos::kernel::MemoryManager memManage;

namespace myos {
namespace kernel {

void MemoryManager::initial() {
    //uint32_t bundary = 0x100;
    internalPDE = reinterpret_cast<PageDirectoryEntry *>(0x200000);
    for (auto i = 0; i < BUNDARY; i++) {
        auto now = internalPDE + i;
        now->P = (static_cast<bool>(i)?0:1);
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
            pte->P = ((j<=0x301 && i == 0)?1:0);
            pte->R_W = 0;
            pte->U_S = 0;
            pte->PWT = 0;
            pte->PCD = 0;
            pte->A = 0;
            pte->D = 0;
            pte->PAT = 0;
            pte->G = 0;
            pte->Avail = 0;
//            pte->PageTable = (0x200000 >> OFFSET) + 1 + BUNDARY
//                             + 1024 * i + j;
            pte->PageTable = 1024* i + j;
        }
    }
    PDE = internalPDE + SIZE_OF_ENTRY;

    //Start paging mechanism
    asm volatile(
            "mov cr3, eax\n"
            "mov eax, cr0\n"
            "or eax, 0x80000000\n"
            "mov cr0, eax\n"
            ::"a"(internalPDE)
            );
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

void MemoryManager::copyKernelPageTable(myos::kernel::PageDirectoryEntry *cr3
        , bool privilageLever) {
    cr3->P = 1;
    PageTableEntry* pte = reinterpret_cast<PageTableEntry *>
                          ((cr3->PageTableAddress << OFFSET) + 0x100*SIZE_OF_ENTRY);
    for (uint32_t i = 0; i<0x100;i++){
        pte->P = 1;
        pte->U_S = privilageLever;
        pte->PageTable = 0x100 + i;
        pte += SIZE_OF_ENTRY;
    }
}

//size: char (unit)
//logicalAddress: User's logical address that want to be allocated memory
void MemoryManager::allocate(uint32_t size, PageDirectoryEntry *cr3
        ,uintptr_t logicalAddress, bool privilageLevel) {
    uint32_t numberOfPage = size % PAGE_SIZE ? size / PAGE_SIZE + 1
                                             : size / PAGE_SIZE;
    logicalAddress = logicalAddress >> OFFSET;      //logical address of page table
    uintptr_t nowPageDirectoryToA = logicalAddress / 1024;
    uintptr_t nowPageTableToA = logicalAddress % 1024;
    PageDirectoryEntry *nowPDE = cr3 + nowPageDirectoryToA*SIZE_OF_ENTRY;     //user_page_table's pde
    PageTableEntry *nowPTE = reinterpret_cast<PageTableEntry *>
    ((nowPDE->PageTableAddress << OFFSET) + nowPageTableToA*SIZE_OF_ENTRY);
    uintptr_t pageCount = nowPageTableToA;
    while (numberOfPage) {
        if (pageCount % 1024 == 0 && pageCount != 0) {
            nowPDE +=SIZE_OF_ENTRY;
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
        nowPTE += SIZE_OF_ENTRY;
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
                PageToDelete += SIZE_OF_ENTRY;
                // free every page
                if (PageToDelete->P) {
                    uintptr_t address = PageToDelete->PageTable;
                    freeOnePage(address);
                }
            }
        }
        // free pageTable
        uintptr_t address = (cr3 + i*SIZE_OF_ENTRY)->PageTableAddress;
        freeOnePage(address);
    }
    // free page directory table
    freeOnePage(reinterpret_cast<uintptr_t>(cr3) >> OFFSET);
}

uint32_t MemoryManager::allocateOnePage() {
    for (auto i = 0; i < BUNDARY; i++) {
        PageTableEntry *pte = reinterpret_cast<PageTableEntry *>((PDE + i*SIZE_OF_ENTRY)->PageTableAddress << 12);
        for (uint32_t j = 0; j < 1024; j++) {
            if (!pte->P) {
                pte->P = 1;
                return pte->PageTable;
            }
            pte++;
        }
        (PDE + i*SIZE_OF_ENTRY)->P = 1;
    }
}

uint32_t MemoryManager::freeOnePage(uintptr_t address) { //address is the beginning of a Page
    uintptr_t nowPageDirectoryToDelete = address / 1024;
    uintptr_t nowPageTableToDelete = address % 1024;
    PageTableEntry *PageToReset = reinterpret_cast<PageTableEntry *>
    ((PDE + nowPageDirectoryToDelete)->PageTableAddress << OFFSET);
    (PageToReset + nowPageTableToDelete*SIZE_OF_ENTRY)->P = 0;
}

}
}


