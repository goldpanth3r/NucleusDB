#pragma once

#include "parser/ast.h"
#include "storage/database.h"

namespace nucleusdb {

// Executor executes SQL statements against a database.
class Executor {
public:
    // Creates an executor for the given database.
    explicit Executor(Database& database);

    // Executes a CREATE TABLE statement.
    void execute(const CreateTableStatement& statement);

private:
    Database& database_;
};

}
