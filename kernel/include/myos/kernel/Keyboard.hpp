#pragma once

#include <Queue.hpp>
#include <cstdint>
//Keyboard structure
namespace myos{
namespace kernel{



using cpplib::Queue;

class Keyboard{
public:
    Keyboard() = default;
    void kb_in();   //keyboard input
    uint8_t kb_read(); //keyboard read, return scan code
    void clean();   //clean buffer
    void reset_kb_buffer(); //reset keyboard buffer
    unsigned int size();
private:
     Queue<uint8_t > buffer;
};

}
}
