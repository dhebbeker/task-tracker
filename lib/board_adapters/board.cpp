#include "board.hpp"
#include "board_pins.hpp"
#include <Arduino.h>
#include <array>
#include <functional>
#include <iterator>
#include <stdexcept>
#include <utility>

static board::HmiHandler callBack;

template <board::HmiSelection SELECTION>
static void isr()
{
    callBack(SELECTION);
}

/**
 * This stores the array time and size.
 * 
 */
template <class T, std::size_t N>
struct H
{
    template <T (&A)[N]>
    constexpr static auto aH1()
    {
        return aH2<A>(std::make_index_sequence<N>());
    }

    /**
     * We must get the array as template argument. 
     * Else the resolution of the template argument to isr<>() would not be at compile time.
     */
    template <T (&A)[N], std::size_t... Is>
    constexpr static std::array<void (*)(), N> aH2(const std::index_sequence<Is...>)
    {
        return {isr<A[Is].second>...};
    }
};

/**
 * Helps to deduce the array type and size.
 */
template <class T, std::size_t N>
static constexpr H<T, N> makeH(T (&)[N])
{
    return H<T, N>();
}

static constexpr std::pair<PinType, board::HmiSelection> selectionForPins[] = {
    {board::button::pin::task1, board::HmiSelection::TASK1},
    {board::button::pin::task2, board::HmiSelection::TASK2},
    {board::button::pin::task3, board::HmiSelection::TASK3},
    {board::button::pin::task4, board::HmiSelection::TASK4},
    {board::button::pin::up, board::HmiSelection::UP},
    {board::button::pin::down, board::HmiSelection::DOWN},
    {board::button::pin::enter, board::HmiSelection::ENTER},
    {board::button::pin::back, board::HmiSelection::BACK},
};

void board::setup(const HmiHandler callbackFunction)
{
    // input pins
    callBack = callbackFunction;
    constexpr auto functionPointers = makeH(selectionForPins).aH1<selectionForPins>();
    std::size_t index = 0;
    for (const auto selectionForPin : selectionForPins)
    {
        pinMode(selectionForPin.first, INPUT_PULLUP);
        attachInterrupt(
            digitalPinToInterrupt(selectionForPin.first),
            functionPointers.at(index),
            FALLING);
        index++;
    }

    // output pins
    constexpr PinType outputPins[] = {
        board::led::pin::task1,
        board::led::pin::task2,
        board::led::pin::task3,
        board::led::pin::task4,
        board::buzzer::pin::on_off,
    };
    for (const auto outputPin : outputPins)
    {
        pinMode(outputPin, OUTPUT);
    }
}

void board::playTone(const unsigned int frequency, const std::chrono::milliseconds duration)
{
    tone(board::buzzer::pin::on_off, frequency, duration.count());
}

static PinType mapIndicatorIdToPin(const board::StatusIndicatorId id)
{
    switch (id)
    {
    case board::StatusIndicatorId::TASK1:
        return board::led::pin::task1;
        break;
    case board::StatusIndicatorId::TASK2:
        return board::led::pin::task2;
        break;
    case board::StatusIndicatorId::TASK3:
        return board::led::pin::task3;
        break;
    case board::StatusIndicatorId::TASK4:
        return board::led::pin::task4;
        break;
    default:
        throw std::runtime_error("no pin defined for selected status indicator");
        break;
    }
}

void board::setStatusIndicator(const StatusIndicatorId indicatorId, const bool toActive)
{
    const auto pin = mapIndicatorIdToPin(indicatorId);
    if (!toActive)
    {
        constexpr int minBrightness = 0;
        analogWrite(pin, minBrightness);
    }
    else
    {
        constexpr int maxBrightness = 255;
        constexpr int brightness = maxBrightness * 25 / 100.0;
        analogWrite(pin, brightness);
    }
}
