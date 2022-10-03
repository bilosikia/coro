#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "coro/task.h"
#include "coro/runtime.h"
#include "doctest.h"

task<int> f(int val)
{
    co_return val;
}

task<int> g(int val)
{
    int a1 = co_await f(val);
    int a2 = co_await f(val);
    co_return a1* a2;
}

task<void> h()
{
    co_await g(10);
    std::cout << "hello world" << std::endl;
}

TEST_CASE("square_await")
{
    auto task = g(10);
    task.coroutine_.resume();
    CHECK_EQ(task.coroutine_.promise().result(), 100);
}

TEST_CASE("runtime_spawn_task")
{
    auto runtime = Runtime {};
    auto task = h();
    auto coroutine_handle = task.coroutine_;
    runtime.spawn(std::move(task));

    assert(runtime.has_task(coroutine_handle.address()) == true);

    runtime.start();
    std::this_thread::sleep_for(std::chrono::seconds(2));

    assert(runtime.has_task(task.coroutine_.address()) == false);

    runtime.stop();
    std::this_thread::sleep_for(std::chrono::seconds(2));
}