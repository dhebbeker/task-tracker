#include <Arduino.h>
#include <cassert>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <input_device_interface/debouncedIsr.hpp>

int debouncer::minPriorityToInt()
{
    return 0; // by FreeRTOS specification
}

int debouncer::maxPriorityToInt()
{
    return configMAX_PRIORITIES - 1;
}

int debouncer::defaultPriorityToInt()
{
    return (maxPriorityToInt() - minPriorityToInt()) / 2;
}

/**
 * Non-memorizing startup delay for debouncing.
 *
 * Based of FreeRTOS task notifications.
 *
 * Using a startup delay has the advantage, that short pulses (shorter than the debounce period) will have no effect.
 * In contrast classical debouncing trough ignoring changes for the debounce period, after an initial change, may
 * miss the second state change of a short pulse.
 *
 * Operation:
 *
 * - if the interrupt function (typically called by an interrupt), provided by \ref getInterruptFunction()
 *   is called, a timer for the startup delay will start
 * - if the interrupt function is called again the timer will restart
 * - when the timer expires, the handler will be called
 *
 * \see \ref debouncing_approaches
 */
class Debouncer
{
  public:
    /**
     * Setup and start an (initially idle) startup delay task.
     *
     * @tparam Rep Rep, an arithmetic type representing the number of ticks
     * @tparam Period a std::ratio representing the tick period (i.e. the number of seconds per tick)
     * @param handler is a function to be called when the timer expires
     * @param debounceTime is the startup delay
     * @param priority is the priority in which the delay task and therefore the handler shall be called
     */
    template <class Rep, class Period>
    Debouncer(
        std::function<void(void)> handler,
        std::chrono::duration<Rep, Period> debounceTime,
        int priority = debouncer::defaultPriorityToInt())
        : handler(handler),
          debounceTaskHandle(nullptr),
          startupDelay(pdMS_TO_TICKS(std::chrono::duration_cast<std::chrono::milliseconds>(debounceTime).count()))
    {
        xTaskCreate(pinDebounce, "pinDebounce", stackSize, this, priority, &debounceTaskHandle);
    }

    /**
     * Indicates minimum free memory on the task specific stack.
     *
     * This can be used to estimate the necessary stack size for the task.
     *
     * @return number of minimum free words on the task
     */
    UBaseType_t getTaskStackHighWaterMark() const
    {
        return uxTaskGetStackHighWaterMark(debounceTaskHandle);
    }

    /**
     * Provides the function to be used to indicate state changes to be debounced.
     *
     * @returns a function which must be called when the observed state changes
     */
    std::function<void(void)> getInterruptFunction() const
    {
        return std::bind(&Debouncer::interruptServiceRoutine, this);
    }

  private:
    std::function<void(void)> handler;
    mutable TaskHandle_t debounceTaskHandle;
    TickType_t startupDelay;

    /**
     * The necessary stack size in words.
     *
     * Has been determined by measuring the stack high water mark and by experimenting.
     */
    static constexpr configSTACK_DEPTH_TYPE stackSize = configMINIMAL_STACK_SIZE + 708 + 1'000;

    /**
     * Triggers the startup delay.
     *
     * This is designed such that it may be called from an interrupt.
     * It is non-blocking and as short as possible
     */
    void ARDUINO_ISR_ATTR interruptServiceRoutine() const
    {
        BaseType_t higherPriorityTaskWoken = pdFALSE;
        vTaskNotifyGiveFromISR(debounceTaskHandle, &higherPriorityTaskWoken);
        portYIELD_FROM_ISR(higherPriorityTaskWoken);
    }

    /**
     * Performs the actual startup delay.
     *
     * \internal
     * Function must be `static` in order to be able to provide a raw function pointer.
     * In order to access the debouncer's non-static members, a pointer to the debouncer object
     * must be provided.
     * \endinternal
     *
     * @param parameter must be a pointer to the debouncer object used for debouncing
     */
    static void pinDebounce(void *const parameter)
    {
        assert(parameter);
        const Debouncer &debouncer = *reinterpret_cast<const Debouncer *>(parameter);
        while (true)
        {
            // prepare for incoming notification
            xTaskNotifyStateClear(nullptr);

            // first stage: wait for start
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

            // second stage: wait for uninterrupted period
            while (ulTaskNotifyTake(pdTRUE, debouncer.startupDelay) > 0)
            {
                continue; // has been interrupted, wait again
            }

            debouncer.handler();
        }
    }
};

std::function<void(void)> createDebouncer(
    const std::function<void(void)> handler,
    const std::chrono::milliseconds debounceTime,
    const int priority)
{
    const auto *const debouncer = new Debouncer(handler, debounceTime, priority);
    return debouncer->getInterruptFunction();
}
