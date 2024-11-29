#include "Screen.hpp"
#include <math.h>
#include <stack>
#include <string>

std::shared_ptr<IScreen> CurrentScreen;
static std::stack<std::shared_ptr<IScreen>> screenHistory;

static inline void IScreen_leave()
{
    //only leave if we have something to go back to
    if (screenHistory.size() > 0)
    {
        //clear the current screen of lvgl
        lv_obj_clean(lv_scr_act());

        //get previous screen
        CurrentScreen = screenHistory.top();
        //remove the previous screen from list
        screenHistory.pop();

        //draw the previous screen
        CurrentScreen->draw();
    }
    else
    {
        LV_LOG_INFO("No previous screen!");
    }
}

/**
 * @brief Event callback function for an item that calls another submenu
 * @note  The lvgl event user data hold a pointer to the triggered submenu item
 * 
 * @param e - pointer to lvgl event object
 */
static void ScreenMenu_submenu_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    auto item = reinterpret_cast<const MenuItemSubmenu *const>(lv_event_get_user_data(e));

    if ((code == LV_EVENT_SHORT_CLICKED) && (item != nullptr))
    {
        //save current screen in history
        screenHistory.push(CurrentScreen);
        //if we were clicked shortly, draw the next menu screen
        lv_obj_clean(lv_scr_act());
        CurrentScreen = std::make_shared<ScreenMenu>(*item->getSubMenuList());
        CurrentScreen->draw();
    }
    else if (code == LV_EVENT_KEY)
    {
        uint32_t key = lv_event_get_key(e);
        if (key == LV_KEY_ESC)
        {
            //if we receive the back button, go one screen back
            IScreen_leave();
        }
    }
}

/**
 * @brief Event callback function for a switch item that modifies a bool variable
 * @note  The lvgl event user data hold a pointer to the triggered switch item
 * 
 * @param e - pointer to lvgl event object
 */
static void ScreenMenu_switch_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    auto item = reinterpret_cast<const MenuItemSwitch *const>(lv_event_get_user_data(e));

    if ((code == LV_EVENT_VALUE_CHANGED) && (item != nullptr))
    {
        //if we were clicked the switch, change the value of the original variable
        bool *ptr = item->getPtrBool();
        if (ptr)
        {
            *ptr = lv_obj_has_state(obj, LV_STATE_CHECKED) ? 1 : 0;
        }
    }
    else if (code == LV_EVENT_KEY)
    {
        uint32_t key = lv_event_get_key(e);
        if (key == LV_KEY_ESC)
        {
            //if we receive the back button, go one screen back
            IScreen_leave();
        }
    }
}

/**
 * @brief Event callback function for a value item that modifies a float variable
 * @note  The lvgl event user data hold a pointer to the triggered value item
 * 
 * @param e - pointer to lvgl event object
 */
static void ScreenMenu_value_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    auto item = reinterpret_cast<const MenuItemValue *const>(lv_event_get_user_data(e));

    if ((code == LV_EVENT_SHORT_CLICKED) && (item != nullptr))
    {
        //save current screen in history
        screenHistory.push(CurrentScreen);
        //if we were clicked shortly, draw the value modification screen
        CurrentScreen = std::make_shared<ScreenValueModifier>(item);
        CurrentScreen->draw();
    }
    else if (code == LV_EVENT_KEY)
    {
        uint32_t key = lv_event_get_key(e);
        if (key == LV_KEY_ESC)
        {
            //if we receive the back button, go one screen back
            IScreen_leave();
        }
    }
}

/**
 * @brief Construct a new ScreenMenu object
 * 
 * @param itemList - List of items to be drawn with this menu
 */
ScreenMenu::ScreenMenu(MenuItemList itemList)
    : _List{itemList}
{
}

static lv_style_t style_small_padding;

