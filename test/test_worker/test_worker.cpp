#include <Worker.hpp>
#include <atomic>
#include <chrono>
#include <unity.h>

using namespace std::chrono_literals;

static std::atomic<bool> workCompleted = false;
static constexpr auto work = []() {
    workCompleted = true;
};

static constexpr auto T_eps = 10ms;

void setUp(void)
{
    workCompleted = false;
}

void tearDown(void)
{
}

void test_do_work_once()
{
    Worker worker;
    worker.restart(work, 0ms);
    worker.finish();
    TEST_ASSERT_TRUE(workCompleted);
}

void test_do_work_once_after_delay()
{
    Worker worker;
    worker.restart(work, 2 * T_eps);
    std::this_thread::sleep_for(T_eps);
    TEST_ASSERT_FALSE(workCompleted);
    std::this_thread::sleep_for(2 * T_eps);
    TEST_ASSERT_TRUE(workCompleted);
}

void test_abort()
{
    Worker worker;
    worker.restart(work, T_eps);
    worker.cancelStartup();
    TEST_ASSERT_FALSE(workCompleted);
}

void test_abort_and_run()
{
    Worker worker;
    worker.restart(work, 2 * T_eps);
    std::this_thread::sleep_for(T_eps);
    worker.restart(work, 3 * T_eps);
    std::this_thread::sleep_for(2 * T_eps);
    TEST_ASSERT_FALSE(workCompleted);
    std::this_thread::sleep_for(2 * T_eps);
    TEST_ASSERT_TRUE(workCompleted);
}

void test_continuous_abort()
{
    Worker worker;
    for (auto i = 10; i > 0; --i)
    {
        worker.restart(work, 2 * T_eps);
        std::this_thread::sleep_for(T_eps);
        TEST_ASSERT_FALSE(workCompleted);
    }

    std::this_thread::sleep_for(2 * T_eps);
    TEST_ASSERT_TRUE(workCompleted);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_do_work_once);
    RUN_TEST(test_do_work_once);
    RUN_TEST(test_do_work_once_after_delay);
    RUN_TEST(test_abort);
    RUN_TEST(test_abort_and_run);
    RUN_TEST(test_continuous_abort);
    return UNITY_END();
}
