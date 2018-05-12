#include <myos/kernel/RW_port.hpp>

namespace myos{
namespace kernel{
namespace RW_port{

uint8_t in(uint32_t port) { //read data form port
    uint8_t data = 0;
    asm volatile(
    "in %0, dx"
    :"=r"(data)
    :"d"(port)
    );
    return data;
}

void out(uint32_t port, uint8_t data) {  //write data to port
    asm volatile(
    "out dx, al"
    ::"d"(port), "a"(data)
    );
}

}
}
}
