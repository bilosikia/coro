#include "coro/task.h"
#include "coro/runtime.h"

task<void> task_promise_type<void>::get_return_object() noexcept
{
    return task {
        std::coroutine_handle<task_promise_type>::from_promise(*this)
    };
}

void promise_type_base::remove_from_runtime(void* key)
{
    runtime_->remove_task(key);
}
