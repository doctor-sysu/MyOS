#pragma once

#include <Queue.hpp>
#include <cstdint>
//Keyboard structure
namespace myos{
namespace kernel{

#define FLAG_BREAK 0x80

using cpplib::Queue;

class Keyboard{
public:
    Keyboard() = default;
    //void init_Keyboard();
    void kb_in();   //keyboard input
    char kb_read(); //keyboard read
    void clean();   //clean buffer
    void reset_kb_buffer(); //reset keyboard buffer
    unsigned int size();
private:
     Queue<char> buffer;
};

}
}
