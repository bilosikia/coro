//#include <sys/fcntl.h>
//#include <unistd.h>
//#include <string>
//#include "spdlog/spdlog.h"
//#include "coro/io/file.h"
//
//File::~File()
//{
//    if (fd_ > 0) {
//        close(fd_);
//    }
//}
//
//File File::open(const std::string& path)
//{
//    int fd = ::open(path.data(), O_RDWR);
//    if (fd == -1) {
//        SPDLOG_ERROR("open file failed: {}", path);
//        exit(-1);
//    }
//    File file{};
//    file.fd_ = fd;
//    return file;
//}
