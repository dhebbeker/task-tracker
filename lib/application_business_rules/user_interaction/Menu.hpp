#pragma once
#include "IDisplay.hpp"
#include "user_interaction/IKeypad.hpp"

class Menu
{
  public:
    Menu(IDisplay &, IKeypad &keypad);
    virtual void loop();

  private:
    IDisplay &display;
};
