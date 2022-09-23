#pragma once

#include "task.h"
#include "util/noncopyable.h"
#include "worker.h"
#include <array>
#include <memory>
#include <thread>
#include <unordered_map>
#include <vector>

class Runtime : public noncopyable {
public:
    using Task = task<void>;

    Runtime();
    ~Runtime();
    Runtime(Runtime&&) = delete;
    Runtime& operator=(Runtime&&) = delete;

public:
    void start();
    void stop();
    void spawn(task<void> task);
    void remove_task(void* key);

private:
    Worker& get_idl_worker();

private:
    int core_ = 4;
    std::array<Worker, 4> workers_;

    std::mutex tasks_mutex_;
    std::unordered_map<void*, Task> tasks_;
};