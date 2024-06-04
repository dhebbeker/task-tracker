#pragma once
#include "IGuiEngine.hpp"
#include "user_interaction/IKeypad.hpp"

/**
 * @brief class to hold the menu structure of the HMI
 * 
 */
class Menu
{
  public:
    Menu(IGuiEngine &, IKeypad &keypad);
    virtual void loop();

  private:
    IGuiEngine &guiEngine;
};
