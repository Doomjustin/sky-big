#ifndef SKY_BIG_LOG_SIMPLE_FORMATTER_H
#define SKY_BIG_LOG_SIMPLE_FORMATTER_H

#include "formatter.h"

#include <map>
#include <memory>

namespace sb::log {

/**
  * %v The actual text to log
  * %t Thread id
  * %P Process id
  * %n Logger's name
  * %l The log level of the message
  * %c Date and time representation
  * %s Basename of the source file  my_file.cpp
  * %g Full or relative path of the source file /some/dir/my_file.cpp
  * %# Source line
  * %! Source function
 */
class SimpleFormatter: public Formatter {
public:
    explicit SimpleFormatter(std::string_view pattern);

    SimpleFormatter(const SimpleFormatter& other) = delete;
    SimpleFormatter& operator=(const SimpleFormatter& other) = delete;

    SimpleFormatter(SimpleFormatter&& other) noexcept = default;
    SimpleFormatter& operator=(SimpleFormatter&& other) noexcept = default;

    ~SimpleFormatter() = default;

    std::string format(const Entry& entry) const override;

    struct Item {
        virtual ~Item() = default;

        virtual std::string format(const Entry& entry) = 0;
    };

private:
    static constexpr char Leader = '%';
    static constexpr char Message = 'v';
    static constexpr char ThreadID = 't';
    static constexpr char ProcessID = 'P';
    static constexpr char LoggerName = 'n';
    static constexpr char Level = 'l';
    static constexpr char OccurredTime = 'c';
    static constexpr char SourceFileBaseName = 's';
    static constexpr char SourceFileFullPath = 'g';
    static constexpr char SourceLine = '#';
    static constexpr char SourceFunction = '!';

    std::string pattern_;
    std::map<char, std::unique_ptr<Item>> items_{};

    std::string convert(char c, const Entry& entry) const;
};

} // namespace sb::log

#endif // SKY_BIG_LOG_SIMPLE_FORMATTER_H