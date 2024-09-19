#include <Arduino.h>
#include <iostream>
#include <iterator>
#include <serial_interface/serial_port.hpp>
#include <type_traits>

namespace serial_port
{
static_assert(std::is_same_v<CharType, std::remove_cv_t<std::remove_reference_t<decltype(*Serial.readString().c_str())>>>);

static StringHandler incomingStringHandler;

std::basic_ostream<CharType> &cout = std::cout;

void initialize()
{
    Serial.begin(BAUD_RATE);
    delay(100);
    Serial.flush();
    delay(100);
}

String readLine()
{
    return String(Serial.readStringUntil('\n').c_str());
}

std::optional<String> getLine()
{
    if (Serial.available() > 0)
    {
        return readLine();
    }
    else
    {
        return {};
    }
}

void setCallbackForLineReception(const StringHandler &callback)
{
    incomingStringHandler = callback;
}

} // namespace serial_port

void serial_port::readAndHandleInput()
{
    while (Serial.available() > 0)
    {
        const auto inData = Serial.read();
        if (inData == -1)
        {
            break;
        }
        else
        {
            static serial_port::String inputBuffer{};
            const serial_port::String::value_type inChar = inData;
            // if the incoming character is a newline, call handler
            if (inChar == '\n' || inChar == '\r')
            {
                if (!inputBuffer.empty())
                {
                    serial_port::incomingStringHandler(inputBuffer);
                }
                inputBuffer.clear();
            }
            else
            {
                inputBuffer += inChar;
            }
        }
    }
}
