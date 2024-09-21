#include "Menu.hpp"
#include "MenuItem.hpp"

/**
 * @brief Construct a new Menu:: Menu object
 * 
 * @param guiEngineToUse - GuiEngine to be used for display
 * @param keypad         - Keypad to get Menu controls from
 */
Menu::Menu(IGuiEngine &guiEngineToUse, IKeypad &keypad)
    : guiEngine(guiEngineToUse)
{
    /* register the Keypad to the GuiEngine for navigation */
    guiEngine.registerKeyPad(&keypad);

    /* initialize some lists for several menus */
    static auto mainMenu = MenuItemList();
    static auto subMenu1 = MenuItemList();
    static auto subMenu2 = MenuItemList();
    static auto subMenu3 = MenuItemList();

    /* define menu items for the main menu */
    static auto ListButton1 = MenuItemSubmenu{"ListButton1 Text", &subMenu1};
    static auto ListValue = MenuItemValue("ListValue", NULL, 2, 0.9, 1001.4);
    static auto ListButton2 = MenuItemSubmenu{"ListButton2 Text", &subMenu2};
    static auto ListButton3 = MenuItemSubmenu{"ListButton3 Text", &subMenu3};
    static auto ListSwitch1 = MenuItemSwitch{"ListSwitch1 Text", NULL};
    static auto ListSwitch2 = MenuItemSwitch{"ListSwitch2 Text", NULL};
    /* add menu items to main menu list */
    mainMenu.push_back(&ListButton1);
    mainMenu.push_back(&ListValue);
    mainMenu.push_back(&ListButton2);
    mainMenu.push_back(&ListSwitch1);
    mainMenu.push_back(&ListSwitch2);
    mainMenu.push_back(&ListButton3);

    /* define menu items for sub menu 1 */
    static auto Sub1Button1 = MenuItemSubmenu{"Sub1 Button1", &subMenu1};
    static auto Sub1Button2 = MenuItemSubmenu{"Sub1 Button2", &subMenu2};
    /* add menu items sub menu 1 list */
    subMenu1.push_back(&Sub1Button1);
    subMenu1.push_back(&Sub1Button2);

    /* define menu items for sub menu 2 */
    static auto Sub2Button1 = MenuItemSubmenu{"Sub2 Button1", &subMenu1};
    static auto Sub2Button2 = MenuItemSubmenu{"Sub2 Button2", &subMenu2};
    /* add menu items sub menu 2 list */
    subMenu2.push_back(&Sub2Button1);
    subMenu2.push_back(&Sub2Button2);

    /* define menu items for sub menu 3 */
    static auto Sub3Button1 = MenuItemSubmenu{"Sub3 Button1", &subMenu1};
    static auto Sub3Button2 = MenuItemSubmenu{"Sub3 Button2", &subMenu2};
    /* add menu items sub menu 3 list */
    subMenu3.push_back(&Sub3Button1);
    subMenu3.push_back(&Sub3Button2);

    /* draw the main menu with GuiEngine */
    guiEngine.drawMenu(&mainMenu);
}

/**
 * @brief cyclic refresh function
 * 
 */
void Menu::loop()
{
    guiEngine.refresh();
}
