#include "test_dummies.hpp"
#include <input_device_interface/debouncedIsr.hpp>
#include <iomanip>
#include <iostream>

std::map<board::PinType, std::function<void(void)>> isr_collection;

// test dummy for Arduino-ESP32 specific function
void attachInterrupt(const uint8_t interruptNum, const std::function<void(void)> userFunc, const int mode)
{
    isr_collection.emplace(interruptNum, userFunc);
    std::cout << "Added ISR for interrupt number " << static_cast<unsigned int>(interruptNum)
              << " function pointer " << std::showbase << std::hex << reinterpret_cast<std::intptr_t>(userFunc.target<void (*)(void)>()) << std::resetiosflags(std::ios::showbase | std::ios::basefield) << std::endl;
}

// test dummy for a FreeRTOS adapter function
int debouncer::defaultPriorityToInt()
{
    return 0xC0FFE;
}

// test dummy for a FreeRTOS adapter function
std::function<void(void)> createDebouncer(const std::function<void(void)> handler, std::chrono::milliseconds, int)
{
    return handler;
}
