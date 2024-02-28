#include "Worker.hpp"

void Worker::waitUntilFinished() const
{
    if (thread.joinable())
    {
        thread.join();
    }
}

bool Worker::isRunning() const
{
    std::lock_guard lock(stateMutex);
    return running;
}

Worker::Worker(std::function<void(void)> &&work)
    : Worker(std::move(work), std::chrono::milliseconds(0))
{
}

void Worker::cancelStartupDelay()
{
    std::lock_guard lock(stateMutex);
    running = false;
    stopCondition.notify_all();
}
