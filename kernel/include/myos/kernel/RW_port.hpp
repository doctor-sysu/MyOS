#pragma once

#include <cstdint>
#include <cstddef>
namespace myos{
namespace kernel{
namespace RW_port{

uint8_t in(uint32_t port);
void out(uint32_t port, uint8_t data);

}
}
}
