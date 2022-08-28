主要功能
- 多线程, work-steal 调度器
- 每一个线程的职责相同，没有专门的 io 线程
- 调度器和 reactor 解耦

主要组件
- 线程池：不断的执行readied task, poll driver
- driver：可以阻塞，类似 reactor

# task
- 能取回 spawn task 的运行结果吗？
- spawn 的类型必须必须要 task<void>?
- 能支持取消？
- task 的生命周期
只需要管理最上层 task 的 handler。

# runtime
- runtime 只关心 task 的调度, 把所有的 task 同等看待, 不知道哪些 task 是需要等待 io的。
- 
- runtime 如何抹掉 task 结果类型? 使得 runtime 能支持不同的结果的 task。 
1. BaseTask
2. 

# io_service
- io_service 只能运行在 runtime 环境下
- 当 io 类 task 被调度时，将 io 注册到 reactor。然后将运行权转给 caller。
- caller 会暂停执行，直到将运行权转到 event_loop。
- event_loop 会执行 run-able task. 如果没有，会 poll reactor。
- 得到的 poll event， 会得到 coro handler。resume 改协程。