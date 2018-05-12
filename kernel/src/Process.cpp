#include <myos/kernel/Process.hpp>
#include <myos/kernel/FAT12.hpp>
#include <myos/kernel/Keyboard.hpp>

myos::kernel::Process processes;
extern myos::kernel::Keyboard Keyboard_buffer;

extern "C" void callprocess(myos::kernel::PCB* progress)
{
    processes.exchange(progress);
}

static unsigned int now_process = 0;
using myos::kernel::FAT12::FAT12;
void __cpp_create_new_process(){
    Keyboard_buffer.kb_in();
    if (Keyboard_buffer.size() < 2) return;
    if (now_process >= 4)
        return;
    void *load = reinterpret_cast<void *>(FAT12((char) (now_process+1)));
    unsigned int entry;
    //加载用户程序
    if (load > 0) {
        entry = *(unsigned int *) ((unsigned int*) load + 0x18);
        processes.create(now_process,entry);
        //((void (*)()) entry)();
    }
    now_process++;
    Keyboard_buffer.clean();
}

extern "C" void create_new_process(){
    __cpp_create_new_process();
}

namespace myos{
namespace kernel{

bool Process::create(uint32_t file, uint32_t _start) {
    PCB* new_process = &PCBList[Process_Count];
    Process_Count++;
    new_process->cs = _start;
    new_process->eip = 0;
    new_process->esp = 0x210000 + 0x20000 * file;
    new_process->eflags = 0x00000202;
    new_process->Error_code = 0;
    return true;
}

void Process::exchange(PCB* progress) {
    if (!Process_Count)
        return;
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

Process::Process():Process_Count(0),now(nullptr) {}

void Process::change(){
    if (Process_Count == 0) return;
    for (int i=0; i< Process_Count - 1; i++){
        if (now == &PCBList[i]){
            PCBList[i] = *now;
            now = &PCBList[i+1];
            return;
        }
    }
    now = &PCBList[0];
}



}
}
