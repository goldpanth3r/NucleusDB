#pragma once

#include <optional>
#include <string>
#include <vector>

namespace nucleusdb {

// ExpressionType enumerates the kinds of expressions supported in the select list.
enum class ExpressionType {
    INTEGER,
    IDENTIFIER,
    STRING,
    STAR
};

// Expression represents a single item in the SELECT list
// (literal, column name, or *).
struct Expression {
    ExpressionType type;
    std::string value;
};

// Condition represents a basic WHERE clause of the form:
//   column <operator> value
struct Condition {
    std::string column;
    std::string operatorSymbol;
    Expression value;
};

// SelectStatement holds the result of parsing a SELECT statement.
struct SelectStatement {
    std::vector<Expression> expressions;  // columns / expressions in the SELECT list
    std::string tableName;                // table specified in the FROM clause
    std::optional<Condition> where;       // optional WHERE condition
};

}