#ifndef SKY_BIG_UTILITY_H
#define SKY_BIG_UTILITY_H

#include <cctype>
#include <ranges>
#include <string>

namespace sb {

namespace details {

constexpr auto is_space = [] (const char c) { return std::isspace(c); };

} // namespace exercise::details


[[nodiscard("trims the output")]]
constexpr std::string trim_left(const std::string_view in) noexcept
{
    auto view = in | std::views::drop_while(details::is_space);
    return {view.begin(), view.end()};
}

[[nodiscard("trims the output")]]
constexpr std::string trim_right(const std::string_view in)
{
    auto view = in
              | std::views::reverse
              | std::views::drop_while(details::is_space)
              | std::views::reverse
              ;

    return { view.begin(), view.end() };
}

[[nodiscard("trims the output")]]
constexpr std::string trim(const std::string_view in)
{
    auto view = in
              | std::views::drop_while(details::is_space)
              | std::views::reverse
              | std::views::drop_while(details::is_space)
              | std::views::reverse
              ;

    return {view.begin(), view.end()};
}

[[nodiscard("to_upper output")]]
constexpr std::string to_upper(const std::string_view in)
{
    auto view = in | std::views::transform([] (const char c) { return std::toupper(c); });
    return { view.begin(), view.end() };
}

[[nodiscard("to_lower output")]]
constexpr std::string to_lower(const std::string_view in)
{
    auto view = in | std::views::transform([] (const char c) { return std::tolower(c); });
    return { view.begin(), view.end() };
}

} // namespace sb

#endif // !SKY_BIG_UTILITY_H