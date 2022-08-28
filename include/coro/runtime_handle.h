#pragma once

#include <memory>
#include "runtime.h"

class RuntimeHandle {
private:
    std::weak_ptr<Runtime> runtime_;

public:
    RuntimeHandle(std::shared_ptr<Runtime> runtime)
            : runtime_(std::move(runtime)) {
    }

public:
    void run();

    void exit();

    void spawn();
};