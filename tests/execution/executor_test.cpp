#include <gtest/gtest.h>

#include "execution/executor.h"

using namespace nucleusdb;

// Executes a CREATE TABLE statement.
TEST(ExecutorTest, CreatesTable) {
    Database database;
    Executor executor(database);

    CreateTableStatement statement{
        "users",
        {"id", "name"}
    };

    executor.execute(statement);

    EXPECT_TRUE(database.hasTable("users"));

    const Table& table = database.getTable("users");

    ASSERT_EQ(table.columns().size(), 2);
    EXPECT_EQ(table.columns()[0], "id");
    EXPECT_EQ(table.columns()[1], "name");
}

// Rejects duplicate tables.
TEST(ExecutorTest, RejectsDuplicateTable) {
    Database database;
    Executor executor(database);

    CreateTableStatement statement{
        "users",
        {"id", "name"}
    };

    executor.execute(statement);

    EXPECT_THROW(
        executor.execute(statement),
        std::runtime_error);
}