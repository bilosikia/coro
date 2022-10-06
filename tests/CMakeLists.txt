cmake_minimum_required(VERSION 3.22)
project(tests VERSION 0.0.1 LANGUAGES CXX)

set(LIB_SRC_DIR ${CMAKE_SOURCE_DIR}/src)
add_executable(tests
    task.cc
    io.cc
    ${LIB_SRC_DIR}/runtime/runtime.cc
    ${LIB_SRC_DIR}/runtime/worker.cc
    ${LIB_SRC_DIR}/runtime/task.cc
    ${LIB_SRC_DIR}/runtime/driver.cc
    ${LIB_SRC_DIR}/io/socket.cc
)

# include dir
set(DOCTEST_INCLUDE_DIR ${THIRD_PARTY_DIR}/doctest/doctest)

target_include_directories(${PROJECT_NAME} PRIVATE ${DOCTEST_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${THIRD_PARTY_DIR}/spdlog/include/)

target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}/third_party/spdlog/)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)