static void drawTaskList(lv_obj_t &screen)
{
    lv_obj_t *table = lv_table_create(&screen);

    uint16_t row = 0;
    for (auto task_item : device::tasks)
    {
        lv_table_set_cell_value(table, row, 0, task_item.second.getLabel().c_str());
        const auto time_label = std::to_string(task_item.second.getRecordedDuration().count()) + "s";
        lv_table_set_cell_value(table, row, 1, time_label.c_str());
        row++;
    }

    lv_table_set_col_width(table, 0, 86);
    lv_table_set_col_width(table, 1, 42);

    lv_obj_add_style(table, &style_small_padding, 0);
}

/**
 * @brief Translates the list of item types into actual lvgl draw directives.
 */
void ScreenMenu::draw()
{
    /* clear the current displayed screen */
    lv_obj_clean(lv_scr_act());

    /* adjust the style so everything has a 1 px padding in all directions */
    static bool initialized = false;
    if (!initialized)
    {
        lv_style_init(&style_small_padding);
        initialized = true;
    }
    lv_style_set_pad_left(&style_small_padding, 1);
    lv_style_set_pad_top(&style_small_padding, 1);
    lv_style_set_pad_bottom(&style_small_padding, 1);
    lv_style_set_pad_right(&style_small_padding, 1);

    /* create the lvgl screen object and configure it's properties */
    static lv_obj_t *screen = lv_obj_create(NULL);
    lv_obj_add_style(screen, &style_small_padding, 0);
    lv_obj_set_flex_flow(screen, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_row(screen, 2, 0);
    lv_obj_set_flex_align(screen, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_scrollbar_mode(screen, LV_SCROLLBAR_MODE_OFF);

    /* draw each item */
    for (auto const &item : this->_List)
    {
        switch (item->getType())
        {
        case MenuItemType::SUBMENU: {
            /* draw submenu button */
            auto btnItem = reinterpret_cast<const MenuItemSubmenu *const>(item);
            auto btn = lv_btn_create(screen);
            lv_obj_set_size(btn, lv_pct(100), 12);
            lv_obj_add_style(btn, &style_small_padding, 0);
            lv_obj_add_event_cb(btn, ScreenMenu_submenu_cb, LV_EVENT_SHORT_CLICKED, (void *)item); /* assign the submenu callback for event short clicked */
            lv_obj_add_event_cb(btn, ScreenMenu_submenu_cb, LV_EVENT_KEY, nullptr);                /* assign the submenu callback for event key press */
            auto lab = lv_label_create(btn);
            lv_obj_set_width(lab, lv_pct(100));
            lv_obj_set_align(lab, LV_ALIGN_LEFT_MID);
            lv_label_set_long_mode(lab, LV_LABEL_LONG_SCROLL);
            lv_label_set_text(lab, item->getText().c_str());
            break;
        }
        case MenuItemType::SWITCH: {
            /* create style without border for disabled switch container */
            static lv_style_t style_no_border;
            lv_style_init(&style_no_border);
            lv_style_set_border_width(&style_no_border, 0);

            /* draw switch */
            auto swtItem = reinterpret_cast<const MenuItemSwitch *const>(item);
            auto cont = lv_obj_create(screen);
            lv_obj_set_size(cont, lv_pct(100), 12);
            lv_obj_add_style(cont, &style_small_padding, 0);
            lv_obj_set_scrollbar_mode(cont, LV_SCROLLBAR_MODE_OFF);
            lv_obj_add_flag(cont, LV_OBJ_FLAG_OVERFLOW_VISIBLE);
            auto lab = lv_label_create(cont);
            lv_obj_set_width(lab, lv_pct(70));
            lv_obj_set_align(lab, LV_ALIGN_LEFT_MID);
            lv_label_set_long_mode(lab, LV_LABEL_LONG_SCROLL);
            lv_label_set_text(lab, item->getText().c_str());
            auto swth = lv_switch_create(cont);
            lv_obj_set_size(swth, 18, 12);
            lv_obj_set_align(swth, LV_ALIGN_RIGHT_MID);

            bool *ptr = swtItem->getPtrBool();
            if (ptr == nullptr)
            {
                /* if pointer to bool variable is not valid, disable the switch and don't add callbacks */
                lv_obj_add_state(swth, LV_STATE_DISABLED);

                /* change style so container seems disabled as well */
                lv_obj_add_style(cont, &style_no_border, 0);
            }
            else
            {
                /* if pointer to bool variable is valid, show it's state and assign callbacks */
                lv_obj_add_state(swth, (*ptr) ? LV_STATE_CHECKED : LV_STATE_DEFAULT);
                lv_obj_add_event_cb(swth, ScreenMenu_switch_cb, LV_EVENT_VALUE_CHANGED, (void *)item); /* assign the switch callback for event value change*/
                lv_obj_add_event_cb(swth, ScreenMenu_switch_cb, LV_EVENT_KEY, nullptr);                /* assign the switch callback for event key press */
            }
            break;
        }
        case MenuItemType::VALUE: {
            /* draw value */
            auto valItem = reinterpret_cast<const MenuItemValue *const>(item);
            auto btn = lv_btn_create(screen);
            lv_obj_set_size(btn, lv_pct(100), 12);
            lv_obj_add_style(btn, &style_small_padding, 0);
            auto lab = lv_label_create(btn);
            lv_obj_set_width(lab, lv_pct(70));
            lv_obj_set_align(lab, LV_ALIGN_LEFT_MID);
            lv_label_set_long_mode(lab, LV_LABEL_LONG_SCROLL);
            lv_label_set_text(lab, item->getText().c_str());
            lab = lv_label_create(btn);
            lv_obj_set_width(lab, lv_pct(25));
            lv_obj_set_align(lab, LV_ALIGN_RIGHT_MID);
            lv_label_set_long_mode(lab, LV_LABEL_LONG_SCROLL);
            lv_obj_set_style_text_align(lab, LV_TEXT_ALIGN_RIGHT, 0);

            double *ptr = valItem->getPtrDouble();
            if (ptr == nullptr)
            {
                /* if pointer to double variable is not valid, disable the switch and don't add callbacks */
                lv_obj_add_state(btn, LV_STATE_DISABLED);
                lv_label_set_text(lab, "NULL");
            }
            else
            {
                /* if pointer to double variable is valid, show it's value and assign callbacks */
                lv_label_set_text_fmt(lab, "%.*f", valItem->getDecimals(), *valItem->getPtrDouble());
                lv_obj_add_event_cb(btn, ScreenMenu_value_cb, LV_EVENT_SHORT_CLICKED, (void *)item); /* assign the value callback for event short clicked */
                lv_obj_add_event_cb(btn, ScreenMenu_value_cb, LV_EVENT_KEY, nullptr);                /* assign the value callback for event key press */
            }
            break;
        }
        case MenuItemType::TASK:
            drawTaskList(*screen);
        }
    }

    /* actually draw the screen with lvgl */
    lv_scr_load(screen);
}

/**
 * @brief Event callback function for the incrementation button in modifier screen
 * @note  The lvgl event user data hold a pointer to the spinbox lvgl object
 * 
 * @param e - pointer to lvgl event object
 */
static void ScreenValueModifier_inc_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    auto spinbox = reinterpret_cast<lv_obj_t *>(lv_event_get_user_data(e));

    if (((code == LV_EVENT_SHORT_CLICKED) || (code == LV_EVENT_LONG_PRESSED_REPEAT)) && (spinbox != nullptr))
    {
        //if we were clicked shortly, or press it for long time, increment the spinbox object, given the object pointer is ok
        lv_spinbox_increment(spinbox);
    }
    else if (code == LV_EVENT_KEY)
    {
        uint32_t key = lv_event_get_key(e);
        if (key == LV_KEY_ESC)
        {
            //if we receive the back button, go one screen back
            IScreen_leave();
        }
    }
}

/**
 * @brief Event callback function for the decrementation button in modifier screen
 * @note  The lvgl event user data hold a pointer to the spinbox lvgl object
 * 
 * @param e - pointer to lvgl event object
 */
static void ScreenValueModifier_dec_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    auto spinbox = reinterpret_cast<lv_obj_t *>(lv_event_get_user_data(e));

    if ((code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) && (spinbox != nullptr))
    {
        //if we were clicked shortly, or press it for long time, decrement the spinbox object, given the object pointer is ok
        lv_spinbox_decrement(spinbox);
    }
    else if (code == LV_EVENT_KEY)
    {
        uint32_t key = lv_event_get_key(e);
        if (key == LV_KEY_ESC)
        {
            //if we receive the back button, go one screen back
            IScreen_leave();
        }
    }
}

