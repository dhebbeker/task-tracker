#pragma once

#include "IInputShiftRegister.hpp"
#include <RoxMux.h>
#include <Wire.h> // is needed by RoxMux
#include <cstddef>

template <std::size_t numberOfBytes>
class InputShiftRegister : public IInputShiftRegister<numberOfBytes * 8>
{
  public:
    //! Alias to the interface this class implements
    using Base = IInputShiftRegister<numberOfBytes * 8>;
    InputShiftRegister(const std::uint8_t serialDataPin, const std::uint8_t clockPin, const std::uint8_t latchPin)
        : isrBackend()
    {
        isrBackend.begin(serialDataPin, latchPin, clockPin);
    }

    virtual typename Base::Data readRegister() const override
    {
        typename Base::Data data{};
        isrBackend.update();
        std::uint16_t pinIndex = Base::numberOfBits - 1;
        for (auto &pin : data)
        {
            pin = isrBackend.readPin(pinIndex--);
        }
        return data;
    }

  private:
    mutable Rox74HC165<numberOfBytes> isrBackend;
};