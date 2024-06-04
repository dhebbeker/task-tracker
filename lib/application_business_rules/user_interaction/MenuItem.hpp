#pragma once
#include <cstdint>
#include <string>
#include <vector>

/**
 * @brief Enumeration to distinguish the type of menu Items
 * 
 */
enum class MenuItemType
{
    SUBMENU,
    SWITCH,
    VALUE,
};

/**
 * @brief interface for all menu items
 * 
 */
class IMenuItem
{
  public:
    IMenuItem() = default;
    virtual ~IMenuItem() = default;

    virtual std::string getText() const = 0;
    virtual MenuItemType getType() const = 0;
};

/**
 * @brief type definition for a list of menu items
 * @note this is used for definition of menus
 * 
 */
typedef std::vector<const IMenuItem *> MenuItemList;

/**
 * @brief menu item to call a submenu
 * 
 */
class MenuItemSubmenu final : public IMenuItem
{
  public:
    MenuItemSubmenu(std::string text, const MenuItemList *subMenuList);
    ~MenuItemSubmenu() override = default;

    std::string getText() const override;
    inline MenuItemType getType() const override
    {
        return MenuItemType::SUBMENU;
    };

    const MenuItemList *getSubMenuList() const;

  protected:
    const std::string _text;
    const MenuItemList *_subMenuList;
};

/**
 * @brief menu item to show and change boolean variables
 * 
 */
struct MenuItemSwitch final : public IMenuItem
{
  public:
    MenuItemSwitch(std::string text, bool *ptrBool);
    ~MenuItemSwitch() override = default;

    std::string getText() const override;
    inline MenuItemType getType() const override
    {
        return MenuItemType::SWITCH;
    };

    bool *getPtrBool() const;

  protected:
    const std::string _text;
    bool *_ptrBool;
};

/**
 * @brief menu item to show double variables and call a modification screen
 * 
 */
struct MenuItemValue final : public IMenuItem
{
  public:
    MenuItemValue(std::string text, double *ptrDouble, std::uint8_t decimals, double min, double max);
    ~MenuItemValue() override = default;

    std::string getText() const override;
    inline MenuItemType getType() const override
    {
        return MenuItemType::VALUE;
    };

    double *getPtrDouble() const;
    std::uint8_t getDecimals() const;
    double getMin() const;
    double getMax() const;

  protected:
    const std::string _text;
    double *_ptrDouble;
    std::uint8_t _decimals;
    double _min;
    double _max;
};
