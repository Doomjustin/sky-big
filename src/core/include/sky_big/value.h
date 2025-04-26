#ifndef SKI_BIG_VALUE_H
#define SKI_BIG_VALUE_H

#include "noncopyable.h"

#include <gsl/gsl>

#include <typeindex>

namespace sb {

class BaseValue: Noncopyable {
public:
    virtual ~BaseValue() = default;

    template<typename T>
    const T& get_value() const noexcept
    {
        Expects(type_ == typeid(T));
        return *static_cast<T*>(value_);
    }

    template<typename T>
    void set_value(T value) noexcept
    {
        Expects(type_ == typeid(T));
        *static_cast<T*>(value_) = value;
    }

    std::type_index get_type() const noexcept
    {
        return type_;
    }

protected:
    void* value_ = nullptr;
    std::type_index type_;

    template<typename T>
    explicit BaseValue(T)
      : value_{ nullptr },
        type_{ typeid(T) }
    {}
};


template<typename T>
struct Value: public BaseValue {
    T value;

    Value(T v)
      : BaseValue{ v },
        value{ std::move(v) }
    {
        value_ = &value;
    }

    ~Value() = default;
};

} // namespace sb

#endif // SKI_BIG_VALUE_H