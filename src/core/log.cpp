#include "log.h"
#include "spdlog_logger.h"

#include <memory>
#include <string_view>

using std::make_unique;
using std::string_view;

namespace sb {

std::unique_ptr<Logger> create_logger(const string_view name) noexcept
{
    auto logger = make_unique<SpdLogLogger>(name);
    logger->pattern("%Y-%m-%d %T.%e.%f %n %^[%l]%$ %t %P %v");
    logger->level(LogLevel::Debug);
    return logger;
}

Logger* default_logger() noexcept
{
    static auto logger = create_logger("sky-big");
    return logger.get();
}

} // namespace sb