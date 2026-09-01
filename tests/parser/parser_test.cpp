#include <gtest/gtest.h>

#include "lexer/lexer.h"
#include "parser/parser.h"

using namespace nucleusdb;

// Parse SELECT with an integer.
TEST(ParserTest, ParsesSelectInteger) {
    Lexer lexer("SELECT 42 FROM users WHERE age > 18;");

    auto tokens = lexer.tokenize();

    Parser parser(tokens);

    auto statement = parser.parseSelect();

    EXPECT_EQ(statement.expression.type, ExpressionType::INTEGER);
    EXPECT_EQ(statement.expression.value, "42");

    EXPECT_EQ(statement.tableName, "users");

    EXPECT_EQ(statement.whereColumn, "age");
    EXPECT_EQ(statement.whereOperator, ">");
    EXPECT_EQ(statement.whereValue.type, ExpressionType::INTEGER);
    EXPECT_EQ(statement.whereValue.value, "18");
}

// Parse SELECT with an identifier.
TEST(ParserTest, ParsesSelectIdentifier) {
    Lexer lexer("SELECT age FROM users WHERE age >= 18;");

    auto tokens = lexer.tokenize();

    Parser parser(tokens);

    auto statement = parser.parseSelect();

    EXPECT_EQ(statement.expression.type, ExpressionType::IDENTIFIER);
    EXPECT_EQ(statement.expression.value, "age");

    EXPECT_EQ(statement.tableName, "users");

    EXPECT_EQ(statement.whereColumn, "age");
    EXPECT_EQ(statement.whereOperator, ">=");
    EXPECT_EQ(statement.whereValue.type, ExpressionType::INTEGER);
    EXPECT_EQ(statement.whereValue.value, "18");
}

// Parse SELECT with a string condition.
TEST(ParserTest, ParsesSelectStringCondition) {
    Lexer lexer("SELECT name FROM users WHERE name = 'Alice';");

    auto tokens = lexer.tokenize();

    Parser parser(tokens);

    auto statement = parser.parseSelect();

    EXPECT_EQ(statement.expression.type, ExpressionType::IDENTIFIER);
    EXPECT_EQ(statement.expression.value, "name");

    EXPECT_EQ(statement.tableName, "users");

    EXPECT_EQ(statement.whereColumn, "name");
    EXPECT_EQ(statement.whereOperator, "=");
    EXPECT_EQ(statement.whereValue.type, ExpressionType::STRING);
    EXPECT_EQ(statement.whereValue.value, "Alice");
}