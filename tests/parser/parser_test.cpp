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

// Reject SELECT without an expression.
TEST(ParserTest, RejectsMissingExpression) {
    Lexer lexer("SELECT FROM users;");

    auto tokens = lexer.tokenize();
    Parser parser(tokens);

    EXPECT_THROW(parser.parseSelect(), std::runtime_error);
}

// Handle SELECT without FROM.
TEST(ParserTest, RejectsMissingFrom) {
    Lexer lexer("SELECT age users;");

    auto tokens = lexer.tokenize();
    Parser parser(tokens);

    EXPECT_THROW(parser.parseSelect(), std::runtime_error);
}

// Handle FROM without a table.
TEST(ParserTest, RejectsMissingTable) {
    Lexer lexer("SELECT age FROM;");

    auto tokens = lexer.tokenize();
    Parser parser(tokens);

    EXPECT_THROW(parser.parseSelect(), std::runtime_error);
}

// Handle WHERE without a condition.
TEST(ParserTest, RejectsMissingWhereCondition) {
    Lexer lexer("SELECT age FROM users WHERE;");

    auto tokens = lexer.tokenize();
    Parser parser(tokens);

    EXPECT_THROW(parser.parseSelect(), std::runtime_error);
}

// Handle WHERE without an operator.
TEST(ParserTest, RejectsMissingWhereOperator) {
    Lexer lexer("SELECT age FROM users WHERE age;");

    auto tokens = lexer.tokenize();
    Parser parser(tokens);

    EXPECT_THROW(parser.parseSelect(), std::runtime_error);
}

// Handle WHERE without a value.
TEST(ParserTest, RejectsMissingWhereValue) {
    Lexer lexer("SELECT age FROM users WHERE age >;");

    auto tokens = lexer.tokenize();
    Parser parser(tokens);

    EXPECT_THROW(parser.parseSelect(), std::runtime_error);
}

// Handle trailing comma in SELECT list.
TEST(ParserTest, RejectsTrailingComma) {
    Lexer lexer("SELECT age, FROM users;");

    auto tokens = lexer.tokenize();
    Parser parser(tokens);

    EXPECT_THROW(parser.parseSelect(), std::runtime_error);
}