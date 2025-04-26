#include "thread_pool.h"

#include <mutex>
#include <spdlog/spdlog.h>

namespace sb {

ThreadPool::ThreadPool(unsigned size)
{
    for (unsigned i = 0; i < size; ++i)
        threads_.emplace_back([this] { work_thread(); });
}

ThreadPool::~ThreadPool()
{
    stop();
}

void ThreadPool::stop()
{
    done_.store(true);
    cv_.notify_all();
}

void ThreadPool::work_thread()
{
    Function task;
    while (!done_) {
        std::unique_lock<std::mutex> locker{ m_ };
        cv_.wait(locker, [this] { return done_ || !work_queue_.empty(); });

        if (work_queue_.try_pop(task))
            task();
    }
}

} // namespace sb