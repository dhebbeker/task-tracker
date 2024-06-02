#include "Menu.hpp"
#include "MenuItem.hpp"

Menu::Menu(IDisplay &displayToUse)
    : display(displayToUse)
{
    static auto mainMenu = MenuItemList();
    static auto subMenu1 = MenuItemList();
    static auto subMenu2 = MenuItemList();
    static auto subMenu3 = MenuItemList();

    static auto ListButton1 = MenuItemSubmenu{"ListButton1 Text", &subMenu1};
    static auto ListValue = MenuItemValue("ListValue", NULL, 2, 0.9, 1001.4);
    static auto ListButton2 = MenuItemSubmenu{"ListButton2 Text", &subMenu2};
    static auto ListButton3 = MenuItemSubmenu{"ListButton3 Text", &subMenu3};
    static auto ListSwitch1 = MenuItemSwitch{"ListSwitch1 Text", NULL};
    static auto ListSwitch2 = MenuItemSwitch{"ListSwitch2 Text", NULL};
    mainMenu.push_back(&ListButton1);
    mainMenu.push_back(&ListValue);
    mainMenu.push_back(&ListButton2);
    mainMenu.push_back(&ListSwitch1);
    mainMenu.push_back(&ListSwitch2);
    mainMenu.push_back(&ListButton3);

    static auto Sub1Button1 = MenuItemSubmenu{"Sub1 Button1", &subMenu1};
    static auto Sub1Button2 = MenuItemSubmenu{"Sub1 Button2", &subMenu2};
    subMenu1.push_back(&Sub1Button1);
    subMenu1.push_back(&Sub1Button2);

    static auto Sub2Button1 = MenuItemSubmenu{"Sub2 Button1", &subMenu1};
    static auto Sub2Button2 = MenuItemSubmenu{"Sub2 Button2", &subMenu2};
    subMenu2.push_back(&Sub2Button1);
    subMenu2.push_back(&Sub2Button2);

    static auto Sub3Button1 = MenuItemSubmenu{"Sub3 Button1", &subMenu1};
    static auto Sub3Button2 = MenuItemSubmenu{"Sub3 Button2", &subMenu2};
    subMenu3.push_back(&Sub3Button1);
    subMenu3.push_back(&Sub3Button2);

    display.drawMenu(&mainMenu);
}

void Menu::loop()
{
    display.refresh();
}
