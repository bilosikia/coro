#pragma once

#include "driver.h"
#include "util/noncopyable.h"
#include <atomic>
#include <memory>
#include <mutex>

enum WorkerState {
    NOT_START = 1,
    RUNING = 2,
    EXITING = 3,
    EXITED = 4
};

class Worker : public noncopyable {
    using SharedDriver = std::shared_ptr<Driver>;

public:


private:
    std::atomic_bool need_exit_;
    WorkerState state_;
    SharedDriver driver_;
    std::mutex driver_mutex_;
    // 如何只有一种类型
    std::queue<Task>

public :
    void run();

    void stop();

private:
    void handle_exit();
};