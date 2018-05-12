#include <myos/kernel/Keyboard.hpp>
#include <myos/kernel/RW_port.hpp>

myos::kernel::Keyboard Keyboard_buffer;

namespace myos{
namespace kernel{

using myos::kernel::RW_port::in;
using myos::kernel::RW_port::out;

void Keyboard::kb_in() {
    buffer.append(in(0x60));
}

void Keyboard::clean() {
    buffer.clear();
}

unsigned int Keyboard::size() {
    return buffer.size();
}

}
}

