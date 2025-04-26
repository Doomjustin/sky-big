#ifndef SKY_BIG_LOG_FORMATTABLE_APPENDER_H
#define SKY_BIG_LOG_FORMATTABLE_APPENDER_H

#include "appender.h"
#include "formatter.h"

#include <memory>

namespace sb::log {

class FormattableAppender: public Appender {
private:
    std::unique_ptr<Formatter> formatter_;
    std::string pattern_;

public:
    FormattableAppender(std::unique_ptr<Formatter> formatter);

    FormattableAppender(const FormattableAppender& other) = delete;
    FormattableAppender& operator=(const FormattableAppender& other) = delete;

    FormattableAppender(FormattableAppender&& other) noexcept = default;
    FormattableAppender& operator=(FormattableAppender&& other) noexcept = default;

    ~FormattableAppender() = default;

    void pattern(std::string_view value);

protected:
    std::string format(const Entry& entry) const;
};

} // namespace sb::log

#endif // !SKY_BIG_LOG_FORMATTABLE_APPENDER_H