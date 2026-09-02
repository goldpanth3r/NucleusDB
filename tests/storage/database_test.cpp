#include <gtest/gtest.h>

#include "storage/database.h"

using namespace nucleusdb;

// Creates a table in the database.
TEST(DatabaseTest, CreatesTable) {
    Database database;

    database.createTable("users", {"id", "name"});

    EXPECT_TRUE(database.hasTable("users"));

    const Table& table = database.getTable("users");

    ASSERT_EQ(table.columns().size(), 2);
    EXPECT_EQ(table.columns()[0], "id");
    EXPECT_EQ(table.columns()[1], "name");
}

// Stores multiple tables in the database.
TEST(DatabaseTest, StoresMultipleTables) {
    Database database;

    database.createTable("users", {"id", "name"});
    database.createTable("posts", {"id", "title"});

    EXPECT_TRUE(database.hasTable("users"));
    EXPECT_TRUE(database.hasTable("posts"));
}

// Returns false for a missing table.
TEST(DatabaseTest, MissingTableReturnsFalse) {
    Database database;

    EXPECT_FALSE(database.hasTable("users"));
}

// Rejects duplicate table names.
TEST(DatabaseTest, RejectsDuplicateTable) {
    Database database;

    database.createTable("users", {"id", "name"});

    EXPECT_THROW(
        database.createTable("users", {"id", "email"}),
        std::runtime_error);
}

// Rejects access to a missing table.
TEST(DatabaseTest, RejectsMissingTable) {
    Database database;

    EXPECT_THROW(
        database.getTable("users"),
        std::runtime_error);
}

// Allows inserting rows through the database.
TEST(DatabaseTest, InsertsRowIntoTable) {
    Database database;

    database.createTable("users", {"id", "name"});

    database.getTable("users").insert({"1", "Alice"});

    const Table& table = database.getTable("users");

    ASSERT_EQ(table.rows().size(), 1);
    EXPECT_EQ(table.rows()[0][0], "1");
    EXPECT_EQ(table.rows()[0][1], "Alice");
}