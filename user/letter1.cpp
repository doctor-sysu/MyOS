
#include <System.hpp>

void sleep();


extern "C" int main() {
    for(int i = 0; i < 1; ++i) {
        char *info = const_cast<char *>("1234567890qwertyuiop\n");
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