#include <Arduino-wrapper.h>
#include <algorithm>
#include <board_pins.hpp>
#include <board_types.hpp>
#include <chrono>
#include <cstdint>
#include <input_device_interface/debouncedIsr.hpp>
#include <iomanip>
#include <ios>
#include <iostream>
#include <iterator>
#include <map>
#include <serial_interface/serial_port.hpp>
#include <tasks/Task.hpp>
#include <thread>
#include <unity.h>
#include <user_interaction/IKeypad.hpp>
#include <user_interaction/Menu.hpp>
#include <user_interaction/Presenter.hpp>
#include <user_interaction/ProcessHmiInputs.hpp>
#include <user_interaction/display_factory_interface.hpp>
#include <user_interaction/keypad_factory_interface.hpp>
#include <user_interaction/statusindicators_factory_interface.hpp>

using namespace std::chrono_literals;
using namespace fakeit;

// mocks

namespace board
{
IDisplay &getDisplay()
{
    Mock<IDisplay> fakeDisplay;
    return fakeDisplay.get();
}
} // namespace board

namespace serial_port
{
std::basic_ostream<CharType> &cout = std::cout;
}

static std::map<board::PinType, std::function<void(void)>> isr_collection;

void setUp()
{
    isr_collection.clear();
}

void tearDown()
{
}

// test dummy for Arduino-ESP32 specific function
void attachInterrupt(const uint8_t interruptNum, const std::function<void(void)> userFunc, const int mode)
{
    isr_collection.emplace(interruptNum, userFunc);
    std::cout << "Added ISR for interrupt number " << static_cast<unsigned int>(interruptNum)
              << " function pointer " << std::showbase << std::hex << reinterpret_cast<std::intptr_t>(userFunc.target<void (*)(void)>()) << std::resetiosflags(std::ios::showbase | std::ios::basefield) << std::endl;
}

// test dummy for a FreeRTOS adapter function
std::function<void(void)> createDebouncingIsr(const std::function<void(void)> handler, std::chrono::milliseconds, int)
{
    return handler;
}

// test dummy for a FreeRTOS adapter function
int debounce_task_priority::defaultToInt()
{
    return 0xC0FFE;
}

void test_Controller()
{
    // irrelevant test doubles
    Fake(Method(ArduinoFake(), pinMode));
    Fake(Method(ArduinoFake(), analogWrite));
    Fake(Method(ArduinoFake(), tone));

    // get collaborator objects
    Menu singleMenu(board::getDisplay());
    Presenter presenter(singleMenu, board::getStatusIndicators());
    ProcessHmiInputs processor(presenter, board::getKeypad());
    auto &task1 = std::begin(device::tasks)->second;                      // we are going to test for task 1
    const auto isrTask1 = isr_collection.find(board::button::pin::task1); // ISR we expect for task 1
    TEST_ASSERT_NOT_EQUAL(std::end(isr_collection), isrTask1);            // assert we found an ISR for task 1 in the list

    std::cout << "trigger ISR for task 1: 'start task'" << std::endl;
    When(Method(ArduinoFake(), digitalRead).Using(board::button::pin::task1)).Return(LOW); // set task 1 button to low
    isrTask1->second();                                                                    // trigger interrupt for task 1 button

    // wait for the task to be running
    while (!task1.isRunning())
    {
        // TODO it would be better to explicitly check for the "stop" task to be finished
        std::this_thread::yield(); // give the task handler time to finish before the test interferes
    }
    constexpr int millisecondsToWait = 1000;
    std::this_thread::sleep_for(std::chrono::milliseconds(millisecondsToWait)); // wait a defined time

    std::cout << "trigger ISR for task 1: 'stop task'" << std::endl;
    When(Method(ArduinoFake(), digitalRead).Using(board::button::pin::task1)).Return(LOW); // set task 1 button to low
    isrTask1->second();                                                                    // stop task

    // wait for the task to be stopped
    while (task1.isRunning())
    {
        // TODO it would be better to explicitly check for the "start" task to be finished
        std::this_thread::yield(); // give the task handler time to finish before the test interferes
    }
    // assert results
    const auto millisecondsMeasured = std::chrono::duration_cast<std::chrono::milliseconds>(task1.getRecordedDuration());
    TEST_ASSERT_INT_WITHIN(10, millisecondsToWait, millisecondsMeasured.count());
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_Controller);

    UNITY_END();
}
