#include "doctest.h"
#include "spdlog/spdlog.h"
#include "coro/task.h"
#include "coro/runtime.h"
#include "coro/io/socket.h"

task<> accept() {
    Socket sock = Socket::open(9000);
    Socket cli_sock = co_await sock.accept();

    SPDLOG_INFO("cli sock {}", cli_sock.fd());
}

TEST_CASE("accept_socket") {
    auto runtime = Runtime {};
    runtime.start();

    auto task = accept();
    auto coroutine_handle = task.coroutine_;
    runtime.spawn(std::move(task));

    std::this_thread::sleep_for(std::chrono::seconds(10000));
}