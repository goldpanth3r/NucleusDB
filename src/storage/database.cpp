#include "storage/database.h"

#include <stdexcept>

namespace nucleusdb {

// Creates a table with the given name and columns.
void Database::createTable(
    const std::string& name,
    const std::vector<std::string>& columns) {

    if (hasTable(name)) {
        throw std::runtime_error("Table already exists");
    }

    tables_.emplace(name, Table(columns));
}

// Returns true if a table exists.
bool Database::hasTable(const std::string& name) const {
    return tables_.find(name) != tables_.end();
}

// Returns a table by name.
Table& Database::getTable(const std::string& name) {
    auto it = tables_.find(name);

    if (it == tables_.end()) {
        throw std::runtime_error("Table not found");
    }

    return it->second;
}

// Returns a table by name.
const Table& Database::getTable(const std::string& name) const {
    auto it = tables_.find(name);

    if (it == tables_.end()) {
        throw std::runtime_error("Table not found");
    }

    return it->second;
}

}