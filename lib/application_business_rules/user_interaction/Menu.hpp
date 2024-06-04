#pragma once
#include "IGuiEngine.hpp"
#include "user_interaction/IKeypad.hpp"

class Menu
{
  public:
    Menu(IGuiEngine &, IKeypad &keypad);
    virtual void loop();

  private:
    IGuiEngine &guiEngine;
};
