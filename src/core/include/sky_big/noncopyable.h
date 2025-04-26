#ifndef SKY_BIG_NONCOPYABLE_H
#define SKY_BIG_NONCOPYABLE_H

namespace sb {

class Noncopyable {
public:
    Noncopyable(Noncopyable&&) noexcept = default;
    Noncopyable& operator=(Noncopyable&&) noexcept = default;

    Noncopyable(const Noncopyable&) = delete;
    Noncopyable& operator=(const Noncopyable&) = delete;

protected:
    Noncopyable() = default;

    // 不能用Noncopyable的指针
    ~Noncopyable() = default;
};

} // namespace sb

#endif // !SKY_BIG_NONCOPYABLE_H