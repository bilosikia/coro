#pragma once

#include "util/noncopyable.h"
#include <experimental/coroutine>
#include <unordered_map>

class Runtime;

class DriverEvent{
public:
    int fd_;
    std::function<void()> ready_func_;
};

class Driver: public noncopyable {
public:
    Driver() = default;

    explicit Driver(Runtime* runtime);

    void init();

    void add_event(DriverEvent event);
    void poll();

private:
    int epoll_fd_ = 0;

    std::unordered_map<int, std::function<void()>> ready_handle_;

    Runtime *runtime_{};
};