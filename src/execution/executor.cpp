#include "execution/executor.h"

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

}