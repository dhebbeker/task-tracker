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

    /* define menu items for the main menu */
    static auto ListValue = MenuItemTask();
    /* add menu items to main menu list */
    mainMenu.push_back(&ListValue);

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
