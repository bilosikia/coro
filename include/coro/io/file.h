//#include <experimental/coroutine>
//#include "coro/task.h"
//
//class File {
//private:
//    File() = default;
//public:
//    ~File();
//
//    class Awaiter {
//        bool await_ready();
//        void await_resume() noexcept;
//        std::coroutine_handle<> await_suspend(std::experimental::coroutine_handle<> coroutine);
//    };
//
//public:
//    static File open(const std::string& path);
//
//    Awaiter read();
//
//private:
//    int fd_;
//};