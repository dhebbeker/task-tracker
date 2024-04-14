#include <Arduino-wrapper.h>
#include <input_device_interface/Keypad.hpp>
#include <unity.h>

using namespace fakeit;

void setUp(void)
{
}

void tearDown(void)
{
}

void test_default_inactive()
{
    Keypad keypad;
    TEST_ASSERT_FALSE(keypad.isKeyPressed(KeyId::TASK1));
}

int main()
{
    // irrelevant test doubles
    Fake(Method(ArduinoFake(), pinMode));

    UNITY_BEGIN();

    RUN_TEST(test_default_inactive);

    return UNITY_END();
}
