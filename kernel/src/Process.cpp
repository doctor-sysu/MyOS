#include <myos/kernel/Process.hpp>
#include <myos/kernel/Keyboard.hpp>

myos::kernel::Process processes;

extern myos::kernel::MemoryManager memManage;

namespace myos{
namespace kernel{

uint32_t Process::create(uint32_t _start) {
    uint32_t userStack = _start + 0x10000;

    Process_Count++;
    PCBList[Process_Count].pid = Process_Count;
    PCBList[Process_Count].priority = 0;
    PCBList[Process_Count].video_page = Process_Count;

    PCB* new_process = &PCBList[Process_Count].pcb;
    new_process->cs = 0x1B;
    new_process->gs = 0x23;
    new_process->fs = 0x23;
    new_process->ss = 0x23;
    new_process->ds = 0x23;
    new_process->es = 0x23;
    new_process->eip = _start;
    new_process->esp = userStack;
    new_process->ebp = new_process->esp;
    new_process->eflags = 0x00000202;
    new_process->Error_code = 0;
    PCBList[Process_Count].CR0 = reinterpret_cast<PageDirectoryEntry *>
                                    (memManage.PageDirectoryAllocate());
    //TODO default allocate memory, a new function to allocate stack memory and the memory of code and data
    memManage.allocate(250,PCBList[Process_Count].CR0,0,1);
    memManage.allocate(250,PCBList[Process_Count].CR0,0xc000000,1);
    return userStack;
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
}

void Process::change(PCB* progress){
    if (running == -1) {
        running++;
        *progress = PCBList[0].pcb;
        return;
    }
    if (Process_Count <= 0) return;
    if (running <= Process_Count){
        PCBList[running].pcb = *progress;
        running++;
        if (running > Process_Count)
            running = 1;
        *progress = PCBList[running].pcb;
    }
}

const int32_t Process::get_running() {
    return running;
}


}
}
