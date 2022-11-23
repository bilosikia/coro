#pragma once

#include "coro/task.h"
#include <coroutine>
#include <sys/socket.h>

class Socket {
private:
    Socket() = default;

public:
    ~Socket();

    class Awaiter {
    public:
        explicit Awaiter(Socket* ser_sock);

    public:
        bool await_ready();
        std::coroutine_handle<> await_suspend(std::coroutine_handle<> coroutine);
        Socket await_resume() noexcept;

    private:
        std::coroutine_handle<> coroutine_handle_;
        int cli_sock_fd_;
        Socket* ser_sock_;
    };

public:
    static Socket open(int port);

    Awaiter accept();
    int fd();

private:
    int fd_;
};