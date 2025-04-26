#ifndef SKY_BIG_LEVELDB_LEVELDB_H
#define SKY_BIG_LEVELDB_LEVELDB_H

#include "write_batch.h"

#include <optional>
#include <string>
#include <string_view>

namespace sb::leveldb {

class LevelDB {
private:
    std::string name_;

public:
    LevelDB(std::string name);

    LevelDB(const LevelDB&) = delete;
    LevelDB& operator=(const LevelDB&) = delete;

    LevelDB(LevelDB&&) noexcept = default;
    LevelDB& operator=(LevelDB&&) noexcept = default;

    ~LevelDB();

    void put(const std::string_view key, const std::string_view value);

    void remove(const std::string_view key);

    std::optional<std::string> get(const std::string_view key) const;

    void write(const WriteBatch& batch);
};

} // namespace sb::leveldb

#endif // !SKY_BIG_LEVELDB_LEVELDB_H