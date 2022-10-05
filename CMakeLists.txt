cmake_minimum_required(VERSION 3.20)
project(
        coro
        VERSION 0.1
        DESCRIPTION "an c++ coroutines runtime"
        LANGUAGES CXX
)

set(THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/third_party)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -stdlib=libc++")
set(CMAKE_EXE_LINKER_FLAGS "-lc++abi")

add_subdirectory(${THIRD_PARTY_DIR}/spdlog)
add_subdirectory(src)
add_subdirectory(tests)
