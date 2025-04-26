#include "simple_formatter.h"

#include <fmt/core.h>
#include <fmt/chrono.h>
#include <fmt/color.h>

#include <sstream>
#include <filesystem>

namespace sb::log {

struct MessageItem: public SimpleFormatter::Item {
    ~MessageItem() = default;

    std::string format(const Entry& entry) override
    {
        return entry.message;
    }
};

struct ThreadIDItem: public SimpleFormatter::Item {
    ~ThreadIDItem() = default;

    std::string format(const Entry& entry) override
    {
        std::stringstream result{};
        result << entry.thread_id;
        return result.str();
    }
};

struct ProcessIDItem: public SimpleFormatter::Item {
    ~ProcessIDItem() = default;

    std::string format(const Entry& entry) override
    {
        std::stringstream result{};
        result << entry.process_id;
        return result.str();
    }
};

struct LoggerNameItem: public SimpleFormatter::Item {
    ~LoggerNameItem() = default;

    std::string format(const Entry& entry) override
    {
        return entry.logger_name;
    }
};

struct LevelItem: public SimpleFormatter::Item {
    ~LevelItem() = default;

    std::string format(const Entry& entry) override
    {
        switch (entry.level) {
        using enum LogLevel;
        case Fatal:
            return fmt::format(fmt::emphasis::bold | fmt::fg(fmt::color::orange_red), "fatal");
        case Error:
            return fmt::format(fmt::emphasis::bold | fmt::fg(fmt::color::red), "error");
        case Warning:
            return fmt::format(fmt::emphasis::bold | fmt::fg(fmt::color::yellow), "warning");
        case Debug:
            return fmt::format(fmt::emphasis::bold | fmt::fg(fmt::color::green), "debug");
        case Info:
            return fmt::format(fmt::emphasis::bold | fmt::fg(fmt::color::blue), "info");
        case Trace:
            return fmt::format(fmt::emphasis::bold | fmt::fg(fmt::color::cyan), "trace");
        default:
            return fmt::format(fmt::emphasis::bold | fmt::fg(fmt::color::purple), "unkonwn level");
        }
    }
};

struct OccurredTimeItem: public SimpleFormatter::Item {
    ~OccurredTimeItem() = default;

    std::string format(const Entry& entry) override
    {
        return fmt::format("{:%Y/%m/%d %H:%M:%S}", fmt::localtime(entry.occurred_time));
    }
};

struct SourceFileBaseNameItem: public SimpleFormatter::Item {
    ~SourceFileBaseNameItem() = default;

    std::string format(const Entry& entry) override
    {
        std::filesystem::path full_path{ entry.source_location.file_name() };
        return full_path.filename().string();
    }
};

struct SourceFileFullPathItem: public SimpleFormatter::Item {
    ~SourceFileFullPathItem() = default;

    std::string format(const Entry& entry) override
    {
        return entry.source_location.file_name();
    }
};

struct SourceLineItem: public SimpleFormatter::Item {
    ~SourceLineItem() = default;

    std::string format(const Entry& entry) override
    {
        return fmt::to_string(entry.source_location.line());
    }
};

struct SourceFunctionItem: public SimpleFormatter::Item {
    ~SourceFunctionItem() = default;

    std::string format(const Entry& entry) override
    {
        return entry.source_location.function_name();
    }
};


SimpleFormatter::SimpleFormatter(std::string_view pattern)
  : pattern_{ pattern }
{
    items_.insert_or_assign(Message, std::make_unique<MessageItem>());
    items_.insert_or_assign(ThreadID, std::make_unique<ThreadIDItem>());
    items_.insert_or_assign(ProcessID, std::make_unique<ProcessIDItem>());
    items_.insert_or_assign(LoggerName, std::make_unique<LoggerNameItem>());
    items_.insert_or_assign(Level, std::make_unique<LevelItem>());
    items_.insert_or_assign(OccurredTime, std::make_unique<OccurredTimeItem>());
    items_.insert_or_assign(SourceFileBaseName, std::make_unique<SourceFileBaseNameItem>());
    items_.insert_or_assign(SourceFileFullPath, std::make_unique<SourceFileFullPathItem>());
    items_.insert_or_assign(SourceLine, std::make_unique<SourceLineItem>());
    items_.insert_or_assign(SourceFunction, std::make_unique<SourceFunctionItem>());
}

std::string SimpleFormatter::format(const Entry& entry) const
{
    std::stringstream result{};

    for (auto beg = pattern_.begin(); beg != pattern_.end(); ++beg) {
        if (*beg == Leader) {
            ++beg;
            result << convert(*beg, entry);
            continue;
        }

        result << *beg;
    }

    return result.str();
}


std::string SimpleFormatter::convert(char c, const Entry& entry) const
{
    if (items_.contains(c))
        return items_.at(c)->format(entry);

    return "[unknown pattern flag]";
}

} // namespace sb::log
