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

// Parse addition.
TEST(ParserTest, ParsesAddition) {
    Lexer lexer("SELECT age + 1 FROM users;");

    auto tokens = lexer.tokenize();
    Parser parser(tokens);

    auto statement = parser.parseSelect();

    ASSERT_EQ(statement.expressions.size(), 1);

    const auto& expression = statement.expressions[0];

    EXPECT_EQ(expression.type, ExpressionType::BINARY);
    EXPECT_EQ(expression.value, "+");

    ASSERT_TRUE(expression.children.has_value());

    const auto& children = expression.children.value();

    ASSERT_EQ(children.size(), 2);

    EXPECT_EQ(children[0].type, ExpressionType::IDENTIFIER);
    EXPECT_EQ(children[0].value, "age");

    EXPECT_EQ(children[1].type, ExpressionType::INTEGER);
    EXPECT_EQ(children[1].value, "1");
}

// Parse subtraction.
TEST(ParserTest, ParsesSubtraction) {
    Lexer lexer("SELECT age - 1 FROM users;");

    auto tokens = lexer.tokenize();
    Parser parser(tokens);

    auto statement = parser.parseSelect();

    const auto& expression = statement.expressions[0];

    EXPECT_EQ(expression.type, ExpressionType::BINARY);
    EXPECT_EQ(expression.value, "-");

    ASSERT_TRUE(expression.children.has_value());

    const auto& children = expression.children.value();

    EXPECT_EQ(children[0].value, "age");
    EXPECT_EQ(children[1].value, "1");
}

// Parse multiplication.
TEST(ParserTest, ParsesMultiplication) {
    Lexer lexer("SELECT age * 2 FROM users;");

    auto tokens = lexer.tokenize();
    Parser parser(tokens);

    auto statement = parser.parseSelect();

    const auto& expression = statement.expressions[0];

    EXPECT_EQ(expression.type, ExpressionType::BINARY);
    EXPECT_EQ(expression.value, "*");

    ASSERT_TRUE(expression.children.has_value());

    const auto& children = expression.children.value();

    EXPECT_EQ(children[0].value, "age");
    EXPECT_EQ(children[1].value, "2");
}

// Parse division.
TEST(ParserTest, ParsesDivision) {
    Lexer lexer("SELECT age / 2 FROM users;");

    auto tokens = lexer.tokenize();
    Parser parser(tokens);

    auto statement = parser.parseSelect();

    const auto& expression = statement.expressions[0];

    EXPECT_EQ(expression.type, ExpressionType::BINARY);
    EXPECT_EQ(expression.value, "/");

    ASSERT_TRUE(expression.children.has_value());

    const auto& children = expression.children.value();

    EXPECT_EQ(children[0].value, "age");
    EXPECT_EQ(children[1].value, "2");
}

// * and / have higher precedence than + and -.
TEST(ParserTest, RespectsOperatorPrecedence) {
    Lexer lexer("SELECT age + 1 * 2 FROM users;");

    auto tokens = lexer.tokenize();
    Parser parser(tokens);

    auto statement = parser.parseSelect();

    const auto& expression = statement.expressions[0];

    EXPECT_EQ(expression.type, ExpressionType::BINARY);
    EXPECT_EQ(expression.value, "+");

    ASSERT_TRUE(expression.children.has_value());

    const auto& children = expression.children.value();

    EXPECT_EQ(children[0].value, "age");

    EXPECT_EQ(children[1].type, ExpressionType::BINARY);
    EXPECT_EQ(children[1].value, "*");

    ASSERT_TRUE(children[1].children.has_value());

    const auto& multiplication = children[1].children.value();

    EXPECT_EQ(multiplication[0].value, "1");
    EXPECT_EQ(multiplication[1].value, "2");
}

// Parentheses override operator precedence.
TEST(ParserTest, ParsesParentheses) {
    Lexer lexer("SELECT (age + 1) * 2 FROM users;");

    auto tokens = lexer.tokenize();
    Parser parser(tokens);

    auto statement = parser.parseSelect();

    const auto& expression = statement.expressions[0];

    EXPECT_EQ(expression.type, ExpressionType::BINARY);
    EXPECT_EQ(expression.value, "*");

    ASSERT_TRUE(expression.children.has_value());

    const auto& children = expression.children.value();

    EXPECT_EQ(children[0].type, ExpressionType::BINARY);
    EXPECT_EQ(children[0].value, "+");

    EXPECT_EQ(children[1].type, ExpressionType::INTEGER);
    EXPECT_EQ(children[1].value, "2");
}

// Parse unary minus.
TEST(ParserTest, ParsesUnaryMinus) {
    Lexer lexer("SELECT -age FROM users;");

    auto tokens = lexer.tokenize();
    Parser parser(tokens);

    auto statement = parser.parseSelect();

    ASSERT_EQ(statement.expressions.size(), 1);

    const auto& expression = statement.expressions[0];

    EXPECT_EQ(expression.type, ExpressionType::UNARY);
    EXPECT_EQ(expression.value, "-");

    ASSERT_TRUE(expression.children.has_value());

    const auto& children = expression.children.value();

    ASSERT_EQ(children.size(), 1);

    EXPECT_EQ(children[0].type, ExpressionType::IDENTIFIER);
    EXPECT_EQ(children[0].value, "age");
}

// Reject missing right-hand side.
TEST(ParserTest, RejectsMissingArithmeticOperand) {
    Lexer lexer("SELECT age + FROM users;");

    auto tokens = lexer.tokenize();
    Parser parser(tokens);

    EXPECT_THROW(parser.parseSelect(), std::runtime_error);
}

// Reject missing expression after an operator.
TEST(ParserTest, RejectsMissingMultiplicationOperand) {
    Lexer lexer("SELECT age * FROM users;");

    auto tokens = lexer.tokenize();
    Parser parser(tokens);

    EXPECT_THROW(parser.parseSelect(), std::runtime_error);
}

// Reject unmatched opening parenthesis.
TEST(ParserTest, RejectsUnclosedParenthesis) {
    Lexer lexer("SELECT (age + 1 FROM users;");

    auto tokens = lexer.tokenize();
    Parser parser(tokens);

    EXPECT_THROW(parser.parseSelect(), std::runtime_error);
}

// Reject unmatched closing parenthesis.
TEST(ParserTest, RejectsUnexpectedClosingParenthesis) {
    Lexer lexer("SELECT age + 1) FROM users;");

    auto tokens = lexer.tokenize();
    Parser parser(tokens);

    EXPECT_THROW(parser.parseSelect(), std::runtime_error);
}