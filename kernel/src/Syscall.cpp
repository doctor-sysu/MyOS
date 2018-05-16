namespace myos{
namespace kernel{
namespace Syscall{

extern "C" void SC_printSomething(int);
//extern void End_Process();
/* parameter     function
 * 1~5           SC_printSomething
 * 90            SC_KillProcess
 *
 * */
void _printSomething(int);

void SC_KillProcess();

void syscall(int FunNum) {
    switch (FunNum) {
        case 1:
            _printSomething(1);
            break;
        case 2:
            _printSomething(2);
            break;
        case 3:
            _printSomething(3);
            break;
        case 4:
            _printSomething(4);
            break;
        case 5:
            _printSomething(5);
            break;
        case 90:
            SC_KillProcess();
            break;
        default:
            break;
    };
}

void _printSomething(int FunNum){
    SC_printSomething(FunNum);
}

void SC_KillProcess() {
    asm volatile(
    "int 0x90\n"
    );
}

}
}
}

extern "C" void _syscall(int FunNum){
    myos::kernel::Syscall::syscall(FunNum);
}