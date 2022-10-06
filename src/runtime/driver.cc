#include "coro/driver.h"
#include "coro/runtime.h"
#include <sys/epoll.h>
#include <spdlog/spdlog.h>

Driver::Driver(Runtime* runtime)
    : runtime_(runtime), epoll_fd_(0)
{
}

void Driver::init()
{
    if (epoll_fd_ != 0) {
        SPDLOG_ERROR("driver has inited");
        return;
    }
    int fd = epoll_create(10);
    if (fd == -1) {
        SPDLOG_ERROR("create epoll failed");
        exit(-1);
    }
    epoll_fd_ = fd;
}

void Driver::add_event(DriverEvent event)
{
    ready_handle_[event.fd_] = std::move(event.ready_func_);

    epoll_event ev;
    ev.data.fd = event.fd_;
    ev.events = EPOLLIN;
    int res = epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, event.fd_, &ev);
    if (res == -1) {
        SPDLOG_ERROR("add listen sock error");
        exit(-1);
    }
}

void Driver::poll()
{
    epoll_event events;
    int res = epoll_wait(epoll_fd_, &events, 1, -1);
    if (res == -1) {
        SPDLOG_ERROR("epoll wait error");
        exit(-1);
    }

    int fd = events.data.fd;
    ready_handle_[fd]();
}
