#ifndef BIG_SKY_LOG_ENTRY_H
#define BIG_SKY_LOG_ENTRY_H

#include "sky_big/log.h"

#include <string>
#include <source_location>
#include <thread>
#include <ctime>

namespace sb::log {

struct Entry {
    LogLevel level;
    std::string message;
    std::string logger_name;
    std::uint64_t process_id = 0;
    std::thread::id thread_id = std::this_thread::get_id();
    std::time_t occurred_time = std::time(nullptr);
    std::source_location source_location;

    Entry(const LogLevel level, const std::string_view message)
      : level{ level }, message{ message }
    {}
};

} // sb::log

#endif // BIG_SKY_LOG_ENTRY_H