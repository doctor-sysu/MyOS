#include <myos/kernel/Keyboard.hpp>
#include <myos/kernel/RW_port.hpp>

myos::kernel::Keyboard Keyboard_buffer;

namespace myos{
namespace kernel{

using myos::kernel::RW_port::in;
using myos::kernel::RW_port::out;

void Keyboard::kb_in() {
    char scan_code = in(0x60);
    buffer.append(scan_code);
    static char *videomem = reinterpret_cast<char *>(0xb8000 + 2 * (5 * 80 + 20));
    *(videomem) = scan_code;
    
}

void Keyboard::clean() {
    buffer.clear();
}

unsigned int Keyboard::size() {
    return buffer.size();
}

}
}

