#include "write_batch.h"
#include "coding.h"

#include <fmt/format.h>
#include <gsl/gsl>

#include <ostream>

using std::underlying_type_t;

namespace sb::leveldb {

Record::Record(const std::string_view key)
  : type_{ Type::Deletion },
    key_{ std::move(key) }
{}

Record::Record(const std::string_view key, const std::string_view value)
  : type_{ Type::Value },
    key_{ std::move(key) },
    value_{ std::move(value) }
{}

std::ostream& Record::encode(std::ostream& os) const noexcept
{
    encode_fixed(os, fmt::underlying(type_));

    encode_varint(os, key_.size());
    os << key_;

    if (type_ == Type::Value) {
        encode_varint(os, value_.size());
        os << value_;
    }
    return os;
}

std::string read_string(std::istream& is) noexcept
{
    const auto size = decode_varint<std::size_t>(is);
    std::string result(size, ' ');
    is.read(result.data(), static_cast<std::streamsize>(size));

    return result;
}

Record Record::decode(std::istream& is) noexcept
{
    Record record{};
    record.type_ = static_cast<Type>(decode_fixed<underlying_type_t<Type>>(is));
    record.key_ = read_string(is);

    if (record.type_ == Type::Value)
        record.value_ = read_string(is);

    return record;
}

bool operator==(const Record& lhs, const Record& rhs) noexcept
{
    return lhs.type_ == rhs.type_ &&
           lhs.key_ == rhs.key_ &&
           lhs.value_ == rhs.value_;
}

std::ostream& operator<<(std::ostream& os, const Record& record)
{
    switch (record.type_) {
        case Record::Type::Value:
            os << fmt::format("[value]{}: {}", record.key_, record.value_);
            break;
        case Record::Type::Deletion:
            os << fmt::format("[deleted]{}", record.key_);
            break;
        }

    return os;
}

} // namespace sb::leveldb