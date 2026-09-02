#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "storage/table.h"

namespace nucleusdb {

// Database stores tables by name.
class Database {
public:
    // Creates a table with the given name and columns.
    void createTable(
        const std::string& name,
        const std::vector<std::string>& columns);

    // Returns true if a table exists.
    bool hasTable(const std::string& name) const;

    // Returns a table by name.
    Table& getTable(const std::string& name);

    // Returns a table by name.
    const Table& getTable(const std::string& name) const;

private:
    std::unordered_map<std::string, Table> tables_;
};

}