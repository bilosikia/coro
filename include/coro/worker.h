#pragma once

#include "driver.h"
#include "util/noncopyable.h"
#include <atomic>
#include <experimental/coroutine>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

class Worker : public noncopyable {
public:
    Worker() = default;

    void run();
    void stop();

    void add_runable_coro(std::experimental::coroutine_handle<> coro);

private:
    std::queue<std::experimental::coroutine_handle<>> drain_runable_queue();

private:
    std::atomic_bool need_exit_ = false;

    std::mutex runable_coros_mutex_;
    std::queue<std::experimental::coroutine_handle<>> runable_coros_;

    std::thread thread_;
};