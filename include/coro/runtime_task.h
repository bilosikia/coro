#pragma once

#include "util/noncopyable.h"
#include <experimental/coroutine>

class Runtime;

class RuntimeTask : public noncopyable {
    class promise_type;

    struct final_awaitable {
        bool await_ready() const noexcept
        {
            return false;
        }

        void await_suspend(
            std::experimental::coroutine_handle<promise_type> coro) noexcept;

        void await_resume() noexcept
        {
        }
    };

    class promise_type {
        RuntimeTask get_return_object() noexcept
        {
            return RuntimeTask {
                std::experimental::coroutine_handle<promise_type>::from_promise(*this)
            };
        }

        std::experimental::suspend_always initial_suspend() noexcept
        {
            return std::experimental::suspend_always {};
        }

        final_awaitable final_suspend() noexcept
        {
            return final_awaitable {};
        }

        void unhandled_exception() noexcept
        {
            std::terminate();
        }

        void return_void() { }
    public:
        promise_type(Runtime *runtime);

        Runtime *runtime_;
    };

public:
    RuntimeTask(std::experimental::coroutine_handle<promise_type> coroutine);
    ~RuntimeTask();

private:
    std::experimental::coroutine_handle<promise_type> coro_;
};