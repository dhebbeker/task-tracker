#pragma once

#include <lvgl.h>
#include <user_interaction/MenuItem.hpp>

/**
 * @brief Interface class for Screen objects
 * 
 */
class IScreen
{
  public:
    IScreen() = default;
    virtual ~IScreen() = default;

    virtual void draw() = 0;
};

/**
 * @brief Screen for menu navigation
 * it receives a list of menu items for it's topology
 * 
 */
class ScreenMenu final : public IScreen
{
  public:
    ScreenMenu(MenuItemList itemList);
    ~ScreenMenu() override = default;

    void draw() override;

  private:
    const MenuItemList _List;
};

/**
 * @brief Screen for value modification
 * 
 */
class ScreenValueModifier final : public IScreen
{
  public:
    ScreenValueModifier(const MenuItemValue *const menuItem);
    ~ScreenValueModifier() override = default;

    void draw() override;

    const MenuItemValue *const _menuItem;
    lv_obj_t *_spinbox;
};
