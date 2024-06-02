/**
 * @file .
 * Provides named objects for the pins provided by the board.
 */
#pragma once
#include "board_types.hpp"

namespace board
{
namespace button::pin
{
constexpr PinType left = 7;
constexpr PinType right = 4;
constexpr PinType enter = 5;
constexpr PinType back = 6;
constexpr PinType task1 = 9;
constexpr PinType task2 = 8;
constexpr PinType task3 = 18;
constexpr PinType task4 = 17;
} // namespace button::pin

namespace led::pin
{
constexpr PinType task1 = 13;
constexpr PinType task2 = 12;
constexpr PinType task3 = 11;
constexpr PinType task4 = 10;
} // namespace led::pin

/**
 * Pins to control the buzzer.
 */
namespace buzzer::pin
{
/**
 * Powers pin of the buzzer.
 */
constexpr PinType on_off = 14;
} // namespace buzzer::pin

/**
 * Pins for I2C controller 1.
 */
namespace i2c_1::pin
{
/**
 * Reset pin.
 * 
 * Active low.
 */
constexpr PinType res = 35;

/**
 * Serial data.
 */
constexpr PinType sda = 37;

/**
 * Serial clock.
 */
constexpr PinType scl = 36;
} // namespace i2c_1::pin

} // namespace board
