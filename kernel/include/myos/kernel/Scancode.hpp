#pragma once

#include <cstdint>
#include "key.hpp"

namespace myos{
namespace kernel {


kernel_keyboard_key_Key kernel_keyboard_scanCode_toKey (uint8_t scanCode);

}}