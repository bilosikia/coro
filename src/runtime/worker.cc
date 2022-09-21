#include "coro/worker.h"

void Worker::run() {
    while (true) {
        if (need_exit_) {
            handle_exit();
            return;
        }


    }
}

void Worker::handle_exit() {

}
