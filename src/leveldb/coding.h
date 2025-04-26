#ifndef SKY_BIG_LEVELDB_CODING_H
#define SKY_BIG_LEVELDB_CODING_H

#include <gsl/gsl>

#include <cstdint>
#include <ostream>
#include <istream>
#include <concepts>

namespace sb::leveldb {

namespace details {

constexpr int VarintByteSize = 7;

constexpr int NotLastByte = 1 << VarintByteSize; // 0b10000000

template<std::unsigned_integral T>
constexpr bool is_not_last_byte(T value)
{
    return value >= NotLastByte;
}

template<std::unsigned_integral T>
constexpr std::uint8_t get_varint_byte(T value)
{
    return gsl::narrow_cast<std::uint8_t>(value | NotLastByte);
}

} // namespace sb::leveldb::details


template<std::unsigned_integral T>
void encode_fixed(std::ostream& os, T value)
{
    os.write(reinterpret_cast<char*>(&value), sizeof(T));
}

template<std::unsigned_integral T>
T decode_fixed(std::istream& is)
{
    T result{};
    is.read(reinterpret_cast<char*>(&result), sizeof(T));

    return result;
}

template<std::unsigned_integral T>
void encode_varint(std::ostream& os, T value)
{
    std::uint8_t byte{};

    while (details::is_not_last_byte(value)) {
        byte = details::get_varint_byte(value);
        os.write(reinterpret_cast<char*>(&byte), 1);
        value >>= details::VarintByteSize;
    }

    os.write(reinterpret_cast<char*>(&value), 1);
}

template<std::unsigned_integral T>
T decode_varint(std::istream& is)
{
    T result{};
    T byte{};
    is.read(reinterpret_cast<char*>(&byte), 1);

    int shift = 0;
    while (details::is_not_last_byte(byte)) {
        constexpr int varint_byte_max = 0b01111111;

        result |= ((byte & varint_byte_max) << shift);
        is.read(reinterpret_cast<char*>(&byte), 1);
        shift += details::VarintByteSize;
    }

    result |= (byte << shift);
    return result;
}

} // namespace sb::leveldb

#endif // !SKY_BIG_LEVELDB_CODING_H