add_library(coro
        lib.cc
        runtime/runtime_handle.cc
        runtime/runtime.cc
        runtime/worker.cc
        runtime/driver.cc
        runtime/runtime_task.cc
)

target_include_directories(coro PUBLIC ../include/)
target_compile_features(coro PUBLIC cxx_std_20)