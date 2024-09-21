#include "../test_dummies.hpp"
#include <Arduino-wrapper.h>
#include <board_pins.hpp>
#include <input_device_interface/Keypad.hpp>
#include <iostream>
#include <unity.h>

using namespace fakeit;

static void dummyCallback(const KeyId id)
{
    std::cout << "dummy callback has been called with key " << id << std::endl;
}

void setUp(void)
{
    isr_collection.clear();
}

void tearDown(void)
{
}

void test_default_inactive()
{
    Keypad keypad;
    TEST_ASSERT_FALSE(keypad.isKeyPressed(KeyId::TASK1));
}

void test_switch_on_off()
{
    Keypad keypad;
    keypad.setCallback(dummyCallback);

    // set & test "active"
    When(Method(ArduinoFake(), digitalRead).Using(board::button::pin::task1)).Return(LOW); // set task 1 button to low
    changeButtonState(board::button::pin::task1);                                          // stop task
    TEST_ASSERT_TRUE(keypad.isKeyPressed(KeyId::TASK1));

    // set & test "inactive"
    When(Method(ArduinoFake(), digitalRead).Using(board::button::pin::task1)).Return(HIGH); // set task 1 button to high
    changeButtonState(board::button::pin::task1);                                           // stop task
    TEST_ASSERT_FALSE(keypad.isKeyPressed(KeyId::TASK1));
}

int main()
{
    // irrelevant test doubles
    Fake(Method(ArduinoFake(), pinMode));

    UNITY_BEGIN();

    RUN_TEST(test_default_inactive);
    RUN_TEST(test_switch_on_off);

    return UNITY_END();
}
