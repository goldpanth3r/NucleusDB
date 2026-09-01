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

// Represents a WHERE condition.
struct Condition {
    std::string column;
    std::string operatorSymbol;
    Expression value;
};

// Represents a SELECT statement.
struct SelectStatement {
    Expression expression;
    std::string tableName;
    Condition where;
};
}