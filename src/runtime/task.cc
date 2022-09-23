#include "coro/task.h"

task<void> promise_type<void>::get_return_object() noexcept
{
    return task {
        std::experimental::coroutine_handle<promise_type>::from_promise(*this)
    };
}