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
            pte->P = ((j<=0x201+BUNDARY && i == 0)?1:0);
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
    PDE = internalPDE;

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
    uintptr_t address = (allocateOnePage() << OFFSET);
    PageDirectoryEntry* pde = reinterpret_cast<PageDirectoryEntry *>(address);
    for (uint32_t j = 0; j < BUNDARY; j++) {
        pde->P = 0;
        pde->R_W = 1;
        pde->U_S = 1;
        pde->PWT = 0;
        pde->PCD = 0;
        pde->A = 0;
        pde->reserved = 0;
        pde->PS = 0;
        pde->G = 0;
        pde->Avail = 0;
        pde->PageTableAddress = 0;
        pde += 1;
    }
    return address;
}

void MemoryManager::copyKernelPageTable(myos::kernel::PageDirectoryEntry *cr3
        , bool privilageLever) {
    PageDirectoryEntry* nowPDE = cr3;
    uintptr_t lga = 0;
    for (int j = 0; j<BUNDARY/2;j++) {
        if (!nowPDE->P) {
            nowPDE->PageTableAddress = allocateOnePage();
            nowPDE->P = 1;
        }
        PageTableEntry *pte = reinterpret_cast<PageTableEntry *>
        (nowPDE->PageTableAddress << OFFSET);
        //pte += kernelStart;       //start of kernel
        for (uint32_t i = 0; i < 1024; i++) {
            pte->P = 1;
            pte->U_S = ((i<=0x201+BUNDARY && j == 0)?0:1);
            pte->PageTable = getPage(lga, internalPDE);
            pte->R_W = 1;
            pte->Avail = 0;
            pte->A = 0;
            pte->G = 0;
            pte->PAT = 0;
            pte->D = 0;
            pte->PCD = 0;
            pte->PWT = 0;

            pte += 1;
            lga += 0x1000;
        }
        nowPDE++;
    }
}

//when allocating memory, CR3 must be internalPDE
//size: char (unit)
//logicalAddressBegin: User's logical address that want to be allocated memory
void MemoryManager::allocate(uint32_t size, PageDirectoryEntry *cr3
        ,uintptr_t logicalAddressBegin, bool privilageLevel) {
    uint32_t numberOfPage = size % PAGE_SIZE ? size / PAGE_SIZE + 1
                                             : size / PAGE_SIZE;
    logicalAddressBegin = logicalAddressBegin >> OFFSET;      //logical address of page table
    uintptr_t nowPageDirectoryToA = logicalAddressBegin / 1024;
    uintptr_t nowPageTableToA = logicalAddressBegin % 1024;

    PageDirectoryEntry *nowPDE = cr3 + nowPageDirectoryToA ;     //user_page_table's pde
    if (!nowPDE->P) {
        nowPDE->PageTableAddress = allocateOnePage();
        nowPDE->P = 1;
        nowPDE->R_W = 1;
        nowPDE->U_S = privilageLevel;
    }
    PageTableEntry *nowPTE = reinterpret_cast<PageTableEntry *>
        (nowPDE->PageTableAddress << OFFSET);
    nowPTE += nowPageTableToA;
    uintptr_t pageCount = nowPageTableToA;
    while (numberOfPage) {
        if (pageCount % 1024 == 0 && pageCount != 0) {
            nowPDE += 1;
            nowPDE->P = 1;
            nowPDE->R_W = 1;
            nowPDE->U_S = privilageLevel;
            nowPDE->PageTableAddress = allocateOnePage();
            nowPTE = reinterpret_cast<PageTableEntry *>
                     (nowPDE->PageTableAddress << OFFSET);
        }
        nowPTE->P = 1;
        nowPTE->U_S = privilageLevel;
        nowPTE->PageTable = allocateOnePage();
        nowPTE->R_W = 1;
        nowPTE->PWT = 0;
        nowPTE->PCD = 0;
        nowPTE->D = 0;
        nowPTE->PAT = 0;
        nowPTE->G = 0;
        nowPTE->A = 0;
        nowPTE->Avail = 0;

        nowPTE += 1;
        numberOfPage--;
        pageCount++;
    }
}

void MemoryManager::free(PageDirectoryEntry *cr3) {
    for (uint32_t i = 0; i <= BUNDARY; i++) {
        if ((cr3 + i)->P) {
            PageTableEntry *PageToDelete = reinterpret_cast<PageTableEntry *>
            ((cr3 + i)->PageTableAddress << OFFSET);
            uint32_t from = i ? 0 : 0x300;
            for (uint32_t j = from; j < 1024; j++) {
                PageToDelete += 1;
                // free every page
                if (PageToDelete->P) {
                    uintptr_t address = PageToDelete->PageTable;
                    freeOnePage(address);
                }
            }
        }
    // free pageTable
        //uintptr_t address = (cr3 + i)->PageTableAddress;
        //freeOnePage(address);
    }
  // free page directory table
    //freeOnePage(reinterpret_cast<uintptr_t>(cr3) >> OFFSET);
}

//cr3 must be given
uint32_t MemoryManager::getPage(uintptr_t interalLogicalAddress, PageDirectoryEntry* cr3) {
    uint32_t pdeOffset = (interalLogicalAddress & 0xffc00000) >> 22;
    uint32_t pteOffset = (interalLogicalAddress & 0x003ff000) >> 12;
    //uint32_t Offset = interalLogicalAddress & 0x00000fff;
    PageTableEntry* pte = reinterpret_cast<PageTableEntry *>
    ((cr3 + pdeOffset)->PageTableAddress << OFFSET);
    return (pte + pteOffset)->PageTable;
}

uint32_t MemoryManager::allocateOnePage() {
    for (auto i = 0; i < BUNDARY; i++) {
        PageTableEntry *pte = reinterpret_cast<PageTableEntry *>((PDE + i)->PageTableAddress << 12);
        for (uint32_t j = 0; j < 1024; j++) {
            if (!pte->P) {
                (PDE + i)->P = 1;
                pte->P = 1;
                return pte->PageTable;
            }
            pte++;
        }
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


