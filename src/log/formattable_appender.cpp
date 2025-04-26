#include "formattable_appender.h"

namespace sb::log {

FormattableAppender::FormattableAppender(std::unique_ptr<Formatter> formatter)
  : formatter_{ std::move(formatter) }
{}

void FormattableAppender::pattern(std::string_view value)
{
    pattern_ = value;
}

std::string FormattableAppender::format(const Entry& entry) const
{
    return formatter_->format(entry);
}

} // namespace sb::log
