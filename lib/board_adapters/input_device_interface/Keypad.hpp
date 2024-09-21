#pragma once

#include <user_interaction/IKeypad.hpp>

class Keypad : public IKeypad
{
  public:
    Keypad();
    virtual void setCallback(const HmiHandler callbackFunction) override;
    virtual bool isKeyPressed(KeyId keyInquiry) override;
};
