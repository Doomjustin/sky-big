#ifndef SKY_BIG_GENERATOR_H
#define SKY_BIG_GENERATOR_H

#include <concepts>
#include <coroutine>
#include <iterator>
#include <optional>
#include <span>
#include <utility>
#include <algorithm>
#include <ranges>

namespace sb {

template<typename T>
class Generator: public std::ranges::view_interface<Generator<T>> {
public:
    struct promise_type;
    using HandleType = std::coroutine_handle<promise_type>;

    struct promise_type {
        std::optional<T> value;

        Generator get_return_object()
        {
            return Generator{ HandleType::from_promise(*this) };
        }

        std::suspend_always initial_suspend() noexcept { return {}; }

        std::suspend_always final_suspend() noexcept { return {}; }

        void return_void() {}

        void unhandled_exception() { throw; }

        void await_transform(T from) = delete;

        template<std::convertible_to<T> From>
        std::suspend_always yield_value(From from)
        {
            value = std::move(from);
            return {};
        }
    };

    explicit Generator(HandleType handle)
      : handle_{ handle }
    {}

    Generator(const Generator&) = delete;
    Generator& operator=(const Generator&) = delete;

    Generator(Generator&& other) noexcept
      : handle_{ std::exchange(other.handle_, {}) }
    {}

    Generator& operator=(Generator&& other) noexcept
    {
        if (this == &other)
            return *this;

        if (handle_) handle_.destroy();

        handle_ = std::exchange(other.handle_, {});

        return *this;
    }

    ~Generator()
    {
        if (handle_) handle_.destroy();
    }

    class Iterator {
    public:
        explicit Iterator(HandleType handle)
          : handle_{ handle }
        {}

        void operator++()
        {
            handle_.resume();
        }

        const T& operator*() const
        {
            return *handle_.promise().value;
        }

        bool operator==(std::default_sentinel_t) const
        {
            return !handle_ || handle_.done();
        }

    private:
        HandleType handle_;
    };

    Iterator begin()
    {
        if (handle_)
            handle_.resume();

        return Iterator{ handle_ };
    }

    std::default_sentinel_t end() { return {}; }

private:
    HandleType handle_;
};


template<std::integral T = int>
Generator<T> range(T start, T end, int stride = 1)
{
    while (start < end) {
        co_yield start;
        start += stride;
    }

    co_return;
}

template<std::integral T = int>
Generator<T> range(T end)
{
    return range(0, end, 1);
}

template <typename T>
Generator<T> range(const std::span<const T> list)
{
    for (const auto& element: list)
        co_yield element;
}

template <typename R>
    requires std::ranges::contiguous_range<R> &&
             std::ranges::sized_range<R>
Generator<std::ranges::range_value_t<R>> range(R&& r)
{
    return range(std::span<const std::ranges::range_value_t<R>>(r));
}

template<std::integral T = int>
Generator<T> fibonacci(int max_times)
{
    co_yield 0;
    co_yield 1;

    T a = 0;
    T b = 1;
    auto times = max_times - 2;
    while (times-- > 0) {
        co_yield a + b;
        b = a + b;
        a = b - a;
    }

    co_return;
}

} // namespace sb

#endif // SKY_BIG_GENERATOR_H