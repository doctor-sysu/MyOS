#include <myos/kernel/Process.hpp>

uint32_t kernel_sp = 0;
myos::kernel::Process processes;

extern "C" void callprocess(myos::kernel::PCB* progress)
{
    processes.exchange(progress);
}


namespace myos{
namespace kernel{




void Process::exchange(PCB*& progress) {
    //Turn to Kernel stack
    //save all registers to kernel stack
    //save all registers to PCB
    (*now) = *progress;
    change();
    (*progress) = (*now);
    //get new progress's registers to kernel stack
    //assign registers
    //turn to progress stack

}

bool Process::create() {
    now = &PCBList[Process_Count];
    Process_Count++;
    now->cs = 0x200000;
    now->esp = 0x210000;
    now->eip = 0;
}

Process::Process():Process_Count(0),now(nullptr) {}

void Process::change(){
    if (Process_Count == 0) return;
    for (int i=0; i< Process_Count - 1; i++){
        if (now == &PCBList[i]){
            now = &PCBList[i+1];
            return;;
        }
    }
    now = &PCBList[0];
}

}
}
