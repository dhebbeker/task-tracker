#pragma once

#include <chrono>
#include <functional>

namespace debounce_task_priority
{
int minToInt();
int maxToInt();
int defaultToInt();
} // namespace debounce_task_priority

std::function<void(void)> createDebouncingIsr(
    std::function<void(void)> handler,
    std::chrono::milliseconds debounceTime,
    int priority = debounce_task_priority::defaultToInt());
