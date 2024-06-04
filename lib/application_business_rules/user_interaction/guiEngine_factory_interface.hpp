/**
 * \file .
 * Dependency injection for the guiEngine.
 *
 * \see \ref dependency_injection
 */

#pragma once

#include "IGuiEngine.hpp"

namespace board
{
IGuiEngine &getGuiEngine();
}
