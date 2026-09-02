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

TEST(ParserTest, ParsesInsert) {
    Lexer lexer("INSERT INTO users VALUES (1, 'Alice');");

    auto tokens = lexer.tokenize();
    Parser parser(tokens);

    auto statement = parser.parseInsert();

    EXPECT_EQ(statement.tableName, "users");

    ASSERT_EQ(statement.values.size(), 2);

    EXPECT_EQ(statement.values[0].type, ExpressionType::INTEGER);
    EXPECT_EQ(statement.values[0].value, "1");

    EXPECT_EQ(statement.values[1].type, ExpressionType::STRING);
    EXPECT_EQ(statement.values[1].value, "Alice");
}

// Parse INSERT with explicit columns.
TEST(ParserTest, ParsesInsertWithColumns) {
    Lexer lexer(
        "INSERT INTO users (id, name) VALUES (1, 'Alice');"
    );

    auto tokens = lexer.tokenize();

    Parser parser(tokens);

    auto statement = parser.parseInsert();

    EXPECT_EQ(statement.tableName, "users");

    ASSERT_EQ(statement.columns.size(), 2);
    EXPECT_EQ(statement.columns[0], "id");
    EXPECT_EQ(statement.columns[1], "name");

    ASSERT_EQ(statement.values.size(), 2);

    EXPECT_EQ(statement.values[0].type, ExpressionType::INTEGER);
    EXPECT_EQ(statement.values[0].value, "1");

    EXPECT_EQ(statement.values[1].type, ExpressionType::STRING);
    EXPECT_EQ(statement.values[1].value, "Alice");
}

// Reject INSERT when there are fewer values than columns.
TEST(ParserTest, RejectsInsertColumnValueMismatch) {
    Lexer lexer(
        "INSERT INTO users (id, name) VALUES (1);"
    );

    auto tokens = lexer.tokenize();

    Parser parser(tokens);

    EXPECT_THROW(
        parser.parseInsert(),
        std::runtime_error
    );
}

// Reject INSERT when there are more values than columns.
TEST(ParserTest, RejectsInsertTooManyValues) {
    Lexer lexer(
        "INSERT INTO users (id) VALUES (1, 'Alice');"
    );

    auto tokens = lexer.tokenize();

    Parser parser(tokens);

    EXPECT_THROW(
        parser.parseInsert(),
        std::runtime_error
    );
}

// Reject INSERT without INTO.
TEST(ParserTest, RejectsInsertMissingInto) {
    Lexer lexer(
        "INSERT users VALUES (1);"
    );

    auto tokens = lexer.tokenize();

    Parser parser(tokens);

    EXPECT_THROW(
        parser.parseInsert(),
        std::runtime_error
    );
}

// Reject INSERT without table name.
TEST(ParserTest, RejectsInsertMissingTable) {
    Lexer lexer(
        "INSERT INTO VALUES (1);"
    );

    auto tokens = lexer.tokenize();

    Parser parser(tokens);

    EXPECT_THROW(
        parser.parseInsert(),
        std::runtime_error
    );
}

// Reject INSERT without VALUES.
TEST(ParserTest, RejectsInsertMissingValues) {
    Lexer lexer(
        "INSERT INTO users (id) (1);"
    );

    auto tokens = lexer.tokenize();

    Parser parser(tokens);

    EXPECT_THROW(
        parser.parseInsert(),
        std::runtime_error
    );
}

// Reject INSERT without opening parenthesis.
TEST(ParserTest, RejectsInsertMissingOpeningParenthesis) {
    Lexer lexer(
        "INSERT INTO users VALUES 1);"
    );

    auto tokens = lexer.tokenize();

    Parser parser(tokens);

    EXPECT_THROW(
        parser.parseInsert(),
        std::runtime_error
    );
}

// Reject INSERT without closing parenthesis.
TEST(ParserTest, RejectsInsertMissingClosingParenthesis) {
    Lexer lexer(
        "INSERT INTO users VALUES (1;"
    );

    auto tokens = lexer.tokenize();

    Parser parser(tokens);

    EXPECT_THROW(
        parser.parseInsert(),
        std::runtime_error
    );
}

