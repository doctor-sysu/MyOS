
#include <System.hpp>

void sleep();

extern "C" int main() {
    for(int i = 0; i < 100000; ++i) {
        for (int j = 3; j < 5; ++j)
            SysC(j);
        //SysC(2);
        sleep();
    }
    SysC(90);
}


void sleep() {
    for (int i = 0; i < 100000; ++i) {
        asm volatile("nop");
    }
}

