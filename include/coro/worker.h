#pragma once

#include <atomic>
#include <mutex>
#include <memory>
#include "driver.h"

enum WorkerState {
    NOT_START = 1,
    RUNING    = 2,
    EXITING   = 3,
    EXITED    = 4
};

class Worker {
    using SharedDriver = std::shared_ptr<Driver>;

private:
    // need exit
    std::atomic_bool exit_;
    WorkerState state_;
    SharedDriver driver_;
    std::mutex driver_mutex_;
    // 如何只有一种类型
    std::queue<Task>

public:
    void run();
    void stop();
};