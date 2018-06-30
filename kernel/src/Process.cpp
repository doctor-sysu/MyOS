#include <myos/kernel/Process.hpp>
#include <myos/kernel/Keyboard.hpp>
#include <myos/kernel/FAT12.hpp>

myos::kernel::Process processes;

extern myos::kernel::MemoryManager memManage;

namespace myos{
namespace kernel{

uintptr_t Process::create(char* name) {
    //uint32_t userStack = _start + 0x10000;

    Process_Count++;
    PCBList[Process_Count].pid = Process_Count;
    PCBList[Process_Count].priority = 0;
    PCBList[Process_Count].video_page = (Process_Count<=0) ? 1 : Process_Count;

    PCB* new_process = &PCBList[Process_Count].pcb;
    new_process->cs = 0x1B;
    new_process->gs = 0x23;
    new_process->fs = 0x23;
    new_process->ss = 0x23;
    new_process->ds = 0x23;
    new_process->es = 0x23;
    //*(reinterpret_cast<uint32_t *>(load + 0x18)))
    //new_process->eip = *(reinterpret_cast<uint32_t *>(0x300000 + 0x18));
    new_process->esp = 0xc000000;
    new_process->ebp = new_process->esp;
    new_process->eflags = 0x00000202;
    new_process->Error_code = 0;
    PCBList[Process_Count].CR3 = reinterpret_cast<PageDirectoryEntry *>
                                    (memManage.PageDirectoryAllocate());
    memManage.copyKernelPageTable(PCBList[Process_Count].CR3,0);    //copy kernel page table
    memManage.allocate(0x5000,PCBList[Process_Count].CR3,0x300000,1);
    memManage.allocate(0x11000,PCBList[Process_Count].CR3,0xbff0000,1);

    myos::kernel::FAT12::FAT12(name, reinterpret_cast<uintptr_t>(PCBList[Process_Count].CR3));
    uintptr_t userStart = memManage.getPage(0x300000,PCBList[Process_Count].CR3) << OFFSET;
    new_process->eip = *(reinterpret_cast<uint32_t *>(userStart + 0x18));
    return reinterpret_cast<uintptr_t>(PCBList[Process_Count].CR3);
}

void Process::exchange(PCB* progress) {
    if (Process_Count<0) {
        //put kernel into PCBList[SIZE_OF_PCBList]
        //PCBList[SIZE_OF_PCBList] = *progress;
        return;
    }
    //Turn to Kernel stack
    //save all registers to kernel stack
    //save all registers to PCB

    change(progress);

    //get new progress's registers to kernel stack
    //assign registers
    //turn to progress stack
    //cpu_enterUserCode(progress->eip, progress->esp);
}

Process::Process():Process_Count(-1),running(-1) {}

void Process::initial(){
    running = -1;
    Process_Count = -1;
}

void Process::kill(PCB* progress) {
    memManage.free(PCBList[running].CR3);
    for (int i = running; i < Process_Count; i++) {
        PCBList[i] = PCBList[i + 1];
    }
    Process_Count--;
    if (!Process_Count) {
        running = 0;
        *progress = PCBList[0].pcb;  //change to kernel process
    } else{
        if (Process_Count < running)
            running = 1;
        *progress = PCBList[running].pcb;
    }
    changeCR3(reinterpret_cast<uintptr_t>(PCBList[running].CR3));
}

void Process::change(PCB* progress){
    if (running == -1) {
        running++;
        *progress = PCBList[0].pcb;
        changeCR3(reinterpret_cast<uintptr_t>(PCBList[running].CR3));
        return;
    }
    if (Process_Count <= 0) return;
    if (running <= Process_Count){
        PCBList[running].pcb = *progress;
        running++;
        if (running > Process_Count)
            running = 1;
        *progress = PCBList[running].pcb;
        changeCR3(reinterpret_cast<uintptr_t>(PCBList[running].CR3));
    }
}

void changeCR3(uintptr_t cr3){
    asm volatile(
            "mov cr3,%0\n"
            ::"r"(cr3)
            );
}

const int32_t Process::get_running_page() {
    return PCBList[running].video_page;
}

}
}
