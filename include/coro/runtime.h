#pragma once

#include "task.h"
#include "util/noncopyable.h"
#include "worker.h"
#include "driver.h"
#include <array>
#include <memory>
#include <thread>
#include <unordered_map>
#include <vector>
#include <cassert>


class Runtime : public noncopyable {
public:
    using Task = task<void>;

    Runtime() = default;
    ~Runtime();
    Runtime(Runtime&&) = delete;
    Runtime& operator=(Runtime&&) = delete;

public:
    void start();
    void stop();
    void spawn(task<void> task);
    void remove_task(void* key);
    bool has_task(void* key);
    Driver& get_driver();

private:
    Worker& get_idl_worker();

private:
    int core_ = 1;
    std::array<Worker, 1> workers_;

    std::mutex tasks_mutex_;
    std::unordered_map<void*, Task> tasks_;

    Driver driver_;
};

Runtime* enter_runtime_context();

void set_runtime_context(Runtime* rt);