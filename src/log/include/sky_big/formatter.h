#ifndef SKY_BIG_LOG_FORMATTER_H
#define SKY_BIG_LOG_FORMATTER_H

#include "entry.h"

namespace sb::log {

struct Formatter {
    Formatter() = default;

    Formatter(const Formatter& other) = delete;
    Formatter& operator=(const Formatter& other) = delete;

    Formatter(Formatter&& other) noexcept = default;
    Formatter& operator=(Formatter&& other) noexcept = default;

    virtual ~Formatter() = default;

    virtual std::string format(const Entry& entry) const = 0;
};

} // namespace sb::log

#endif // !SKY_BIG_LOG_FORMATTER_H