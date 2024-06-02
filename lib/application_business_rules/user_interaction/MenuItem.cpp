#include "MenuItem.hpp"

MenuItemSubmenu::MenuItemSubmenu(std::string text, const MenuItemList *subMenuList)
    : _text{text}, _subMenuList{subMenuList}
{
}

std::string MenuItemSubmenu::getText() const
{
    return this->_text;
}

const MenuItemList *MenuItemSubmenu::getSubMenuList() const
{
    return this->_subMenuList;
}

MenuItemSwitch::MenuItemSwitch(std::string text, bool *ptrBool)
    : _text{text}, _ptrBool{ptrBool}
{
}

std::string MenuItemSwitch::getText() const
{
    return this->_text;
}

bool *MenuItemSwitch::getPtrBool() const
{
    return this->_ptrBool;
}

MenuItemValue::MenuItemValue(std::string text, double *ptrDouble, uint8_t decimals, double min, double max)
    : _text{text}, _ptrDouble{ptrDouble}, _decimals{decimals}, _min{min}, _max{max}
{
}

std::string MenuItemValue::getText() const
{
    return this->_text;
}

double *MenuItemValue::getPtrDouble() const
{
    return this->_ptrDouble;
}

uint8_t MenuItemValue::getDecimals() const
{
    return this->_decimals;
}

double MenuItemValue::getMin() const
{
    return this->_min;
}

double MenuItemValue::getMax() const
{
    return this->_max;
}
