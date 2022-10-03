#include "coro/worker.h"
#include "spdlog/spdlog.h"

Worker::~Worker()
{
    thread_.join();
}

void Worker::run()
{
    auto f = [this]() {
        while (true) {
            if (need_exit_) {
                break;
            }

            auto runable_coros = drain_runable_queue();
            while (!runable_coros.empty()) {
                auto coro = runable_coros.front();
                runable_coros.pop();
                coro.resume();
            }
        }
        SPDLOG_INFO("{} exited", name());
    };

    SPDLOG_INFO("{} stared", name());
    thread_ = std::thread(f);
}

void Worker::stop()
{
    if (!need_exit_) {
        need_exit_ = true;
    }
}

void Worker::add_runable_coro(std::experimental::coroutine_handle<> coro)
{
    auto guard = std::lock_guard(runable_coros_mutex_);
    runable_coros_.push(coro);
}

std::queue<std::experimental::coroutine_handle<>> Worker::drain_runable_queue()
{
    auto guard = std::lock_guard(runable_coros_mutex_);
    std::queue<std::experimental::coroutine_handle<>> queue;
    std::swap(runable_coros_, queue);

    return queue;
}

std::string Worker::name()
{
    return std::string("worker-") + std::to_string(num_);
}

void Worker::set_num(int num)
{
    num_ = num;
}
