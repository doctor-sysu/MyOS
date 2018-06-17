#include <System.hpp>

void sleep() {
    for (int i = 0; i < 100000; ++i) {
        asm volatile("nop");
    }
}

extern "C" int main() {
    while (true){

    }
}

