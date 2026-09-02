#include <gtest/gtest.h>

#include "engine/engine.h"

using namespace nucleusdb;

// Executes CREATE TABLE through the complete SQL pipeline.
TEST(EngineTest, ExecutesCreateTable) {
    Engine engine;

    engine.execute(
        "CREATE TABLE users (id, name);");

    EXPECT_TRUE(engine.database().hasTable("users"));

    const Table& table =
        engine.database().getTable("users");

    ASSERT_EQ(table.columns().size(), 2);
    EXPECT_EQ(table.columns()[0], "id");
    EXPECT_EQ(table.columns()[1], "name");
}

// Executes INSERT through the complete SQL pipeline.
TEST(EngineTest, ExecutesInsert) {
    Engine engine;

    engine.execute(
        "CREATE TABLE users (id, name);");

    engine.execute(
        "INSERT INTO users VALUES (1, 'Alice');");

    const Table& table =
        engine.database().getTable("users");

    ASSERT_EQ(table.rows().size(), 1);
    EXPECT_EQ(table.rows()[0][0], "1");
    EXPECT_EQ(table.rows()[0][1], "Alice");
}

// Executes multiple INSERT statements.
TEST(EngineTest, ExecutesMultipleInserts) {
    Engine engine;

    engine.execute(
        "CREATE TABLE users (id, name);");

    engine.execute(
        "INSERT INTO users VALUES (1, 'Alice');");

    engine.execute(
        "INSERT INTO users VALUES (2, 'Bob');");

    const Table& table =
        engine.database().getTable("users");

    ASSERT_EQ(table.rows().size(), 2);

    EXPECT_EQ(table.rows()[0][0], "1");
    EXPECT_EQ(table.rows()[0][1], "Alice");

    EXPECT_EQ(table.rows()[1][0], "2");
    EXPECT_EQ(table.rows()[1][1], "Bob");
}