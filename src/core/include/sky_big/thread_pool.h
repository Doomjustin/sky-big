#ifndef SKY_BIG_THREAD_POOL_H
#define SKY_BIG_THREAD_POOL_H

#include "queue.h"

#include <condition_variable>
#include <mutex>
#include <thread>
#include <type_traits>
#include <vector>
#include <future>

namespace sb {

class Function {
private:
    struct Callable {
        virtual ~Callable() = default;

        virtual void call() = 0;
    };

    template<typename Func>
    struct Functor: Callable {
        Func func;

        Functor(Func f) 
          : func{ std::move(f) } 
        {}

        ~Functor() = default;

        void call() override
        {
            func();
        }
    };

    std::unique_ptr<Callable> functor_;

public:
    template<typename Func>
    Function(Func f)
      : functor_{ std::make_unique<Functor<Func>>(std::move(f)) }
    {}

    Function() = default;

    Function(Function&& other) noexcept
        : functor_{ std::move(other.functor_) }
    {}

    Function& operator=(Function&& other) noexcept
    {
        if (this != &other) {
            functor_ = std::move(other.functor_);
        }
        return *this;
    }

    Function(const Function&) = delete;
    Function& operator=(const Function&) = delete;

    ~Function() = default;

    void operator() ()
    {
        functor_->call();
    }
};


class ThreadPool {
private:
    std::vector<std::jthread> threads_;
    concurrency::Queue<Function> work_queue_;
    std::mutex m_;
    std::condition_variable cv_;
    std::atomic<bool> done_{ false };

public:
    explicit ThreadPool(unsigned size);

    ~ThreadPool();

    template<typename Func, typename... Args>
    auto submit(Func f, Args&&... args)
    {
        using ResultType = std::invoke_result_t<Func, Args...>;
        
        std::packaged_task task{ std::move(f), std::forward<Args>(args)...};
        auto result = task.get_future();
        work_queue_.push(std::move(task));

        return result;
    }

    void stop();

private:
    void work_thread();
};

} // namespace sb

#endif // !SKY_BIG_THREAD_POOL_H