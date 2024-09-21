#include "ProcessHmiInputs.hpp"
#include "IKeypad.hpp"
#include "IPresenter.hpp"
#include "board_interface.hpp"
#include <functional>
#include <serial_interface/serial_port.hpp>
#include <stdexcept>
#include <tasks/Task.hpp>
#include <type_traits.hpp>

static Task *mapTaskSelectionToTask(const KeyId taskSelection)
{
    // TODO lookup which task selection belongs to which task object
    const std::size_t taskIndex = to_underlying(taskSelection) - to_underlying(KeyId::TASK1);
    if (taskIndex < device::tasks.size())
    {
        return &(std::next(std::begin(device::tasks), taskIndex)->second);
    }
    else
    {
        return nullptr;
    }
}

static TaskIndex mapTaskToStatusIndicator(const KeyId selection)
{
    switch (selection)
    {
    case KeyId::TASK1:
        return 0;
        break;
    case KeyId::TASK2:
        return 1;
        break;
    case KeyId::TASK3:
        return 2;
        break;
    case KeyId::TASK4:
        return 3;
        break;
    default:
        throw std::runtime_error("no status indicator for this task available");
        break;
    }
}

void ProcessHmiInputs::handleHmiSelection(const KeyId selection)
{
    serial_port::cout << "Handle event nr. " << to_underlying(selection) << std::endl;
    switch (selection)
    {
    case KeyId::TASK1:
    case KeyId::TASK2:
    case KeyId::TASK3:
    case KeyId::TASK4: {
        Task *const optTask = mapTaskSelectionToTask(selection);
        if (optTask)
        {
            Task &task = *optTask;
            if (task.isRunning())
            {
                task.stop();
            }
            else
            {
                task.start();
            }
            stateVisualizer.setTaskStatusIndicator(
                mapTaskToStatusIndicator(selection),
                task.isRunning() ? TaskIndicatorState::ACTIVE : TaskIndicatorState::INACTIVE);
        }
        else
        {
            serial_port::cout << "No task assigned to selection " << to_underlying(selection) << std::endl;
        }
        break;
    }
    default:
        break;
    }
}

template <class CONTAINER>
static void initializeTasks(CONTAINER &tasks)
{
    // TODO get tasks from storage

    // workaround for missing storage: initialize dummy tasks
    device::tasks.emplace(31, "Task 1");
    device::tasks.emplace(32, "Task 2");
    device::tasks.emplace(33, "Task 3");
    device::tasks.emplace(34, "Task 4");
}

ProcessHmiInputs::ProcessHmiInputs(IPresenter &stateVisualizer, IKeypad &keypad)
    : stateVisualizer(stateVisualizer)
{
    using namespace std::placeholders;
    keypad.setCallback(std::bind(&ProcessHmiInputs::handleHmiSelection, this, _1));
    initializeTasks(device::tasks);
}
