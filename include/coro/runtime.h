#pragma once

#include "worker.h"
#include "task.h"
#include "util/noncopyable.h"
#include "runtime_task.h"
#include <thread>
#include <memory>
#include <vector>
#include <unordered_map>

class Runtime : public noncopyable {
private:
    int core_ = 4;
    std::vector<Worker> workers_;

    std::mutex tasks_mutex_;
    std::unordered_map<void *, RuntimeTask> tasks_;
public:
    Runtime();

    ~Runtime();

public:
    void start();
    void stop();
    void spawn(task<void> task);
    void remove_task(void *key);
};