#include "GuiEngine.hpp"
#include <Arduino.h>
#include <board_pins.hpp>
#include <user_interaction/guiEngine_factory_interface.hpp>

namespace board
{
IGuiEngine &getGuiEngine()
{
    Wire.begin(board::i2c_1::pin::sda, board::i2c_1::pin::scl);
    constexpr GuiEngine::Configuration configuration = {
        .screen_width = 128,
        .screen_height = 64,
        .generateDisplayVoltageInternally = true,
        .display_i2c_address = 0x3D,
    };
    static GuiEngine singleton(configuration, Wire);
    return singleton;
}
} // namespace board
