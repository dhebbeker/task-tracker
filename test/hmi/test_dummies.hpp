#pragma once
#include <board_types.hpp>
#include <functional>
#include <map>

extern std::map<board::PinType, std::function<void(void)>> isr_collection;

/**
 * Triggers a interrupt and sets the internal state.
 */
void changeButtonState(board::PinType pin);