// Reject INSERT without semicolon.
TEST(ParserTest, RejectsInsertMissingSemicolon) {
    Lexer lexer(
        "INSERT INTO users VALUES (1)"
    );

    auto tokens = lexer.tokenize();

    Parser parser(tokens);

    EXPECT_THROW(
        parser.parseInsert(),
        std::runtime_error
    );
}

// Reject INSERT with a trailing comma in values.
TEST(ParserTest, RejectsInsertTrailingValueComma) {
    Lexer lexer(
        "INSERT INTO users VALUES (1,);"
    );

    auto tokens = lexer.tokenize();

    Parser parser(tokens);

    EXPECT_THROW(
        parser.parseInsert(),
        std::runtime_error
    );
}

// Reject INSERT with a trailing comma in columns.
TEST(ParserTest, RejectsInsertTrailingColumnComma) {
    Lexer lexer(
        "INSERT INTO users (id, name,) VALUES (1, 'Alice');"
    );

    auto tokens = lexer.tokenize();

    Parser parser(tokens);

    EXPECT_THROW(
        parser.parseInsert(),
        std::runtime_error
    );
}

// Reject INSERT with empty values.
TEST(ParserTest, RejectsInsertEmptyValues) {
    Lexer lexer(
        "INSERT INTO users VALUES ();"
    );

    auto tokens = lexer.tokenize();

    Parser parser(tokens);

    EXPECT_THROW(
        parser.parseInsert(),
        std::runtime_error
    );
}

// Reject INSERT with empty column list.
TEST(ParserTest, RejectsInsertEmptyColumns) {
    Lexer lexer(
        "INSERT INTO users () VALUES (1);"
    );

    auto tokens = lexer.tokenize();

    Parser parser(tokens);

    EXPECT_THROW(
        parser.parseInsert(),
        std::runtime_error
    );

}

// Parse CREATE TABLE.
TEST(ParserTest, ParsesCreateTable) {
    Lexer lexer(
        "CREATE TABLE users (id, name);"
    );

    auto tokens = lexer.tokenize();
    Parser parser(tokens);

    auto statement = parser.parseCreateTable();

    EXPECT_EQ(statement.tableName, "users");

    ASSERT_EQ(statement.columns.size(), 2);
    EXPECT_EQ(statement.columns[0], "id");
    EXPECT_EQ(statement.columns[1], "name");
}

// Reject CREATE without TABLE.
TEST(ParserTest, RejectsCreateMissingTableKeyword) {
    Lexer lexer("CREATE users (id);");

    auto tokens = lexer.tokenize();
    Parser parser(tokens);

    EXPECT_THROW(parser.parseCreateTable(), std::runtime_error);
}

// Reject CREATE without table name.
TEST(ParserTest, RejectsCreateMissingTableName) {
    Lexer lexer("CREATE TABLE (id);");

    auto tokens = lexer.tokenize();
    Parser parser(tokens);

    EXPECT_THROW(parser.parseCreateTable(), std::runtime_error);
}

// Reject CREATE with empty column list.
TEST(ParserTest, RejectsCreateEmptyColumns) {
    Lexer lexer("CREATE TABLE users ();");

    auto tokens = lexer.tokenize();
    Parser parser(tokens);

    EXPECT_THROW(parser.parseCreateTable(), std::runtime_error);
}

// Reject CREATE with trailing comma.
TEST(ParserTest, RejectsCreateTrailingComma) {
    Lexer lexer("CREATE TABLE users (id, name,);");

    auto tokens = lexer.tokenize();
    Parser parser(tokens);

    EXPECT_THROW(parser.parseCreateTable(), std::runtime_error);
}

// Reject CREATE without semicolon.
TEST(ParserTest, RejectsCreateMissingSemicolon) {
    Lexer lexer("CREATE TABLE users (id, name)");

    auto tokens = lexer.tokenize();
    Parser parser(tokens);

    EXPECT_THROW(parser.parseCreateTable(), std::runtime_error);
}