#include <gtest/gtest.h>

#include "storage/table.h"

using namespace nucleusdb;

// Creates a table with columns.
TEST(TableTest, CreatesTable) {
    Table table({"id", "name"});

    ASSERT_EQ(table.columns().size(), 2);

    EXPECT_EQ(table.columns()[0], "id");
    EXPECT_EQ(table.columns()[1], "name");

    EXPECT_TRUE(table.rows().empty());
}

// Inserts a row into a table.
TEST(TableTest, InsertsRow) {
    Table table({"id", "name"});

    table.insert({"1", "Alice"});

    ASSERT_EQ(table.rows().size(), 1);

    EXPECT_EQ(table.rows()[0][0], "1");
    EXPECT_EQ(table.rows()[0][1], "Alice");
}