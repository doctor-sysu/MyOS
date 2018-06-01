#include <myos/kernel/Process.hpp>
#include <myos/kernel/Keyboard.hpp>

myos::kernel::Process processes;

namespace myos{
namespace kernel{

uint32_t Process::create(uint32_t _start) {
    uint32_t userStack = _start + 0x10000;
    Process_Count++;
    PCB* new_process = &PCBList[Process_Count];
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
//    asm volatile(
//            "mov dword [edx], eax\n"
//            "mov dword [edx - 4], ebx\n"
//            "mov dword [edx - 8], ecx\n"
//            ::"a"(new_process->eflags), "b"(new_process->cs)
//                ,"c"(new_process->eip), "d"(new_process->esp)
//            );
//    asm volatile(
//            "mov dword [eax - 12], eax\n"
//            "mov dword [eax - 16], ebx\n"
//            ::"a"(new_process->esp), "b"(new_process->ss)
//            );
//    new_process->esp -= 16;
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
        *progress = PCBList[0];  //change to kernel process
    } else{
        if (Process_Count < running)
            running = 1;
        *progress = PCBList[running];
    }
}

void Process::change(PCB* progress){
    if (running == -1) {
        running++;
        *progress = PCBList[0];
        return;
    }
    if (Process_Count <= 0) return;
    if (running <= Process_Count){
        PCBList[running] = *progress;
        running++;
        if (running > Process_Count)
            running = 1;
        *progress = PCBList[running];
    }
}



}
}
