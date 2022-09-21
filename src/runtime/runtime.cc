#include "coro/runtime.h"
#include "coro/runtime_handle.h"

Runtime&
Runtime::operator=(const Runtime&& r)
{
    core_ = r.core_;
}

Runtime::~Runtime()
{
    stop();
}

void Runtime::start()
{
    for (auto& worker : workers_) {
        worker.run();
    }
}

void Runtime::stop()
{
    for (auto& worker : workers_) {
        worker.stop();
    }
}

void Runtime::spawn(task<void> t)
{
    auto runtime_task = [](task<void> t) -> RuntimeTask {
        co_await t;
        co_return;
    }();

}

void Runtime::remove_task(void* key)
{
    auto guard = std::lock_guard(tasks_mutex_);
    tasks_.erase(key);
}
