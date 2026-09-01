#pragma once

#include <string>

namespace nucleusdb {

// Types of expressions.
enum class ExpressionType {
    INTEGER,
    IDENTIFIER,
    STRING
};

// A SQL expression.
struct Expression {
    ExpressionType type;
    std::string value;
};

// Represents a SELECT statement.
struct SelectStatement {
    Expression expression;
    std::string tableName;

    std::string whereColumn;
    std::string whereOperator;
    Expression whereValue;};

}