/**
 * @brief Event callback function for the step button in modifier screen
 * @note  The lvgl event user data hold a pointer to the spinbox lvgl object
 * 
 * @param e - pointer to lvgl event object
 */
static void ScreenValueModifier_step_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    auto spinbox = reinterpret_cast<lv_obj_t *>(lv_event_get_user_data(e));

    if ((code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LONG_PRESSED_REPEAT) && (spinbox != nullptr))
    {
        //if we were clicked shortly, or press it for long time, increment the step of spinbox object, given the object pointer is ok
        uint32_t step = lv_spinbox_get_step(spinbox);
        step = (step > 100000) ? 1 : (step * 10); //multiply old step by 10 or set it to 1, if the value is beyond 1.000.000
        lv_spinbox_set_step(spinbox, step);
    }
    else if (code == LV_EVENT_KEY)
    {
        uint32_t key = lv_event_get_key(e);
        if (key == LV_KEY_ESC)
        {
            //if we receive the back button, go one screen back
            IScreen_leave();
        }
    }
}

/**
 * @brief Event callback function for the spinbox in modifier screen
 * @note  The lvgl event user data hold a pointer to the menu item, from where the modifier screen was called from
 * 
 * @param e - pointer to lvgl event object
 */
static void ScreenValueModifier_valueChange_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    auto screen = reinterpret_cast<ScreenValueModifier *>(lv_event_get_user_data(e));

    if ((code == LV_EVENT_VALUE_CHANGED) && (screen != nullptr) && (screen->_menuItem != nullptr) && (screen->_spinbox != nullptr))
    {
        //if we changed the value of the spinbox, write it according the decimals to the source variable.
        auto ptr = screen->_menuItem->getPtrDouble();
        *ptr = ((double)lv_spinbox_get_value(screen->_spinbox)) / std::pow(10, screen->_menuItem->getDecimals());
    }
    else if (code == LV_EVENT_KEY)
    {
        uint32_t key = lv_event_get_key(e);
        if (key == LV_KEY_ESC)
        {
            //if we receive the back button, go one screen back
            //TODO: consider writing the new value on exit not on change
            IScreen_leave();
        }
    }
}

