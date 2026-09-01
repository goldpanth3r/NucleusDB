#include <gtest/gtest.h>

#include "lexer/lexer.h"
#include "parser/parser.h"

using namespace nucleusdb;

// Parses SELECT * with a single table and no WHERE clause.
TEST(ParserTest, ParsesSelectAll) {
    Lexer lexer("SELECT * FROM users;");
    auto tokens = lexer.tokenize();
    Parser parser(tokens);

    auto statement = parser.parseSelect();

    // One expression: the star
    ASSERT_EQ(statement.expressions.size(), 1);
    EXPECT_EQ(statement.expressions[0].type, ExpressionType::STAR);
    EXPECT_EQ(statement.expressions[0].value, "*");

    EXPECT_EQ(statement.tableName, "users");
    EXPECT_FALSE(statement.where.has_value());
}

// Parses a SELECT with multiple columns and no WHERE clause.
TEST(ParserTest, ParsesMultipleColumns) {
    Lexer lexer("SELECT name, age FROM users;");
    auto tokens = lexer.tokenize();
    Parser parser(tokens);

    auto statement = parser.parseSelect();

    // Select list contains two columns
    ASSERT_EQ(statement.expressions.size(), 2);
    EXPECT_EQ(statement.expressions[0].value, "name");
    EXPECT_EQ(statement.expressions[1].value, "age");

    EXPECT_EQ(statement.tableName, "users");
    EXPECT_FALSE(statement.where.has_value());
}

// Parses a SELECT with multiple columns and a WHERE condition.
TEST(ParserTest, ParsesWhereClause) {
    Lexer lexer("SELECT name, age FROM users WHERE age >= 18;");
    auto tokens = lexer.tokenize();
    Parser parser(tokens);

    auto statement = parser.parseSelect();

    // Select list contains two columns
    ASSERT_EQ(statement.expressions.size(), 2);
    EXPECT_EQ(statement.expressions[0].value, "name");
    EXPECT_EQ(statement.expressions[1].value, "age");

    // WHERE clause is present
    ASSERT_TRUE(statement.where.has_value());
    EXPECT_EQ(statement.where->column, "age");
    EXPECT_EQ(statement.where->operatorSymbol, ">=");
    EXPECT_EQ(statement.where->value.value, "18");
}