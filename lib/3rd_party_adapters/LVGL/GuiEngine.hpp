#pragma once

#include <Adafruit_SSD1306.h>
#include <cstdint>
#include <lvgl.h>
#include <memory>
#include <user_interaction/IGuiEngine.hpp>

/**
 * @brief lvgl version of a GuiEngine
 * 
 */
class GuiEngine : public IGuiEngine
{
  public:
    struct Configuration
    {
        std::uint8_t screen_width;
        std::uint8_t screen_height;
        bool generateDisplayVoltageInternally;
        std::uint8_t display_i2c_address;
    };
    GuiEngine(const Configuration &configuration, TwoWire &i2c);
    virtual void registerKeyPad(IKeypad *keypad) override;
    virtual void refresh() override;
    virtual void drawMenu(const MenuItemList *menuList) override;

    Adafruit_SSD1306 display;

  private:
    const std::unique_ptr<lv_color_t[]> buf;
};
