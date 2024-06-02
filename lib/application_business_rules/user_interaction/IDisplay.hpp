#pragma once
#include "MenuItem.hpp"

/**
 * Interface to a display capable of displaying various information to a human.
 *
 * This is used to implement the \ref plugin_architecture.
 */
class IDisplay
{
  public:
    virtual void refresh() = 0;
    virtual void drawMenu(const MenuItemList* menuList) = 0;
};
