#ifndef SKY_BIG_LOG_SPDLOG_LOGGER_H
#define SKY_BIG_LOG_SPDLOG_LOGGER_H

#include "log.h"

#include <spdlog/common.h>
#include <spdlog/spdlog.h>

#include <string_view>
#include <vector>

namespace sb {

class SpdLogLogger: public Logger {
public:
    explicit SpdLogLogger(std::string_view name);

    ~SpdLogLogger() = default;

    void fatal(std::string_view message, std::source_location cur) override;

    void error(std::string_view message, std::source_location cur) override;

    void warning(std::string_view message, std::source_location cur) override;

    void info(std::string_view message, std::source_location cur) override;

    void debug(std::string_view message, std::source_location cur) override;

    void trace(std::string_view message, std::source_location cur) override;

    void pattern(std::string_view pattern) override;

    constexpr LogLevel level() const noexcept override;

    void level(LogLevel new_level) noexcept override;

private:
    std::shared_ptr<spdlog::logger> logger_;
    std::vector<spdlog::sink_ptr> sinks_;

    static std::string make_message(std::string_view message, std::source_location cur);
};

} // namespace sb

#endif // !SKY_BIG_LOG_SPDLOG_LOGGER_H