#include "console_appender.h"

#include <fmt/base.h>

using fmt::println;

namespace sb::log {

ConsoleAppender::ConsoleAppender(std::unique_ptr<Formatter> formatter)
  : FormattableAppender{ std::move(formatter) }
{}

void ConsoleAppender::append(const Entry& entry)
{
    println("{}", format(entry));
}

} // namespace sb::log
