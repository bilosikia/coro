cmake_minimum_required(VERSION 3.20)
project(
        coro
        VERSION 0.1
        DESCRIPTION "an c++ coroutines runtime"
        LANGUAGES CXX
)

# for found libpthread
set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)

set(THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/third_party)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
set(CMAKE_EXE_LINKER_FLAGS "-lpthread")

add_subdirectory(${THIRD_PARTY_DIR}/spdlog)
add_subdirectory(src)
add_subdirectory(tests)
