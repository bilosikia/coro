#pragma once

#include "driver.h"
#include "util/noncopyable.h"
#include <atomic>
#include <coroutine>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

class Runtime;

class Worker : public noncopyable {
public:
    Worker() = default;
    ~Worker();

    std::string name();
    void set_num(int num);
    void set_runtime(Runtime* runtime);

    void run();
    void stop();

    void add_runable_coro(std::coroutine_handle<> coro);

private:
    std::queue<std::coroutine_handle<>> drain_runable_queue();

private:
    int num_;
    std::atomic_bool need_exit_ = false;

    std::mutex runable_coros_mutex_;
    std::queue<std::coroutine_handle<>> runable_coros_;

    std::thread thread_;
    Runtime* runtime_;
};