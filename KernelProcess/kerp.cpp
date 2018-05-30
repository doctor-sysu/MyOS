#include <System.hpp>

void sleep() {
    for (int i = 0; i < 100000; ++i) {
        asm volatile("nop");
    }
}

extern "C" int main() {
    for(int i = 0; i < 100000; ++i) {
        for (int j = 1; j < 5; ++j)
               SysC(j);
        sleep();
    }
    while (true){

    }
}

