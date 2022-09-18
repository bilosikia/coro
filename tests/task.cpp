#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "coro/task.h"

TEST_CASE("square await")
{
    auto f = [](int val) -> task<int> {
        co_return val;
    };
    auto g = [&](int val) -> task<int> {
        int a1 = co_await f(val);
        int a2 = co_await f(val);
        co_return a1 * a2;
    };
    auto task = g(10);
    task.coroutine_.resume();
    CHECK_EQ(task.coroutine_.promise().result(), 100);
}