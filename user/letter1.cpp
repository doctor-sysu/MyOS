
#include <Syscall.h>

void sleep();

extern "C" int main() {
    for(int i = 0; i < 100000; ++i) {
        for (int i = 1; i < 5; ++i)
            Syscall(i);
        sleep();
    }
}


void sleep() {
    for (int i = 0; i < 100000; ++i) {
        asm volatile("nop");
    }
}

