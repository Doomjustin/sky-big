#include "write_batch.h"
#include "coding.h"

#include <algorithm>
#include <vector>

namespace sb::leveldb {

void WriteBatch::encode(std::ostream& os) const noexcept
{
    encode_fixed(os, sequence_number());
    encode_varint(os, size());
    std::ranges::for_each(records_, [&os] (const auto& record) { record.encode(os); });
}

WriteBatch WriteBatch::decode(std::istream& is) noexcept
{
    WriteBatch batch{};
    batch.sequence_number_ = decode_fixed<SequenceNumber>(is);
    auto record_size = decode_varint<SizeType>(is);

    while (record_size-- > 0)
        batch.records_.push_back(Record::decode(is));

    return batch;
}

WriteBatch& WriteBatch::operator+=(const WriteBatch& other) noexcept
{
    std::ranges::copy(other.records_, std::back_inserter(records_));
    return *this;
}


bool operator==(const WriteBatch& lhs, const WriteBatch& rhs) noexcept
{
    if (lhs.sequence_number_ != rhs.sequence_number_)
        return false;

    if (lhs.size() != rhs.size())
        return false;

    return std::equal(lhs.records_.begin(), lhs.records_.end(), 
                      rhs.records_.begin());
}

WriteBatch operator+(WriteBatch& lhs, const WriteBatch& rhs) noexcept
{
    return lhs += rhs;
}

std::ostream& operator<<(std::ostream& os, const WriteBatch& batch)
{
    os << batch.sequence_number_ << '\n';
    std::ranges::copy(batch.records_, std::ostream_iterator<Record>(os, "\n"));
    return os;
}

} // namespace sb::leveldb