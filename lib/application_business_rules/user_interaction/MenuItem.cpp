#include "MenuItem.hpp"

/**
 * @brief Construct a new MenuItemSubmenu:: MenuItemSubmenu object
 * 
 * @param text          - text to be shown on the button
 * @param subMenuList   - list of menu items for the sub menu tu be drawn
 */
MenuItemSubmenu::MenuItemSubmenu(std::string text, const MenuItemList *subMenuList)
    : _text{text}, _subMenuList{subMenuList}
{
}

/**
 * @brief returns the text of this item
 * 
 */
std::string MenuItemSubmenu::getText() const
{
    return this->_text;
}

/**
 * @brief returns a constant pointer to the constant SubMenuList
 * 
 */
const MenuItemList *MenuItemSubmenu::getSubMenuList() const
{
    return this->_subMenuList;
}

/**
 * @brief Construct a new Menu Item Switch:: Menu Item Switch object
 * 
 * @param text      - text to be shown on the button
 * @param ptrBool   - pointer to the variable to be modified and shown
 */
MenuItemSwitch::MenuItemSwitch(std::string text, bool *ptrBool)
    : _text{text}, _ptrBool{ptrBool}
{
}

/**
 * @brief returns the text of this item
 * 
 */
std::string MenuItemSwitch::getText() const
{
    return this->_text;
}

/**
 * @brief returns a pointer to the modification variable
 * 
 */
bool *MenuItemSwitch::getPtrBool() const
{
    return this->_ptrBool;
}

/**
 * @brief Construct a new Menu Item Value:: Menu Item Value object
 * 
 * @param text      - text to be shown on the button
 * @param ptrDouble - pointer to the variable to be modified and shown
 * @param decimals  - amoint of decimal digits
 * @param min       - minimum value
 * @param max       - maximum value
 */
MenuItemValue::MenuItemValue(std::string text, double *ptrDouble, uint8_t decimals, double min, double max)
    : _text{text}, _ptrDouble{ptrDouble}, _decimals{decimals}, _min{min}, _max{max}
{
}

/**
 * @brief returns the text of this item
 * 
 */
std::string MenuItemValue::getText() const
{
    return this->_text;
}

/**
 * @brief returns a pointer to the modification variable
 * 
 */
double *MenuItemValue::getPtrDouble() const
{
    return this->_ptrDouble;
}

/**
 * @brief returns the amount of decimal digits
 * 
 */
uint8_t MenuItemValue::getDecimals() const
{
    return this->_decimals;
}

/**
 * @brief returns the minimum value
 * 
 */
double MenuItemValue::getMin() const
{
    return this->_min;
}

/**
 * @brief returns the maximum value
 * 
 */
double MenuItemValue::getMax() const
{
    return this->_max;
}
