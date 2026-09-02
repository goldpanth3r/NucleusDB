#pragma once

#include <string>

#include "execution/executor.h"
#include "storage/database.h"

namespace nucleusdb {

// Engine processes SQL statements against a database.
class Engine {
public:
    // Creates an engine with an empty database.
    Engine();

    // Executes a SQL statement.
    void execute(const std::string& sql);

    // Returns the database.
    Database& database();

private:
    Database database_;
};

} 