#include "Keypad.hpp"
#include "debouncedIsr.hpp"
#include <Arduino-wrapper.h>
#include <algorithm>
#include <array>
#include <board_pins.hpp>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <functional>
#include <iterator>
#include <optional>
#include <utility>

#if __has_include(<FunctionalInterrupt.h>) // specific to Arduino-ESP32
#include <FunctionalInterrupt.h>
#else // we assume we build for unit tests
extern void attachInterrupt(uint8_t pin, std::function<void(void)> intRoutine, int mode); // must be defined by the test
#endif

using namespace std::chrono_literals;

static HmiHandler callBack;

/**
 * Debounce period.
 * 
 * Key presses must be at least this long to be detected.
 */
static constexpr auto debouncePeriod = 20ms;

/**
 * Maps HMI buttons to events.
 */
static constexpr std::pair<board::PinType, KeyId> selectionForPins[] = {
    {board::button::pin::task1, KeyId::TASK1},
    {board::button::pin::task2, KeyId::TASK2},
    {board::button::pin::task3, KeyId::TASK3},
    {board::button::pin::task4, KeyId::TASK4},
    {board::button::pin::left, KeyId::LEFT},
    {board::button::pin::right, KeyId::RIGHT},
    {board::button::pin::enter, KeyId::ENTER},
    {board::button::pin::back, KeyId::BACK},
};

static std::array<bool, std::size(selectionForPins)> keyPressedState;

static std::optional<std::size_t> getStateIndex(const KeyId keyId)
{
    for (std::size_t index = 0; index < std::size(selectionForPins); ++index)
    {
        if (selectionForPins[index].second == keyId)
        {
            return index;
        }
    }
    return std::nullopt;
}

/**
 * Reacts on a debounced (stabilized) pin change.
 *
 * It will be checked if the pin has "active" state.
 * If the pin is active, the callback handler will be called.
 *
 * @param pin must be the I/O pin which has changed
 * @param keyId is an argument which will be passed to the callback handler
 */
static void reactOnPinChange(const board::PinType pin, KeyId keyId)
{
    const bool isPressed = digitalRead(pin) == LOW;
    if (isPressed)
    {
        callBack(keyId);
    }
    keyPressedState.at(getStateIndex(keyId).value()) = isPressed;
}

Keypad::Keypad()
{
    // input pins
    std::size_t index = 0;
    for (const auto selectionForPin : selectionForPins)
    {
        pinMode(selectionForPin.first, INPUT_PULLUP);
        attachInterrupt(
            digitalPinToInterrupt(selectionForPin.first),
            createDebouncer(std::bind(
                                reactOnPinChange,
                                selectionForPin.first,
                                selectionForPin.second),
                            debouncePeriod),
            CHANGE);
        index++;
    }
    std::fill(std::begin(keyPressedState), std::end(keyPressedState), false);
}

void Keypad::setCallback(std::function<void(KeyId)> callbackFunction)
{
    callBack = callbackFunction;
}

bool Keypad::isKeyPressed(const KeyId keyInquiry)
{
    return keyPressedState.at(getStateIndex(keyInquiry).value());
}
