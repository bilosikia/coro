cmake_minimum_required(VERSION 3.22)
project(
        coro
        VERSION 0.1
        DESCRIPTION "an c++ coroutines runtime"
        LANGUAGES CXX
)

set(THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/third_party)

add_subdirectory(${THIRD_PARTY_DIR}/spdlog)
add_subdirectory(src)
add_subdirectory(tests)
