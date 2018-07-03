
#include <System.hpp>

void sleep();

extern "C" int main() {
    for (int i = 0; i < 100000; ++i) {
        SysC(1);
        sleep();
    }
    SysC(90);    //kill the process
}


void sleep() {
    for (int i = 0; i < 10000000; ++i) {
        asm volatile("nop");
    }
}

