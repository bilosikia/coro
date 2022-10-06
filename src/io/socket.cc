#include "coro/io/socket.h"
#include "coro/runtime.h"
#include "spdlog/spdlog.h"
#include <netinet/in.h>
#include <unistd.h>

Socket::~Socket()
{
    if (fd_ > 0) {
        close(fd_);
    }
}

Socket Socket::open(int port)
{
    int fd = socket(PF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        SPDLOG_ERROR("open socket failed: port= {}", port);
        exit(-1);
    }

    sockaddr_in addr_in;
    bzero(&addr_in, sizeof(addr_in));
    addr_in.sin_port = htonl(port);
    addr_in.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr_in.sin_family = AF_INET;
    bind(fd, (sockaddr*)&addr_in, sizeof(addr_in));

    Socket sock {};
    sock.fd_ = fd;
    return sock;
}

Socket::Awaiter Socket::accept()
{
    return Socket::Awaiter(this);
}

int Socket::fd()
{
    return fd_;
}

Socket::Awaiter::Awaiter(Socket* ser_sock)
    : ser_sock_(ser_sock)
    , cli_sock_fd_(0)
{
}

bool Socket::Awaiter::await_ready()
{
    return false;
}

Socket Socket::Awaiter::await_resume() noexcept
{
    Socket sock {};
    sock.fd_ = cli_sock_fd_;
    return sock;
}

std::experimental::coroutine_handle<> Socket::Awaiter::await_suspend(std::experimental::coroutine_handle<> coroutine)
{
    coroutine_handle_ = coroutine;
    Runtime* runtime = enter_runtime_context();
    Driver& driver = runtime->get_driver();

    DriverEvent event;
    event.fd_ = ser_sock_->fd_;
    event.ready_func_ = [this]() {
        int cli_fd = ::accept(ser_sock_->fd_, nullptr, nullptr);
        if (cli_fd == -1) {
            SPDLOG_ERROR("accept error");
            exit(-1);
        }
        this->cli_sock_fd_ = cli_fd;
        coroutine_handle_.resume();
    };
    driver.add_event(event);

    return std::experimental::noop_coroutine();
}
