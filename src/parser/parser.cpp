#include "parser/parser.h"

#include <stdexcept>

namespace nucleusdb {

// Create a parser.
Parser::Parser(const std::vector<Token>& tokens)
    : tokens_(tokens) {}

// Parse a SELECT statement.
SelectStatement Parser::parseSelect() {
    if (tokens_[position_].type != TokenType::SELECT) {
        throw std::runtime_error("Expected SELECT");
    }

    ++position_;

    // Parse an expression.
    Expression expression;

    if (tokens_[position_].type == TokenType::INTEGER) {
        expression = {
            ExpressionType::INTEGER,
            tokens_[position_].lexeme
        };
    } else if (tokens_[position_].type == TokenType::IDENTIFIER) {
        expression = {
            ExpressionType::IDENTIFIER,
            tokens_[position_].lexeme
        };
    } else {
        throw std::runtime_error("Expected expression");
    }

    ++position_;

    // Parse FROM.
    if (tokens_[position_].type != TokenType::FROM) {
        throw std::runtime_error("Expected FROM");
    }

    ++position_;

    // Parse table name.
    if (tokens_[position_].type != TokenType::IDENTIFIER) {
        throw std::runtime_error("Expected table name");
    }

    std::string tableName = tokens_[position_].lexeme;

    ++position_;

    // Parse WHERE.
    if (tokens_[position_].type != TokenType::WHERE) {
        throw std::runtime_error("Expected WHERE");
    }

    ++position_;

    // Parse WHERE column.
    if (tokens_[position_].type != TokenType::IDENTIFIER) {
        throw std::runtime_error("Expected WHERE column");
    }

    std::string whereColumn = tokens_[position_].lexeme;

    ++position_;

    // Parse comparison operator.
    TokenType operatorType = tokens_[position_].type;

    if (operatorType != TokenType::EQUAL &&
        operatorType != TokenType::LESS &&
        operatorType != TokenType::LESS_EQUAL &&
        operatorType != TokenType::GREATER &&
        operatorType != TokenType::GREATER_EQUAL &&
        operatorType != TokenType::NOT_EQUAL) {
        throw std::runtime_error("Expected comparison operator");
    }

    std::string whereOperator = tokens_[position_].lexeme;

    ++position_;

    // Parse WHERE value.
    Expression whereValue;

    if (tokens_[position_].type == TokenType::INTEGER) {
        whereValue = {
            ExpressionType::INTEGER,
            tokens_[position_].lexeme
        };
    } else if (tokens_[position_].type == TokenType::STRING) {
        whereValue = {
            ExpressionType::STRING,
            tokens_[position_].lexeme
        };
    } else if (tokens_[position_].type == TokenType::IDENTIFIER) {
        whereValue = {
            ExpressionType::IDENTIFIER,
            tokens_[position_].lexeme
        };
    } else {
        throw std::runtime_error("Expected WHERE value");
    }

    ++position_;

    // Parse semicolon.
    if (tokens_[position_].type != TokenType::SEMICOLON) {
        throw std::runtime_error("Expected semicolon");
    }

    Condition where{
        whereColumn,
        whereOperator,
        whereValue
    };

    return SelectStatement{
        expression,
        tableName,
        where
    };
}

}