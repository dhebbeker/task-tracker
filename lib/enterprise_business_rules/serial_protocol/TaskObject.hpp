#pragma once
#include <chrono>
#include <string>

namespace task_tracker_systems
{

/**
 * task object
 */
struct TaskObject
{
    /**
     * unique identifier
     */
    unsigned int id;
    /**
     * name or summary; ASCII only, no line breaks
     */
    std::string label;
    /**
     * duration in seconds
     */
    std::chrono::seconds::rep duration;
};
} // namespace task_tracker_systems
