#ifndef SKY_BIG_QUEUE_H
#define SKY_BIG_QUEUE_H

#include <algorithm>
#include <deque>
#include <queue>
#include <mutex>

namespace sb::concurrency {

template<typename T, typename Sequence = std::deque<T>>
class Queue {
public:
    T pop()
    {
        std::lock_guard<std::mutex> locker{ m_ };
        auto item = std::move(queue_.front());
        queue_.pop();
        return item;
    }

    bool try_pop(T& item)
    {
        std::lock_guard<std::mutex> locker{ m_ };
        if (queue_.empty()) return false;
     
        item = std::move(queue_.front());
        queue_.pop();
        return true;
    }

    void push(T item)
    {
        std::lock_guard<std::mutex> locker{ m_ };
        queue_.push(std::move(item));
    }

    template<typename Iter>
    void push(Iter begin, Iter end)
    {
        std::lock_guard<std::mutex> locker{ m_ };
        for (auto it = begin; it != end; ++it)
            queue_.push(std::move(*it));
    }

    constexpr bool empty() const noexcept
    {
        std::lock_guard<std::mutex> locker{ m_ };
        return queue_.empty();
    }

private:
    std::queue<T, Sequence> queue_;
    mutable std::mutex m_;
};

} // namespace sb::concurrency

#endif // !SKY_BIG_QUEUE_H