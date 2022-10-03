#pragma once

#include "util/noncopyable.h"
#include <experimental/coroutine>

template <typename T>
class task;
class Runtime;

class promise_type_base : noncopyable {
public:
    struct final_awaitable {
        bool await_ready() const noexcept { return false; }

        template <typename PromiseType>
        std::experimental::coroutine_handle<> await_suspend(
            std::experimental::coroutine_handle<PromiseType> coro) noexcept
        {
            promise_type_base& promise = coro.promise();
            if (promise.continuation_) {
                return coro.promise().continuation_;
            }
            if (promise.runtime_) {
                assert(promise.continuation_ == nullptr);
                promise.remove_from_runtime(promise.continuation_.address());
            }
            return std::experimental::noop_coroutine();
        }

        void await_resume() noexcept { }
    };

    std::experimental::suspend_always initial_suspend() noexcept
    {
        return std::experimental::suspend_always {};
    }

    final_awaitable final_suspend() noexcept { return final_awaitable {}; }

    void unhandled_exception() noexcept { std::terminate(); }

    void set_continuation(std::experimental::coroutine_handle<> continuation)
    {
        continuation_ = continuation;
    }

    void set_runtime(Runtime* runtime)
    {
        runtime_ = runtime;
    }

    void remove_from_runtime(void* key);

private:
    std::experimental::coroutine_handle<> continuation_ = nullptr;
    // runtime spawn task
    Runtime* runtime_ = nullptr;
};

template <typename T>
class promise_type final : public promise_type_base {
public:
    task<T> get_return_object() noexcept
    {
        return task {
            std::experimental::coroutine_handle<promise_type>::from_promise(
                *this)
        };
    }

    void return_value(T value)
    {
        result_ = value;
        return;
    }

    T result() { return result_; }

private:
    T result_;
};

template <>
class promise_type<void> final : public promise_type_base {
public:
    task<void> get_return_object() noexcept;

    void return_void() { }
};

template <typename T = void>
class task : public noncopyable {
public:
    using promise_type = promise_type<T>;
    using value_type = T;

    task() noexcept = default;

    task(std::experimental::coroutine_handle<promise_type> coro)
        : coroutine_(coro)
    {
    }

    ~task()
    {
        if (coroutine_) {
            coroutine_.destroy();
        }
    }

    task(task&& task) noexcept
        : coroutine_(std::exchange(task.coroutine_, nullptr))
    {
    }

    task& operator=(task&& task) noexcept
    {
        if (std::addressof(task) != this) {
            if (coroutine_) {
                coroutine_.destroy();
            }
            coroutine_ = std::exchange(task.coroutine_, nullptr);
        }
        return *this;
    }

    struct awaiter : noncopyable {
    public:
        using coroutine_handle = std::experimental::coroutine_handle<promise_type>;
        coroutine_handle coroutine_;

        explicit awaiter(coroutine_handle coro)
            : coroutine_(coro) {};

        bool await_ready() const noexcept
        {
            return !coroutine_ || coroutine_.done();
        }

        std::experimental::coroutine_handle<> await_suspend(
            std::experimental::coroutine_handle<> awaiting_coro) noexcept
        {
            coroutine_.promise().set_continuation(awaiting_coro);
            return coroutine_;
        }

        decltype(auto) await_resume()
        {
            return this->coroutine_.promise().result();
        }
    };

    awaiter operator co_await() const& noexcept { return awaiter { coroutine_ }; }

public:
    std::experimental::coroutine_handle<promise_type> coroutine_;
};