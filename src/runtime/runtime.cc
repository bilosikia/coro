#include <spdlog/spdlog.h>

#include "coro/runtime.h"

thread_local Runtime* rt_context;

Runtime::~Runtime()
{
    stop();
}

void Runtime::start()
{
    driver_.init();
    for (int i = 0; i < workers_.size(); i++) {
        auto& worker = workers_[i];
        worker.set_num(i);
        worker.set_runtime(this);
        worker.run();
    }
}

void Runtime::stop()
{
    for (auto& worker : workers_) {
        worker.stop();
    }
}

void Runtime::spawn(Task t)
{
    SPDLOG_INFO("spawn task");

    t.coroutine_.promise().set_runtime(this);
    auto handle = t.coroutine_;

    {
        auto guard = std::lock_guard(tasks_mutex_);
        tasks_[t.coroutine_.address()] = std::move(t);
    }

    auto& idl_worker = get_idl_worker();
    idl_worker.add_runable_coro(handle);
}

void Runtime::remove_task(void* key)
{
    SPDLOG_INFO("remove task");
    auto guard = std::lock_guard(tasks_mutex_);
    tasks_.erase(key);
}

Worker& Runtime::get_idl_worker()
{
    return workers_[0];
}

bool Runtime::has_task(void* key)
{
    return tasks_.contains(key);
}

Driver& Runtime::get_driver()
{
    return driver_;
}

Runtime* enter_runtime_context() {
    return rt_context;
}

void set_runtime_context(Runtime* rt) {
    assert(rt_context == nullptr);
    rt_context = rt;
}
