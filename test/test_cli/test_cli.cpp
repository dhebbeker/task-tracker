#include <command_line_interpreter.hpp>
#include <cstddef>
#include <fakeit.hpp>
#include <iostream>
#include <unity.h>

using namespace fakeit;

void setUp()
{
}

void tearDown()
{
}

static struct FooData
{
    std::size_t timesCalled;
} fooData;

static void foo()
{
    fooData.timesCalled++;
}

static void two(const std::string s, const int n)
{
    std::cout << "Two: " << n << " " << s << std::endl;
}

void test_command_argVoid()
{
    const auto myCommand = makeCommand("operate", std::make_tuple(), std::function(foo));
    myCommand.parse("operate");

    TEST_ASSERT_EQUAL_UINT(1, fooData.timesCalled);
}

static struct BarData
{
    std::size_t timesCalled;
    int n;
} barData;

static void bar(const int n)
{
    barData.timesCalled++;
    barData.n = n;
}

void test_command_argInt()
{
    const Argument<int> barArg = {.labels = {"drinks"}, -42};
    const auto myCommand2 = makeCommand("bar", std::make_tuple(&barArg), std::function(bar));
    {
        barData = {};
        myCommand2.parse("bar drinks 3");
        TEST_ASSERT_EQUAL_UINT(1, barData.timesCalled);
        TEST_ASSERT_EQUAL_INT(3, barData.n);
    }
    {
        barData = {};
        myCommand2.parse("bar");
        TEST_ASSERT_EQUAL_UINT(1, barData.timesCalled);
        TEST_ASSERT_EQUAL_INT(-42, barData.n);
    }
    {
        barData = {};
        try
        {
            myCommand2.parse("bar drins 3");
            TEST_FAIL_MESSAGE("exception has not been thrown");
        }
        catch (const std::runtime_error &)
        {
        }

        TEST_ASSERT_EQUAL_UINT(0, barData.timesCalled);
        TEST_ASSERT_EQUAL_INT(0, barData.n);
    }
}

void test_command_argStringInt()
{
    const Argument<std::string> object = {.labels = {"thing"}, "nothing"};
    const Argument<int> number = {.labels = {"number"}, 0};
    const auto myCommand3 = makeCommand("two", std::make_tuple(&object, &number), std::function(two));
    myCommand3.parse("two");
    myCommand3.parse("two thing=tree number 15");
    myCommand3.parse("two number -1 thing car");
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_command_argVoid);
    RUN_TEST(test_command_argInt);
    RUN_TEST(test_command_argStringInt);

    UNITY_END();
}