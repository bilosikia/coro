#include "coro/runtime_task.h"
#include "coro/runtime.h"

RuntimeTask::RuntimeTask(
    std::experimental::coroutine_handle<promise_type> coroutine)
    : coro_(coroutine)
{
}

RuntimeTask::~RuntimeTask()
{
    if (coro_) {
        coro_.destroy();
    }
}

RuntimeTask::promise_type::promise_type(Runtime* runtime)
    : runtime_(runtime)
{
}

void RuntimeTask::final_awaitable::await_suspend(std::experimental::coroutine_handle<promise_type> coro) noexcept
{
    auto promise = coro.promise();
    auto coroutine_handle = std::experimental::coroutine_handle<promise_type>::from_promise(promise);
    promise.runtime_->remove_task(coroutine_handle.address());
}
