#include "storage/table.h"

namespace nucleusdb {

// Creates a table with the given column names.
Table::Table(const std::vector<std::string>& columns)
    : columns_(columns) {}

// Returns the column names.
const std::vector<std::string>& Table::columns() const {
    return columns_;
}

// Inserts a row into the table.
void Table::insert(const Row& row) {
    rows_.push_back(row);
}

// Returns all rows.
const std::vector<Row>& Table::rows() const {
    return rows_;
}

}