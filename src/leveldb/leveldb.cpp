#include "leveldb.h"

namespace sb::leveldb {

LevelDB::LevelDB(std::string name)
    : name_{std::move(name)}
{}

LevelDB::~LevelDB() = default;

void LevelDB::put(const std::string_view key, const std::string_view value)
{
    WriteBatch batch{};
    batch.put(key, value);
    write(batch);
}

void LevelDB::remove(const std::string_view key)
{
    WriteBatch batch{};
    batch.remove(key);
    write(batch);
}

std::optional<std::string> LevelDB::get(const std::string_view key) const
{
    return {};
}

void LevelDB::write(const WriteBatch& batch)
{
}

} // namespace sb::leveldb
