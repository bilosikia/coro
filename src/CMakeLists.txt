add_library(coro
        lib.cpp
        runtime/runtime_handle.cpp
        runtime/runtime.cpp
        runtime/worker.cpp
        runtime/driver.cpp
)

target_include_directories(coro PUBLIC ../include/)
target_compile_features(coro PUBLIC cxx_std_20)