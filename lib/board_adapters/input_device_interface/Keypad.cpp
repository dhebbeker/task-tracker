#include "Keypad.hpp"
#include "debouncedIsr.hpp"
#include <Arduino-wrapper.h>
#include <board_pins.hpp>
#include <chrono>
#include <cstddef>
#include <functional>
#include <utility>

using namespace std::chrono_literals;

static HmiHandler callBack;

/**
 * Debounce period.
 * 
 * Key presses must be at least this long to be detected.
 */
static constexpr auto debouncePeriod = 20ms;

static void reactOnPinChange(const board::PinType pin, KeyId keyId)
{
    const bool isPressed = digitalRead(pin) == LOW;
    if (isPressed)
    {
        callBack(keyId);
    }
}

/**
 * Maps HMI buttons to events.
 */
static constexpr std::pair<board::PinType, KeyId> selectionForPins[] = {
    {board::button::pin::task1, KeyId::TASK1},
    {board::button::pin::task2, KeyId::TASK2},
    {board::button::pin::task3, KeyId::TASK3},
    {board::button::pin::task4, KeyId::TASK4},
    {board::button::pin::up, KeyId::LEFT},
    {board::button::pin::down, KeyId::RIGHT},
    {board::button::pin::enter, KeyId::ENTER},
    {board::button::pin::back, KeyId::BACK},
};

Keypad::Keypad()
{
    // input pins
    std::size_t index = 0;
    for (const auto selectionForPin : selectionForPins)
    {
        pinMode(selectionForPin.first, INPUT_PULLUP);
        attachInterrupt(
            digitalPinToInterrupt(selectionForPin.first),
            createDebouncingIsr(std::bind(
                                    reactOnPinChange,
                                    selectionForPin.first,
                                    selectionForPin.second),
                                debouncePeriod),
            CHANGE);
        index++;
    }
}

void Keypad::setCallback(std::function<void(KeyId)> callbackFunction)
{
    callBack = callbackFunction;
}

bool Keypad::isKeyPressed(const KeyId keyInquiry)
{
    return false; //TODO
}
