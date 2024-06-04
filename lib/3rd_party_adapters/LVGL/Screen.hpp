#pragma once

#include <lvgl.h>
#include <user_interaction/MenuItem.hpp>

class IScreen
{
  public:
    IScreen() = default;
    virtual ~IScreen() = default;

    virtual void draw() = 0;
};

class ScreenMenu final : public IScreen
{
  public:
    ScreenMenu(MenuItemList itemList);
    ~ScreenMenu() override = default;

    void draw() override;

  private:
    const MenuItemList _List;
};

class ScreenValueModifier final : public IScreen
{
  public:
    ScreenValueModifier(const MenuItemValue *const menuItem);
    ~ScreenValueModifier() override = default;

    void draw() override;

    const MenuItemValue *const _menuItem;
    lv_obj_t *_spinbox;
};
