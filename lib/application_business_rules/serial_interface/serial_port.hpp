#pragma once

#include <array>
#include <functional>
#include <optional>
#include <ostream>
#include <string>

namespace serial_port
{
/**
 * Character type used by serial port.
 */
typedef char CharType;

/**
 * String type provided by serial port.
 */
typedef std::basic_string<CharType> String;

/**
 * Output stream for characters to serial port.
 * 
 * \pre call \ref initialize() before using
 */
extern std::basic_ostream<CharType> &cout;

/**
 * Configures and initializes serial port.
 */
void initialize();

/**
 * Reads a line from serial port.
 * 
 * Expects the end of line to be `\n`.
 * It will wait for data for the duration of the timeout.
 * \returns an empty string in case no data is read
 */
String readLine();

/**
 * Gets a line from serial port.
 * 
 * Expects the end of line to be `\n`.
 * \returns an object that does not contain a value in case no data is already available.
 */
std::optional<String> getLine();

/**
 * Callback which can handle strings.
 */
typedef std::function<void(const String &)> StringHandler;

/**
 * Set the handler to be called when a full line has been received via serial_port.
 * \param callback 
 */
void setCallbackForLineReception(const StringHandler &callback);

} // namespace serial_port

/**
 * Writes formatted bits to `ostream`.
 * 
 * \tparam BITS number of bits
 * \param os output stream to write to
 * \param bitArray the bits to output
 * \returns the stream
 */
template <std::size_t BITS, typename CharType = char>
std::ostream &operator<<(std::basic_ostream<CharType> &os, const std::array<bool, BITS> &bitArray)
{
    os << "0b";
    for (const bool bit : bitArray)
    {
        if (bit)
        {
            os << "1";
        }
        else
        {
            os << "0";
        }
    }
    return os;
}