/**
 * @brief Construct a new ScreenValueModifier object
 * 
 * @param menuItem - Pointer to menu item that has called the modifier screen
 */
ScreenValueModifier::ScreenValueModifier(const MenuItemValue *const menuItem)
    : _menuItem{menuItem}
{
}

/**
 * @brief Draws a new screen, with a spinbox and 3 buttons for incrementation, decrementation and step modification.
 */
void ScreenValueModifier::draw()
{
    lv_obj_t *btn;
    lv_obj_t *lab;

    /* clear the current displayed screen */
    lv_obj_clean(lv_scr_act());

    /* adjust the style so everything has a 1 px padding in all directions */
    static lv_style_t style_small_padding;
    lv_style_init(&style_small_padding);
    lv_style_set_pad_left(&style_small_padding, 1);
    lv_style_set_pad_top(&style_small_padding, 1);
    lv_style_set_pad_bottom(&style_small_padding, 1);
    lv_style_set_pad_right(&style_small_padding, 1);

    /* create the lvgl screen object and configure it's properties */
    lv_obj_t *screen = lv_obj_create(NULL);
    lv_obj_add_style(screen, &style_small_padding, 0);

    /* draw spinbox */
    _spinbox = lv_spinbox_create(screen);
    lv_group_remove_obj(_spinbox); //remove the spinbox from being selectable by default
    lv_obj_set_width(_spinbox, lv_pct(55));
    lv_obj_add_style(_spinbox, &style_small_padding, 0);
    lv_obj_center(_spinbox);
    lv_spinbox_set_digit_format(_spinbox, 7, (7 - _menuItem->getDecimals()));
    int32_t min = ((_menuItem->getMin()) * std::pow(10, _menuItem->getDecimals()));
    int32_t max = ((_menuItem->getMax()) * std::pow(10, _menuItem->getDecimals()));
    lv_spinbox_set_range(_spinbox, min, max);
    int32_t val = ((*_menuItem->getPtrDouble()) * std::pow(10, _menuItem->getDecimals()));
    lv_spinbox_set_value(_spinbox, val);
    lv_obj_add_event_cb(_spinbox, ScreenValueModifier_valueChange_cb, LV_EVENT_VALUE_CHANGED, (void *)this);
    lv_obj_add_event_cb(_spinbox, ScreenValueModifier_valueChange_cb, LV_EVENT_KEY, nullptr);

    lv_coord_t h = lv_obj_get_height(_spinbox);

    /* draw incrementation button */
    btn = lv_btn_create(screen);
    lv_obj_set_size(btn, h, h);
    lv_obj_add_style(btn, &style_small_padding, 0);
    lv_obj_align_to(btn, _spinbox, LV_ALIGN_OUT_RIGHT_MID, 2, 0);
    lv_obj_add_event_cb(btn, ScreenValueModifier_inc_cb, LV_EVENT_LONG_PRESSED_REPEAT, _spinbox);
    lv_obj_add_event_cb(btn, ScreenValueModifier_inc_cb, LV_EVENT_SHORT_CLICKED, _spinbox);
    lv_obj_add_event_cb(btn, ScreenValueModifier_inc_cb, LV_EVENT_KEY, nullptr);
    lab = lv_label_create(btn);
    lv_label_set_text_static(lab, "+");
    lv_obj_set_align(lab, LV_ALIGN_CENTER);

    /* draw decrementation button */
    btn = lv_btn_create(screen);
    lv_obj_set_size(btn, h, h);
    lv_obj_add_style(btn, &style_small_padding, 0);
    lv_obj_align_to(btn, _spinbox, LV_ALIGN_OUT_LEFT_MID, -2, 0);
    lv_obj_add_event_cb(btn, ScreenValueModifier_dec_cb, LV_EVENT_LONG_PRESSED_REPEAT, _spinbox);
    lv_obj_add_event_cb(btn, ScreenValueModifier_dec_cb, LV_EVENT_SHORT_CLICKED, _spinbox);
    lv_obj_add_event_cb(btn, ScreenValueModifier_dec_cb, LV_EVENT_KEY, nullptr);
    lab = lv_label_create(btn);
    lv_label_set_text_static(lab, "-");
    lv_obj_set_align(lab, LV_ALIGN_CENTER);

    /* draw step modification button */
    btn = lv_btn_create(screen);
    lv_obj_set_size(btn, 10, 10);
    lv_obj_add_style(btn, &style_small_padding, 0);
    lv_obj_align_to(btn, _spinbox, LV_ALIGN_OUT_BOTTOM_MID, 0, 2);
    lv_obj_add_event_cb(btn, ScreenValueModifier_step_cb, LV_EVENT_LONG_PRESSED_REPEAT, _spinbox);
    lv_obj_add_event_cb(btn, ScreenValueModifier_step_cb, LV_EVENT_SHORT_CLICKED, _spinbox);
    lv_obj_add_event_cb(btn, ScreenValueModifier_step_cb, LV_EVENT_KEY, nullptr);
    lab = lv_label_create(btn);
    lv_label_set_text_static(lab, "*");
    lv_obj_set_align(lab, LV_ALIGN_CENTER);

    /* actually draw the screen with lvgl */
    lv_scr_load(screen);
}
