#include "Display.hpp"
#include <Adafruit_SSD1306.h>
#include <user_interaction/MenuItem.hpp>

#if LV_USE_LOG != 0
#include <Arduino.h>
/* Serial debugging */
static void lvgl_log_to_serial(const char *buf)
{
    Serial.printf(buf);
    Serial.flush();
}
#endif

/* Display flushing */
static void flushSSD1306Adafruit(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    const auto displayAdapter = reinterpret_cast<Display *>(disp_drv->user_data);
    auto &display = displayAdapter->display;
    uint32_t width = (area->x2 - area->x1 + 1);
    uint32_t height = (area->y2 - area->y1 + 1);

    for (uint16_t row = 0; row < height; row++)
    {
        for (uint16_t col = 0; col < width; col++)
        {
            display.drawPixel(area->x1 + col, area->y1 + row, (color_p->full) ? SSD1306_WHITE : SSD1306_BLACK);
            color_p++;
        }
    }
    display.display();
    lv_disp_flush_ready(disp_drv);
}

Display::Display(const Configuration &configuration, TwoWire &i2c)
    : display(configuration.screen_width, configuration.screen_height, &i2c),
      buf(std::make_unique<lv_color_t[]>(configuration.screen_width * 16))
{
#if LV_USE_LOG != 0
    lv_log_register_print_cb(lvgl_log_to_serial); /* register print function for debugging */
#endif
    const std::uint8_t switchVcc = configuration.generateDisplayVoltageInternally ? SSD1306_SWITCHCAPVCC : SSD1306_EXTERNALVCC;
    const bool allocationSuccessful = display.begin(switchVcc, configuration.display_i2c_address);
    assert(allocationSuccessful);

    // Show initial display buffer contents on the screen --
    // the library initializes this with an Adafruit splash screen.
    display.display();
    delay(500); // Pause for short time

    // Clear the buffer
    display.clearDisplay();

    // Show the display buffer on the screen. You MUST call display() after
    // drawing commands to make them visible on screen!
    display.display();

    // Initialize lvgl library
    static lv_disp_draw_buf_t draw_buf;
    lv_init();
    lv_disp_draw_buf_init(&draw_buf, buf.get(), NULL, configuration.screen_width * 16);

    // Initialize the display driver for lvgl
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = configuration.screen_width;
    disp_drv.ver_res = configuration.screen_height;
    disp_drv.flush_cb = flushSSD1306Adafruit;
    disp_drv.draw_buf = &draw_buf;
    disp_drv.user_data = this;

    // Register display driver to lvgl
    lv_disp_drv_register(&disp_drv);
    delay(200);

    // Create monochomatic theme and set it as default
    lv_theme_t *mono_theme = lv_theme_mono_init(0, false, &lv_font_unscii_8);
    lv_disp_set_theme(0, mono_theme);

    // create first text in lvgl
    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "LVGL is up");
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 0);

    // display lvgl screen
    lv_timer_handler();
}

void Display::refresh()
{
    lv_timer_handler();
}

void Display::drawMenu(const MenuItemList *menuList)
{
    ;
}
