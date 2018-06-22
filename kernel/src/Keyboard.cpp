#include <myos/kernel/Keyboard.hpp>
#include <myos/kernel/RW_port.hpp>
#include <myos/kernel/Scancode.hpp>
#include <cstdint>
#include <cstddef>
extern "C" void disable_int();
extern "C" void enable_int();
myos::kernel::Keyboard Keyboard_buffer;

namespace myos{
namespace kernel{

using myos::kernel::RW_port::in;
using myos::kernel::RW_port::out;

void Keyboard::kb_in(){
    uint8_t scan_code = in(0x60);
    buffer.append(scan_code);
}

char Keyboard::kb_read()
{
    char scan_code;
    bool make;
    kernel_keyboard_key_Key key;

    if(!buffer.empty())
    {
        disable_int();
        scan_code = buffer.top();
        buffer.pop();
        enable_int();

        //start to analyze scancode
        if(scan_code == 0xE1){
        }
        else if(scan_code == 0xE0)
        {}
        else    //printable char
        {
            make = (scan_code & FLAG_BREAK) == 0;
            if(make)
            {
                key = kernel_keyboard_scanCode_toKey(static_cast<uint8_t>(scan_code & 0x7F));
                return key.keyCode;
            }
        }
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

