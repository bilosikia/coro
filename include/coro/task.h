#pragma once

#include <experimental/coroutine>

template<typename R>
class PromiseType;

template<typename R>
class Task {
public:
    using promise_type = PromiseType<R>;
    using CoroHandle = std::experimental::coroutine_handle<promise_type>;
private:
    CoroHandle coro_;
public:
    Task(CoroHandle handle)
            : coro_(handle) {}
};

template<typename R>
struct AwaiterBase {
    using CoroHandle = std::experimental::coroutine_handle<PromiseType<R>>;
    CoroHandle handle_;

    AwaiterBase(CoroHandle handle)
            : handle_(handle) {};

    AwaiterBase(AwaiterBase &) = delete;

    AwaiterBase &operator=(AwaiterBase &) = delete;

    AwaiterBase(AwaiterBase &&other) noexcept
            : handle_(std::exchange(other.handle_, nullptr)) {
    }

    AwaiterBase &operator=(AwaiterBase &&other) {
        std::swap(handle_, other._handle);
        return *this;
    }

    ~AwaiterBase() {
        if (handle_) {
            handle_.destroy();
            handle_ = nullptr;
        }
    }

    bool await_ready() const noexcept { return false; }
};

template<typename R>
class PromiseType {
private:
    struct FinalAwaiter {
        bool await_ready() const noexcept { return false; }

        auto await_suspend(std::experimental::coroutine_handle<PromiseType<R>> h) noexcept {
            return h.promise().continuation_;
        }

        void await_resume() noexcept {}
    };

    struct Awaiter : public AwaiterBase<R> {
        Awaiter(typename AwaiterBase<R>::CoroHandle handle)
                : AwaiterBase<R>(handle) {}

        std::experimental::coroutine_handle<> await_suspend(
                std::experimental::coroutine_handle<> continuation
        ) {
            auto promise = handle_.promise();
            promise.continuation_ = continuation;
            promise.coro_
        }
    };

private:
    std::experimental::coroutine_handle<> continuation_;
    std::experimental::coroutine_handle<PromiseType<R>> coro_;

public:
    std::experimental::suspend_always initial_suspend() noexcept { return {}; }

    FinalAwaiter final_suspend() noexcept { return {}; }

    Task<R> get_return_object() noexcept {
        auto handle = Task<R>::CoroHandle::from_promise(&*this);
        return Task<R>(handle);
    }

    Awaiter operator
    co_await() noexcept {
        auto handle = Task<R>::CoroHandle::from_promise(&*this);
        return Awaiter(handle);
    }
};