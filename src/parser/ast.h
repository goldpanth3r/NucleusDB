#pragma once

#include <optional>
#include <string>
#include <vector>

namespace nucleusdb {

    enum class StatementType {
    SELECT,
    INSERT
};


// ExpressionType enumerates the kinds of expressions.
enum class ExpressionType {
    INTEGER,
    IDENTIFIER,
    STRING,
    STAR,
    BINARY,
    UNARY

};

// Expression represents a SQL expression.
struct Expression {
    ExpressionType type;
    std::string value;
    std::optional<std::vector<Expression>> children;
};

// Condition represents:
// column <operator> value
struct Condition {
    std::string column;
    std::string operatorSymbol;
    Expression value;
};

// SelectStatement holds a SELECT statement.
struct SelectStatement {
    std::vector<Expression> expressions;
    std::string tableName;
    std::optional<Condition> where;
};


struct InsertStatement {
    std::string tableName;
    std::vector<Expression> values;
};

}