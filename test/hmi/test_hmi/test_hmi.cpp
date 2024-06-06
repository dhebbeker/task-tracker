#include "../test_dummies.hpp"
#include <Arduino-wrapper.h>
#include <algorithm>
#include <board_pins.hpp>
#include <chrono>
#include <cstdint>
#include <input_device_interface/debouncedIsr.hpp>
#include <ios>
#include <iterator>
#include <serial_interface/serial_port.hpp>
#include <tasks/Task.hpp>
#include <thread>
#include <unity.h>
#include <user_interaction/IKeypad.hpp>
#include <user_interaction/IPresenter.hpp>
#include <user_interaction/ProcessHmiInputs.hpp>
#include <user_interaction/keypad_factory_interface.hpp>

using namespace std::chrono_literals;
using namespace fakeit;

// mocks

IPresenter &getFakePresenter()
{
    static Mock<IPresenter> fakePresenter;
    When(Method(fakePresenter, setTaskStatusIndicator)).AlwaysReturn();
    return fakePresenter.get();
}

namespace serial_port
{
std::basic_ostream<CharType> &cout = std::cout;
}

void setUp()
{
    isr_collection.clear();
}

void tearDown()
{
}

void test_Controller()
{
    // irrelevant test doubles
    Fake(Method(ArduinoFake(), pinMode));
    Fake(Method(ArduinoFake(), analogWrite));
    Fake(Method(ArduinoFake(), tone));

    // get collaborator objects
    ProcessHmiInputs processor(getFakePresenter(), board::getKeypad());
    auto &task1 = std::begin(device::tasks)->second; // we are going to test for task 1

    When(Method(ArduinoFake(), digitalRead).Using(board::button::pin::task1)).Return(LOW); // set task 1 button to low
    changeButtonState(board::button::pin::task1);

    // wait for the task to be running
    while (!task1.isRunning())
    {
        // TODO it would be better to explicitly check for the "stop" task to be finished
        std::this_thread::yield(); // give the task handler time to finish before the test interferes
    }
    constexpr int millisecondsToWait = 1000;
    std::this_thread::sleep_for(std::chrono::milliseconds(millisecondsToWait)); // wait a defined time

    When(Method(ArduinoFake(), digitalRead).Using(board::button::pin::task1)).Return(LOW); // set task 1 button to low
    changeButtonState(board::button::pin::task1);                                          // stop task

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
