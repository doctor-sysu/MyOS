#include <myos/kernel/IDT.hpp>

extern "C" void cpu_initialize();

extern "C" int main() {
    cpu_initialize();

    //char *video = reinterpret_cast<char *>(0xb8000);
    //*(video) = 'A';

    return 0;
}
