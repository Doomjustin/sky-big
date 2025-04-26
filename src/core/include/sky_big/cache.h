#ifndef SKY_BIG_CACHE_H
#define SKY_BIG_CACHE_H

#include "value.h"

#include <gsl/gsl>

#include <cstddef>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <string_view>
#include <typeindex>
#include <algorithm>


namespace sb {

class LRUCache {
private:
    struct Node {
        std::string key;
        std::unique_ptr<BaseValue> value = nullptr;

        Node* prev = nullptr;
        Node* next = nullptr;

        template<typename T>
        Node(std::string_view key, T value)
          : key{ key },
            value{ std::make_unique<Value<T>>(std::move(value)) }
        {}

        Node() = default;

        template<typename T>
        void set_value(T val) noexcept
        {
            value->set_value(std::move(val));
        }

        template<typename T>
        const T& get_value() const noexcept
        {
            return value->get_value<T>();
        }

        std::type_index get_type() const noexcept
        {
            return value->get_type();
        }
    };

    std::size_t capacity_;
    std::unordered_map<std::string, Node> values_;

    Node head_{};
    Node tail_{};

    std::mutex m_; // TODO: 锁的粒度是否太大？目前感觉还行

public:
    explicit LRUCache(std::size_t capacity)
      : capacity_{ capacity }
    {
        head_.next = &tail_;
        tail_.prev = &head_;
    }

    // TODO: 实现移动语义

    template<typename T>
    void put(const std::string& key, T value) noexcept
    {
        std::lock_guard<std::mutex> locker{ m_ };
        if (is_exist(key)) {
            values_[key].set_value(value);
            move_to_head(&values_[key]);
        }
        else {
            auto [iter, succed] = values_.emplace(key, Node{ key, value });
            Ensures(succed);
            add_new_node(&iter->second);

            if (is_full())
                remove_last_value();
        }
    }

    void put(const std::string& key, const char* value) noexcept
    {
        put(key, std::string{ value });
    }

    template<typename T>
    std::optional<T> get(const std::string& key) noexcept
    {
        std::lock_guard<std::mutex> locker{ m_ };
        if (is_exist(key) && typeid(T) == values_[key].get_type())
            return values_[key].get_value<T>();

        return {};
    }

    void clear() noexcept
    {
        std::lock_guard<std::mutex> locker{ m_ };
        values_.clear();
        head_.next = &tail_;
        tail_.prev = &head_;
    }

private:
    using iterator = typename std::unordered_map<std::string, Node>::iterator;
    using const_iterator = typename std::unordered_map<std::string, Node>::const_iterator;

    [[nodiscard]]
    constexpr bool is_exist(const std::string& key) const noexcept
    {
        return values_.contains(key);
    }

    void add_new_node(Node* node) noexcept
    {
        node->prev = &head_;
        node->next = head_.next;

        head_.next->prev = node;
        head_.next = node;
    }

    void remove_node(Node* node) noexcept
    {
        node->prev->next = node->next;
        node->next->prev = node->prev;

        node->prev = node->next = nullptr;
    }

    void move_to_head(Node* node) noexcept
    {
        remove_node(node);
        add_new_node(node);
    }

    Node* pop_last() noexcept
    {
        auto* last = tail_.prev;
        remove_node(last);
        return last;
    }

    void remove_last_value() noexcept
    {
        auto last = pop_last();
        values_.erase(last->key);
    }

    constexpr bool is_full() const noexcept
    {
        return values_.size() > capacity_;
    }
};

} // namespace sb

#endif // SKY_BIG_CACHE_H