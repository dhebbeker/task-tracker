#include <input_device_interface/Keypad.hpp>
#include <unity.h>

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
    UNITY_BEGIN();

    RUN_TEST(test_default_inactive);

    return UNITY_END();
}
