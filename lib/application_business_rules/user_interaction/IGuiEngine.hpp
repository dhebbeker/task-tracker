#pragma once
#include "MenuItem.hpp"
#include "user_interaction/IKeypad.hpp"

/**
 * Interface to a guiEngine capable of displaying various information to a human.
 *
 * This is used to implement the \ref plugin_architecture.
 */
class IGuiEngine
{
  public:
    virtual void registerKeyPad(IKeypad *keypad) = 0;
    virtual void refresh() = 0;
    virtual void drawMenu(const MenuItemList *menuList) = 0;
};
