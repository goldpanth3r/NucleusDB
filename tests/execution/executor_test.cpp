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

// Executes an INSERT statement.
TEST(ExecutorTest, InsertsRow) {
    Database database;
    Executor executor(database);

    executor.execute(CreateTableStatement{
        "users",
        {"id", "name"}
    });

    executor.execute(InsertStatement{
        "users",
        {},
        {
            Expression{ExpressionType::INTEGER, "1", std::nullopt},
            Expression{ExpressionType::STRING, "Alice", std::nullopt}
        }
    });

    const Table& table = database.getTable("users");

    ASSERT_EQ(table.rows().size(), 1);
    EXPECT_EQ(table.rows()[0][0], "1");
    EXPECT_EQ(table.rows()[0][1], "Alice");
}

// Rejects an INSERT into a missing table.
TEST(ExecutorTest, RejectsMissingTable) {
    Database database;
    Executor executor(database);

    InsertStatement statement{
        "users",
        {},
        {
            Expression{ExpressionType::INTEGER, "1", std::nullopt},
            Expression{ExpressionType::STRING, "Alice", std::nullopt}
        }
    };

    EXPECT_THROW(
        executor.execute(statement),
        std::runtime_error);
}

// Rejects an INSERT with the wrong number of values.
TEST(ExecutorTest, RejectsWrongValueCount) {
    Database database;
    Executor executor(database);

    executor.execute(CreateTableStatement{
        "users",
        {"id", "name"}
    });

    InsertStatement statement{
        "users",
        {},
        {
            Expression{ExpressionType::INTEGER, "1", std::nullopt}
        }
    };

    EXPECT_THROW(
        executor.execute(statement),
        std::runtime_error);
}