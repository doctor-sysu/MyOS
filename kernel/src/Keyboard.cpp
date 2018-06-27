#include <myos/kernel/Keyboard.hpp>
#include <myos/kernel/RW_port.hpp>
#include <myos/kernel/Scancode.hpp>
#include <cstdint>
#include <cstddef>

extern "C" void disable_int();

extern "C" void enable_int();

myos::kernel::Keyboard keyboard;

namespace myos {
namespace kernel {

using myos::kernel::RW_port::in;
using myos::kernel::RW_port::out;

void Keyboard::kb_in() {
    uint8_t scancode = in(0x60);
    buffer.append(scancode);
}

uint8_t Keyboard::kb_read() {
    uint8_t scancode;
    if (!buffer.empty()) {
        scancode = buffer.top();
        buffer.pop();
        return scancode;
    }
}

void Keyboard::clean() {
    buffer.clear();
}

unsigned int Keyboard::size() {
    return buffer.size();
}

void Keyboard::reset_kb_buffer() {
    buffer.clear();
}

}
}

