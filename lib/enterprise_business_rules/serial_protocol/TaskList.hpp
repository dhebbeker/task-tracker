#pragma once
#include "TaskObject.hpp"
#include <vector>

namespace task_tracker_systems
{

/**
 * list of tasks
 */
struct TaskList
{
    /**
     * list
     */
    std::vector<TaskObject> list;
};
} // namespace task_tracker_systems
