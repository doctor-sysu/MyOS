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

extern "C" void kill_process(myos::kernel::PCB* progress){
    //processes.exchange(progress);
    processes.kill(progress);
}

namespace myos{
namespace kernel{

bool Process::create(uint32_t file, uint32_t _start) {
    PCB* new_process = &PCBList[Process_Count];
    Process_Count++;
    new_process->cs = 0x08;
    new_process->gs = 0x10;
    new_process->fs = 0x10;
    new_process->ss = 0x10;
    new_process->ds = 0x10;
    new_process->es = 0x10;
    new_process->eip = _start;
    new_process->esp = 0x210000 + 0x20000 * file;
    new_process->ebp = new_process->esp;
    new_process->eflags = 0x00000202;
    new_process->Error_code = 0;
    asm volatile(
            "mov dword [edx], eax\n"
            "mov dword [edx - 4], ebx\n"
            "mov dword [edx - 8], ecx\n"
            ::"a"(new_process->eflags), "b"(new_process->cs)
                ,"c"(new_process->eip), "d"(new_process->esp)
            );
    new_process->esp -= 8;
    return true;
}

void Process::exchange(PCB* progress) {
    if (!Process_Count) {
        //put kernel into PCBList[SIZE_OF_PCBList]
        PCBList[SIZE_OF_PCBList] = *progress;
        return;
    }
    //Turn to Kernel stack
    //save all registers to kernel stack
    //save all registers to PCB
    change(progress);
    //get new progress's registers to kernel stack
    //assign registers
    //turn to progress stack
}

Process::Process():Process_Count(0),running(-1) {}

void Process::initial(){
    running = -1;
}

void Process::kill(PCB* progress) {
    for (int i = running; i < Process_Count - 1; i++) {
        PCBList[i] = PCBList[i + 1];
    }
    Process_Count--;
    if (!Process_Count) {
        running = -1;
        *progress = PCBList[SIZE_OF_PCBList];  //change to the kernel
    } else{
        if (Process_Count == running)
            running = 0;
        *progress = PCBList[running];
    }
}

void Process::change(PCB* progress){
    if (Process_Count == 0) return;
    if (running == -1) {
        running++;
        *progress = PCBList[0];
        return;
    }
    if (running < Process_Count){
        PCBList[running] = *progress;
        running++;
        if (running == Process_Count)
            running = 0;
        *progress = PCBList[running];
    }
}



}
}
