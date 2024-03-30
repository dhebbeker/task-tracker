#include "Keypad.hpp"
#include <Arduino-wrapper.h>
#include <array>
#include <board_pins.hpp>
#include <chrono>
#include <cstddef>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <functional>
#include <thread>
#include <type_traits>
#include <utility>

using namespace std::chrono_literals;

static HmiHandler callBack;

template <board::PinType PIN>
class DebouncedPinIsr
{
  public:
    DebouncedPinIsr(std::function<void(void)> handler, const std::chrono::milliseconds startupDelay = 200ms, const UBaseType_t priority = configMAX_PRIORITIES / 2)
    {
        DebouncedPinIsr::handler = handler;
        DebouncedPinIsr::debounceTaskHandle = nullptr;
        DebouncedPinIsr::startupDelay = pdMS_TO_TICKS(startupDelay.count());
        xTaskCreate(debounceTask, "pin debounce task", configMINIMAL_STACK_SIZE + (1 << 12), nullptr, priority, &debounceTaskHandle);
    }

    void (*getInterruptFunction() const)()
    {
        return interruptSubroutine;
    }

  private:
    static std::function<void(void)> handler;
    static TaskHandle_t debounceTaskHandle;
    static TickType_t startupDelay;

    static void IRAM_ATTR interruptSubroutine()
    {
        BaseType_t higherPriorityTaskWoken = pdFALSE;
        vTaskNotifyGiveFromISR(debounceTaskHandle, &higherPriorityTaskWoken);
        portYIELD_FROM_ISR(higherPriorityTaskWoken);
    }

    static void debounceTask(void *)
    {
        do
        {
            // prepare for incoming notification
            xTaskNotifyStateClear(nullptr);

            // first stage: wait for start
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

            // second stage: wait for uninterrupted period
            while (ulTaskNotifyTake(pdTRUE, startupDelay) > 0)
            {
                continue; // has been interrupted, wait again
            }

            handler();
        } while (true);
    }
};

template <board::PinType PIN>
std::function<void(void)> DebouncedPinIsr<PIN>::handler;
template <board::PinType PIN>
TaskHandle_t DebouncedPinIsr<PIN>::debounceTaskHandle;
template <board::PinType PIN>
TickType_t DebouncedPinIsr<PIN>::startupDelay;

/**
 * Generator for ISR function pointers.
 *
 * Uses an array to instantiate function templates and assembles an array of function pointers to those instances.
 * This is used to attach a value to the ISRs which can not accept any argument.
 * Instead the value is provided as non-type template argument to the ISR function template.
 *
 * @tparam T type of the array elements
 * @tparam N number of the array elements
 */
template <class T, std::size_t N>
struct FunctionPointerGenerator
{
    /**
     * Creates an array of function pointers to interrupt functions.
     *
     * Passing the values as template argument is necessary as they will be evaluated at compile time
     * for instantiating the function templates.
     *
     * @tparam VALUES array containing the values used for ISR function template instantiation
     * @returns an array with the same number of function pointers as the number of elements of the input values array
     */
    template <T (&VALUES)[N]>
    static auto createIsrPointers()
    {
        return createIsrPointers<VALUES>(std::make_index_sequence<N>());
    }

    /**
     * \copydoc createIsrPointers()
     * @tparam Is is a sequence of the indices to be used to access the array elements
     * @param indices is an object to derive `Is`
     */
    template <T (&VALUES)[N], std::size_t... Is>
    static std::array<void (*)(), N> createIsrPointers([[maybe_unused]] const std::index_sequence<Is...> indices)
    {
        return {DebouncedPinIsr<VALUES[Is].first>(std::bind(callBack, VALUES[Is].second)).getInterruptFunction()...};
    }
};

/**
 * Creates generator for function pointers.
 *
 * This function automatically deduces the template arguments for the generator.
 *
 * @param array of the same type and size to be used with the generator.
 *        Argument is used for template argument deduction only.
 * @tparam T the type of the array elements
 * @tparam N the number of array elements
 * @return generator which can be used to generate function pointers
 */
template <class T, std::size_t N>
static constexpr FunctionPointerGenerator<T, N> createFPG([[maybe_unused]] T (&array)[N])
{
    return FunctionPointerGenerator<T, N>();
}

/**
 * Maps HMI buttons to events.
 */
static constexpr std::pair<board::PinType, KeyId> selectionForPins[] = {
    {board::button::pin::task1, KeyId::TASK1},
    {board::button::pin::task2, KeyId::TASK2},
    {board::button::pin::task3, KeyId::TASK3},
    {board::button::pin::task4, KeyId::TASK4},
    {board::button::pin::up, KeyId::LEFT},
    {board::button::pin::down, KeyId::RIGHT},
    {board::button::pin::enter, KeyId::ENTER},
    {board::button::pin::back, KeyId::BACK},
};

Keypad::Keypad()
{
    // input pins
    const auto functionPointers = createFPG(selectionForPins).createIsrPointers<selectionForPins>();
    std::size_t index = 0;
    for (const auto selectionForPin : selectionForPins)
    {
        pinMode(selectionForPin.first, INPUT_PULLUP);
        attachInterrupt(
            digitalPinToInterrupt(selectionForPin.first),
            functionPointers.at(index),
            FALLING);
        index++;
    }
}

void Keypad::setCallback(std::function<void(KeyId)> callbackFunction)
{
    callBack = callbackFunction;
}

bool Keypad::isKeyPressed(const KeyId keyInquiry)
{
    return false; //TODO
}