#include "Task.hpp"
#include <type_traits>

const Task::String &Task::getLabel() const
{
    return label;
}

Task::Task(const String &newLabel, const Duration elapsedTime)
    : recordedDuration(elapsedTime), label(newLabel), state(State::IDLE)
{
}

void Task::start()
{
    timestampStart = std::chrono::round<DurationFraction>(Clock::now());
    state = State::RUNNING;
}

void Task::stop()
{
    // this check is necessary, as else the timestamp using for comparison will be invalid
    if (state == State::RUNNING)
    {
        recordedDuration += std::chrono::duration_cast<DurationFraction>(Clock::now() - timestampStart);
        state = State::IDLE;
    }
}

bool Task::isRunning() const
{
    return state == State::RUNNING;
}

void Task::setLabel(const String &label)
{
    this->label = label;
}

Task::Duration Task::getRecordedDuration()
{
    if (isRunning())
    {
        stop();
        start();
    }
    return std::chrono::round<Duration>(recordedDuration);
}

device::TaskCollection device::tasks;

void Task::setRecordedDuration(Duration newDuration)
{
    recordedDuration = newDuration;
}

Task::Duration Task::getLastRecordedDuration() const
{
    return std::chrono::round<Duration>(recordedDuration);
}
