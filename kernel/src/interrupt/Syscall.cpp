#include "Syscall.hpp"
#include <myos/kernel/Process.hpp>

extern myos::kernel::Process processes;

namespace myos{
namespace kernel{
namespace Syscall{

//extern "C" void User_Int1();
//extern "C" void User_Int2();
//extern "C" void User_Int3();
//extern "C" void User_Int4();
//extern "C" void windmill();
//extern void End_Process();
/* parameter     function
 * 1~5           SC_printSomething
 * 90            SC_KillProcess
 *
 * */

void windmill() {
    //windwill
    static int state = 1;
    static char *videomem_windwill = reinterpret_cast<char *>(0xb8000 + 2 * (5 * 80 + 20));
    if (state == 5) {
        *(videomem_windwill) = '-';
        state = 1;
    } else {
        if (state == 1)
            *(videomem_windwill) = '|';
        else if (state == 2)
            *(videomem_windwill) = '/';
        else if (state == 3)
            *(videomem_windwill) = '-';
        else if (state == 4)
            *(videomem_windwill) = '\\';
        ++state;
    }
}

void windmill2() {
    static int state2 = 1;
    static char *videomem_windwill2 = reinterpret_cast<char *>(0xb8000 + 2 * (10 * 80 + 20));
    if (state2 == 5) {
        *(videomem_windwill2) = '-';
        state2 = 1;
    } else {
        if (state2 == 1)
            *(videomem_windwill2) = '|';
        else if (state2 == 2)
            *(videomem_windwill2) = '/';
        else if (state2 == 3)
            *(videomem_windwill2) = '-';
        else if (state2 == 4)
            *(videomem_windwill2) = '\\';
        ++state2;
    }
}

//print_info
static char info[40] = "16337062FENGZHENXUAN16337067GUANPEIDONG";
static int info_x = 0;
static int info_y = 0;
static int index = 0;
static int info_count = 40;
/*
 * 1 right down
 * 2 right up
 * 3 left down
 * 4 left up
 */
static int info_state = 1;
void print_info() {
//    --info_count;
//    if (info_count)
//        return;
    char *videomem_info = reinterpret_cast<char *>(0xb8000 + 2 * (80 * (info_y + 1) + info_x + 40));
    *(videomem_info) = info[index++];
    if (index == 40)
        index = 0;
    if (info_state == 1) {
        ++info_x;
        ++info_y;
        if (info_x == 37 && info_y == 11)
            info_state = 4;
        else if (info_y == 11)
            info_state = 2;
        else if (info_x == 37)
            info_state = 3;
    } else if (info_state == 2) {
        ++info_x;
        --info_y;
        if (info_x == 37 && info_y == 0)
            info_state = 3;
        else if (info_x == 37)
            info_state = 4;
        else if (info_y == 0)
            info_state = 1;
    } else if (info_state == 3) {
        --info_x;
        ++info_y;
        if (info_x == 0 && info_y == 11)
            info_state = 2;
        else if (info_x == 0)
            info_state = 1;
        else if (info_y == 11)
            info_state = 4;
    } else if (info_state == 4) {
        --info_x;
        --info_y;
        if (info_x == 0 && info_y == 0)
            info_state = 1;
        else if (info_x == 0)
            info_state = 2;
        else if (info_y == 0)
            info_state = 3;
    }
    info_count = 40;
}

//every 100 clock interrupts have a flash
int helloworld_count = 200;

void print_Hello() {
    if (helloworld_count != 100)
        return;
    char *videomem_Hello = reinterpret_cast<char *>(0xb8000 + 2760);
    for (int i = 0; i < 5; ++i) {
        *(videomem_Hello + 60) = ' ';
        videomem_Hello += 2;
    }
    videomem_Hello -= 10;
    char str[6] = "Hello";
    for (int i = 0; i < 5; ++i) {
        *(videomem_Hello) = str[i];
        videomem_Hello += 2;
    }
}

void print_World() {
    --helloworld_count;
    if (helloworld_count)
        return;
    char *videomem_World = reinterpret_cast<char *>(0xb8000 + 2820);
    for(int i = 0; i < 5; ++i)
    {
        *(videomem_World - 60) = ' ';
        videomem_World += 2;
    }
    videomem_World -= 10;
    char str[6] = "World";
    for (int i = 0; i < 5; ++i) {
        *(videomem_World) = str[i];
        videomem_World += 2;
    }
    helloworld_count = 200;
}

void kill_process(myos::kernel::PCB* progress){
    //processes.exchange(progress);
    processes.kill(progress);
}

void syscall(PCB* progress) {
    switch (progress->eax) {
        case 1:
            windmill();
            break;
        case 2:
            windmill2();
            break;
        case 3:
            print_Hello();
            break;
        case 4:
            print_World();
            break;
        case 90:
            kill_process(progress);
            break;
        default:
            break;
    };
}

}
}
}

