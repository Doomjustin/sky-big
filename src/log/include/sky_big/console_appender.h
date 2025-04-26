#ifndef SKY_BIG_LOG_CONSOLE_APPENDER_H
#define SKY_BIG_LOG_CONSOLE_APPENDER_H

#include "formattable_appender.h"

namespace sb::log {

class ConsoleAppender final: public FormattableAppender {
public:
    ConsoleAppender(std::unique_ptr<Formatter> formatter);

    ConsoleAppender(const ConsoleAppender& other) = delete;
    ConsoleAppender& operator=(const ConsoleAppender& other) = delete;

    ConsoleAppender(ConsoleAppender&& other) noexcept = default;
    ConsoleAppender& operator=(ConsoleAppender&& other) noexcept = default;

    ~ConsoleAppender() = default;

    void append(const Entry& entry) override;
};

} // namespace sb::log

#endif // !SKY_BIG_LOG_CONSOLE_APPENDER_H