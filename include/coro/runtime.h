#pragma once

#include <thread>
#include <memory>
#include <vector>
#include "worker.h"
#include "task.h"

class Runtime {
private:
    // thread core num
    int core_ = 4;
    // worker threads
    std::vector<Worker> workers_;

public:
    Runtime() {}

    Runtime(const Runtime &) = delete;

    Runtime &operator=(const Runtime &) = delete;

    Runtime(const Runtime &&r);

    Runtime &operator=(const Runtime &&r);

    ~Runtime();

public:
    void start();
    void stop();

    template<class R>
    void spawn(task task) {

    }
};