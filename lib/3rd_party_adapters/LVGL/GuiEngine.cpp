/**
 * \file .
 * Connects LVGL callbacks to display driver and input device.
 */
#include "GuiEngine.hpp"
#include "Screen.hpp"
#include <Adafruit_SSD1306.h>
#include <user_interaction/MenuItem.hpp>

/* lvgl log functionality via serial interface */
#if LV_USE_LOG != 0
#include <serial_interface/serial_port.hpp>
static void lvgl_log_to_serial(const char *buf)
{
    serial_port::cout << buf << std::endl;
}
#endif

/* Display flushing */
static void flushSSD1306Adafruit(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    const auto displayAdapter = reinterpret_cast<GuiEngine *>(disp_drv->user_data);
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
    //this->display.display();  // this has been removed from here due to performance, as multiple flushes are triggered, when more areas on screen are refreshed
    lv_disp_flush_ready(disp_drv);
}

static IKeypad *myKeypad = nullptr;

/**
 * @brief Construct a new Gui Engine:: GuiEngine object
 * 
 * @param configuration - structure fith configuration information
 * @param i2c           - reference to the i2c/TwoWire object for communication
 */
GuiEngine::GuiEngine(const Configuration &configuration, TwoWire &i2c)
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

/**
 * @brief implementation for lvgl read_cb for the left key
 */
static void keypad_read_left(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    if (!myKeypad)
        return;

    // assign key state
    data->state = (myKeypad->isKeyPressed(KeyId::LEFT)) ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
    data->key = LV_KEY_PREV;
}

/**
 * @brief implementation for lvgl read_cb for the right key
 */
static void keypad_read_right(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    if (!myKeypad)
        return;

    // assign key state
    data->state = (myKeypad->isKeyPressed(KeyId::RIGHT)) ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
    data->key = LV_KEY_NEXT;
}

/**
 * @brief implementation for lvgl read_cb for the enter key
 */
static void keypad_read_enter(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    if (!myKeypad)
        return;

    // assign key state
    data->state = (myKeypad->isKeyPressed(KeyId::ENTER)) ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
    data->key = LV_KEY_ENTER;
}

/**
 * @brief implementation for lvgl read_cb for the back key
 */
static void keypad_read_back(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    if (!myKeypad)
        return;

    // assign key state
    data->state = (myKeypad->isKeyPressed(KeyId::BACK)) ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
    data->key = LV_KEY_ESC;
}

/**
 * @brief register the Keypad to read the button states from
 * 
 * @param keypad - pointer to the Keypad object
 */
void GuiEngine::registerKeyPad(IKeypad *keypad)
{
    //assign keypad reference to local pointer
    myKeypad = keypad;

    // create default group for button navigation and assign input device to it
    static lv_group_t *const group = lv_group_create();
    lv_group_set_default(group);

    // Register all buttons as individual indev, so they all have their separate "old" state (lvgl has only one "old" state per indev)
    static lv_indev_drv_t indev_drv_left;
    lv_indev_drv_init(&indev_drv_left);
    indev_drv_left.type = LV_INDEV_TYPE_KEYPAD;
    indev_drv_left.read_cb = keypad_read_left;

    static lv_indev_drv_t indev_drv_right;
    lv_indev_drv_init(&indev_drv_right);
    indev_drv_right.type = LV_INDEV_TYPE_KEYPAD;
    indev_drv_right.read_cb = keypad_read_right;

    static lv_indev_drv_t indev_drv_enter;
    lv_indev_drv_init(&indev_drv_enter);
    indev_drv_enter.type = LV_INDEV_TYPE_KEYPAD;
    indev_drv_enter.read_cb = keypad_read_enter;

    static lv_indev_drv_t indev_drv_back;
    lv_indev_drv_init(&indev_drv_back);
    indev_drv_back.type = LV_INDEV_TYPE_KEYPAD;
    indev_drv_back.read_cb = keypad_read_back;

    // Register the drivers in LVGL and save the created input device object
    lv_indev_set_group(lv_indev_drv_register(&indev_drv_left), group);
    lv_indev_set_group(lv_indev_drv_register(&indev_drv_right), group);
    lv_indev_set_group(lv_indev_drv_register(&indev_drv_enter), group);
    lv_indev_set_group(lv_indev_drv_register(&indev_drv_back), group);
}

/**
 * @brief cyclic function to be called to handle lvgl
 *   This also flushes the data to the display.
 * 
 */
void GuiEngine::refresh()
{
    lv_timer_handler();
    LV_LOG_USER("Adafruit display() start");
    this->display.display();
    LV_LOG_USER("Adafruit display() end");
}

/**
 * @brief function to draw a menu Screen onto the display
 * 
 * @param menuList - list of items for the menu to display
 */
void GuiEngine::drawMenu(const MenuItemList *menuList)
{
    auto ptrMenuScreen = new ScreenMenu{*menuList};
    ptrMenuScreen->draw();
}
