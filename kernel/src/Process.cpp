#include <myos/kernel/Process.hpp>

uint32_t kernel_sp = 0;

extern "C" void callprocess(myos::kernel::PCB* progress)
{

}


namespace myos{
namespace kernel{




void Process::exchange(PCB*& progress) {
    //Turn to Kernel stack
    //save all registers to kernel stack
    //save all registers to PCB

    //get new progress's registers to kernel stack
    //assign registers
    //turn to progress stack

}

bool Process::create() {

}

Process::Process() {
    Process_Count = 0;
}

}
}
