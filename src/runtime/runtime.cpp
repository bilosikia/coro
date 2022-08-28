#include "coro/runtime.h"
#include "coro/runtime_handle.h"

Runtime &Runtime::operator=(const Runtime &&r) {
    core_ = r.core_;
}

Runtime::~Runtime() {
    stop();
}

void Runtime::start() {
    for (auto &worker: workers_) {
        worker.run();
    }
}

void Runtime::stop() {
    for (auto &worker: workers_) {
        worker.stop();
    }
}