#include <Arduino.h>
#include <cassert>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <input_device_interface/debouncedIsr.hpp>

int debounce_task_priority::minToInt()
{
    return 0; // by FreeRTOS specification
}

int debounce_task_priority::maxToInt()
{
    return configMAX_PRIORITIES - 1;
}

int debounce_task_priority::defaultToInt()
{
    return (maxToInt() - minToInt()) / 2;
}

class Debouncer
{
  public:
    template <class Rep, class Period>
    Debouncer(
        std::function<void(void)> handler,
        std::chrono::duration<Rep, Period> debounceTime,
        int priority = debounce_task_priority::defaultToInt())
        : handler(handler),
          debounceTaskHandle(nullptr),
          startupDelay(pdMS_TO_TICKS(std::chrono::duration_cast<std::chrono::milliseconds>(debounceTime).count()))
    {
        xTaskCreate(pinDebounce, "pinDebounce", stackSize, this, priority, &debounceTaskHandle);
    }

    UBaseType_t getTaskStackHighWaterMark() const
    {
        return uxTaskGetStackHighWaterMark(debounceTaskHandle);
    }

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
    static constexpr configSTACK_DEPTH_TYPE stackSize = configMINIMAL_STACK_SIZE + 580;

    void ARDUINO_ISR_ATTR interruptServiceRoutine() const
    {
        BaseType_t higherPriorityTaskWoken = pdFALSE;
        vTaskNotifyGiveFromISR(debounceTaskHandle, &higherPriorityTaskWoken);
        portYIELD_FROM_ISR(higherPriorityTaskWoken);
    }

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

std::function<void(void)> createDebouncingIsr(
    const std::function<void(void)> handler,
    const std::chrono::milliseconds debounceTime,
    const int priority)
{
    const auto *const debouncer = new Debouncer(handler, debounceTime, priority);
    return debouncer->getInterruptFunction();
}
