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
    //void init_Keyboard();
    void kb_in();   //keyboard input
    void clean();   //clean buffer
    unsigned int size();
private:
     Queue<char> buffer;
};

}
}
