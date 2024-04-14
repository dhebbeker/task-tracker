#pragma once
#include <board_types.hpp>
#include <functional>
#include <map>

extern std::map<board::PinType, std::function<void(void)>> isr_collection;
