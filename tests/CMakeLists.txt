cmake_minimum_required(VERSION 3.22)
project(tests VERSION 0.0.1 LANGUAGES CXX)


set(LIB_SRC_DIR ${CMAKE_SOURCE_DIR}/src)
add_executable(tests
    task.cc
    ${LIB_SRC_DIR}/runtime/runtime.cc
    ${LIB_SRC_DIR}/runtime/worker.cc
    ${LIB_SRC_DIR}/runtime/task.cc
)

# include dir
set(THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/third_party)
set(DOCTEST_INCLUDE_DIR ${THIRD_PARTY_DIR}/doctest/doctest)

target_include_directories(${PROJECT_NAME} PRIVATE ${DOCTEST_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)