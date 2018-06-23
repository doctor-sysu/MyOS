
#include <System.hpp>

void sleep();


extern "C" int main() {
    for(int i = 0; i < 10000000; ++i) {
        char *info = const_cast<char *>("I love studying operating system!\n");
        printf(info);
        sleep();
    }

    SysC(90);
    while (true) {}
}

void sleep() {
    for (int i = 0; i < 100000; ++i) {
        asm volatile("nop");
    }
}