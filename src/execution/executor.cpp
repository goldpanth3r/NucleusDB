#include "execution/executor.h"

#include <stdexcept>

namespace nucleusdb {

// Creates an executor for the given database.
Executor::Executor(Database& database)
    : database_(database) {}

// Executes a CREATE TABLE statement.
void Executor::execute(const CreateTableStatement& statement) {
    database_.createTable(
        statement.tableName,
        statement.columns);
}

// Executes an INSERT statement.
void Executor::execute(const InsertStatement& statement) {
    Table& table = database_.getTable(statement.tableName);

    if (!statement.columns.empty()) {
        if (statement.columns.size() != statement.values.size()) {
            throw std::runtime_error(
                "Column count does not match value count");
        }
    }

    if (statement.values.size() != table.columns().size()) {
        throw std::runtime_error(
            "Value count does not match table column count");
    }

    Row row;

    for (const Expression& expression : statement.values) {
        row.push_back(expression.value);
    }

    table.insert(row);
}

}