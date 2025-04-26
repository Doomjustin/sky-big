#ifndef SKY_BIG_LEVELDB_WRITE_BATCH_H
#define SKY_BIG_LEVELDB_WRITE_BATCH_H

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace sb::leveldb {

class Record {
friend bool operator==(const Record& lhs, const Record& rhs) noexcept;
friend std::ostream& operator<<(std::ostream& os, const Record& record);

private:
    enum class Type: std::uint8_t {
        Value,
        Deletion
    };

    Type type_;
    std::string key_;
    std::string value_;

public:
    explicit Record(const std::string_view key);

    Record(const std::string_view key, const std::string_view value);

    Record(const Record&) = default;
    Record& operator=(const Record&) = default;

    Record(Record&&) noexcept = default;
    Record& operator=(Record&&) noexcept = default;

    ~Record() = default;

    std::string_view key() const { return key_; }

    std::string_view value() const { return value_; }

    std::ostream& encode(std::ostream& os) const noexcept;

    static Record decode(std::istream& is) noexcept;

private:
    Record() = default;
};

bool operator==(const Record& lhs, const Record& rhs) noexcept;
std::ostream& operator<<(std::ostream& os, const Record& record);


class WriteBatch {
friend bool operator==(const WriteBatch& lhs, const WriteBatch& rhs) noexcept;
friend std::ostream& operator<<(std::ostream& os, const WriteBatch& batch);

public:
    using SequenceNumber = std::uint64_t;
    using SizeType = std::size_t;

private:
    SequenceNumber sequence_number_ = 0;
    std::vector<Record> records_;

public:
    void put(const std::string_view key, const std::string_view value)
    {
        records_.emplace_back(key, value);
    }

    void remove(const std::string_view key)
    {
        records_.emplace_back(key);
    }

    void clear()
    {
        records_.clear();
    }

    [[nodiscard]]
    constexpr SizeType size() const noexcept
    {
        return records_.size();
    }

    [[nodiscard]]
    constexpr SequenceNumber sequence_number() const noexcept
    {
        return sequence_number_;
    }

    void encode(std::ostream& os) const noexcept;

    WriteBatch& operator+=(const WriteBatch& other) noexcept;

    static WriteBatch decode(std::istream& is) noexcept;
};

bool operator==(const WriteBatch& lhs, const WriteBatch& rhs) noexcept;
WriteBatch operator+(WriteBatch& lhs, const WriteBatch& rhs) noexcept;
std::ostream& operator<<(std::ostream& os, const WriteBatch& batch);

} // namespace sb::leveldb

#endif // !SKY_BIG_LEVELDB_WRITE_BATCH_H