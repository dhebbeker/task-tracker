#pragma once

#include <cstddef>

enum class TaskIndicatorState
{
    ACTIVE,
    INACTIVE,
};

using TaskIndex = std::size_t;

/**
 * Responsible for advertising information and alerts to a human user.
 *
 * This class decides what technical means shall be used to provide logical information.
 * For example if a status is shown using a display or an LED.
 * Also it may use other than visual means to signal events.
 * For example sound or vibration.
 *
 * Designated users of this class are use case interactors or other forms of use case implementations.
 * This class may depend on devices or device interfaces which in turn will be realized by hardware (or board assembly).
 */
class IPresenter
{
  public:
    virtual void setTaskStatusIndicator(const TaskIndex, const TaskIndicatorState) = 0;
};
