#pragma once

#include <string>
#include <vector>

namespace nucleusdb {

// Row represents one record in a table.
using Row = std::vector<std::string>;

// Table stores a schema and its rows.
class Table {
public:
    // Creates a table with the given column names.
    explicit Table(const std::vector<std::string>& columns);

    // Returns the column names.
    const std::vector<std::string>& columns() const;

    // Inserts a row into the table.
    void insert(const Row& row);

    // Returns all rows.
    const std::vector<Row>& rows() const;

private:
    std::vector<std::string> columns_;
    std::vector<Row> rows_;
};

}