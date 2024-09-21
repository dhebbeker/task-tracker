#pragma once

#include <chrono>
#include <functional>

namespace debouncer
{
/**
 * \defgroup debouncer_priority Debouncer task priority
 * @{
 */
/**
 * \returns the value for the minimum priority of a debounce task
 */
int minPriorityToInt();

/**
 * \returns the value for the maximum priority of a debounce task
 */
int maxPriorityToInt();

/**
 * \returns the value for the default priority of a debounce task
 */
int defaultPriorityToInt();
/** @}*/
} // namespace debouncer

/**
 * Creates a Debouncer.
 *
 * The Debouncer takes a function ("handler") which will be called after a signal has been debounced.
 * The Debouncer provides a function which shall be called by the user to indicate the signal.
 *
 * The Debouncer acts as a non-memorizing startup delay.
 * A signal will only trigger the handler in case the debounce time expires while no further signal is recorded.
 *
 * The Debouncer will use some kind of thread or task to wait for the debounce time.
 *
 * For each signal a separate Debouncer needs to be created.
 *
 * \param handler is the function to be called by the Debouncer
 * \param debounceTime is the time which shall be waited by the Debouncer to recognize a stable signal
 * \param priority is the priority of the debounce task used for that debouncer
 * \returns a function which must be called by the user to register new signals
 */
std::function<void(void)> createDebouncer(
    std::function<void(void)> handler,
    std::chrono::milliseconds debounceTime,
    int priority = debouncer::defaultPriorityToInt());
