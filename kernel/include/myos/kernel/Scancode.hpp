#pragma once

#include <cstdint>
#include "key.hpp"
#include "Keyboard.hpp"
namespace myos{
namespace kernel {

static bool _ctrl = false;
kernel_keyboard_key_Key kernel_keyboard_scanCode_toKey (uint8_t scanCode);

}}