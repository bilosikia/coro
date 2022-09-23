#include "coro/runtime.h"
#include "coro/runtime_handle.h"

Runtime::Runtime()
{
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
    while (true)
        ;
}

void Runtime::stop()
{
    for (auto& worker : workers_) {
        worker.stop();
    }
}

void Runtime::spawn(Task t)
{
    t.coroutine_.promise().set_runtime(this);
    auto guard = std::lock_guard(tasks_mutex_);
    auto handle = t.coroutine_;
    tasks_[t.coroutine_.address()] = std::move(t);

    auto& idl_worker = get_idl_worker();
    idl_worker.add_runable_coro(handle);
}

void Runtime::remove_task(void* key)
{
    auto guard = std::lock_guard(tasks_mutex_);
    tasks_.erase(key);
}

Worker& Runtime::get_idl_worker()
{
    return workers_[0];
}
