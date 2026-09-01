#include "parser/parser.h"

#include <stdexcept>

namespace nucleusdb {

// Creates a parser from the given token list.
Parser::Parser(const std::vector<Token>& tokens)
    : tokens_(tokens) {}

// parseExpression converts a single token into an Expression.
// Supported types: INTEGER, IDENTIFIER, STRING, and STAR.
// Throws if the token cannot be interpreted as a valid expression.
static Expression parseExpression(const Token& token) {
    switch (token.type) {
        case TokenType::INTEGER:
            return {ExpressionType::INTEGER, token.lexeme};

        case TokenType::IDENTIFIER:
            return {ExpressionType::IDENTIFIER, token.lexeme};

        case TokenType::STRING:
            return {ExpressionType::STRING, token.lexeme};

        case TokenType::STAR:
            return {ExpressionType::STAR, token.lexeme};

        default:
            throw std::runtime_error("Expected expression");
    }
}

// parseSelect parses a SELECT statement of the form:
//   SELECT <expr> [, <expr>]* FROM <table> [WHERE <col> <op> <value>];
SelectStatement Parser::parseSelect() {
    // Consume SELECT keyword
    if (tokens_[position_].type != TokenType::SELECT) {
        throw std::runtime_error("Expected SELECT");
    }
    ++position_;

    // Parse the select list (at least one expression is required)
    std::vector<Expression> expressions;
    expressions.push_back(parseExpression(tokens_[position_]));
    ++position_;

    // Consume additional expressions separated by commas
    while (tokens_[position_].type == TokenType::COMMA) {
        ++position_;
        expressions.push_back(parseExpression(tokens_[position_]));
        ++position_;
    }

    // Consume FROM keyword
    if (tokens_[position_].type != TokenType::FROM) {
        throw std::runtime_error("Expected FROM");
    }
    ++position_;

    // Parse the table name
    if (tokens_[position_].type != TokenType::IDENTIFIER) {
        throw std::runtime_error("Expected table name");
    }
    std::string tableName = tokens_[position_].lexeme;
    ++position_;

    // Optional WHERE clause
    std::optional<Condition> where;
    if (tokens_[position_].type == TokenType::WHERE) {
        ++position_;

        // Column name
        if (tokens_[position_].type != TokenType::IDENTIFIER) {
            throw std::runtime_error("Expected WHERE column");
        }
        std::string column = tokens_[position_].lexeme;
        ++position_;

        // Comparison operator (=, <, <=, >, >=, !=)
        TokenType operatorType = tokens_[position_].type;
        if (operatorType != TokenType::EQUAL &&
            operatorType != TokenType::LESS &&
            operatorType != TokenType::LESS_EQUAL &&
            operatorType != TokenType::GREATER &&
            operatorType != TokenType::GREATER_EQUAL &&
            operatorType != TokenType::NOT_EQUAL) {
            throw std::runtime_error("Expected comparison operator");
        }
        std::string operatorSymbol = tokens_[position_].lexeme;
        ++position_;

        // Right-hand side value
        Expression value = parseExpression(tokens_[position_]);
        ++position_;

        where = Condition{column, operatorSymbol, value};
    }

    // Expect a terminating semicolon
    if (tokens_[position_].type != TokenType::SEMICOLON) {
        throw std::runtime_error("Expected semicolon");
    }

    return SelectStatement{expressions, tableName, where};
}

}