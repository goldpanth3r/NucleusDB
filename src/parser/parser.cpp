#include "parser/parser.h"

#include <stdexcept>

namespace nucleusdb {

// Creates a parser from the given token list.
Parser::Parser(const std::vector<Token>& tokens)
    : tokens_(tokens) {}

// Parses an expression.
static Expression parseExpression(
    const std::vector<Token>& tokens,
    size_t& position);

// Parses a primary expression.
static Expression parsePrimary(
    const std::vector<Token>& tokens,
    size_t& position) {

    // Parse unary minus.
    if (tokens[position].type == TokenType::MINUS) {
        ++position;

        Expression child = parsePrimary(tokens, position);

        Expression unary{
            ExpressionType::UNARY,
            "-"
        };

        unary.children = std::vector<Expression>{child};

        return unary;
    }

    // Parse parentheses.
    if (tokens[position].type == TokenType::LPAREN) {
        ++position;

        Expression expression =
            parseExpression(tokens, position);

        if (tokens[position].type != TokenType::RPAREN) {
            throw std::runtime_error("Expected ')'");
        }

        ++position;

        return expression;
    }

    // Parse basic values.
    switch (tokens[position].type) {
        case TokenType::INTEGER:
            return {
                ExpressionType::INTEGER,
                tokens[position++].lexeme
            };

        case TokenType::IDENTIFIER:
            return {
                ExpressionType::IDENTIFIER,
                tokens[position++].lexeme
            };

        case TokenType::STRING:
            return {
                ExpressionType::STRING,
                tokens[position++].lexeme
            };

        case TokenType::STAR:
            return {
                ExpressionType::STAR,
                tokens[position++].lexeme
            };

        default:
            throw std::runtime_error("Expected expression");
    }
}

// Parses multiplication and division.
static Expression parseTerm(
    const std::vector<Token>& tokens,
    size_t& position) {

    Expression left =
        parsePrimary(tokens, position);

    while (tokens[position].type == TokenType::STAR ||
           tokens[position].type == TokenType::SLASH) {

        std::string operatorSymbol =
            tokens[position].lexeme;

        ++position;

        Expression right =
            parsePrimary(tokens, position);

        Expression binary{
            ExpressionType::BINARY,
            operatorSymbol
        };

        binary.children = std::vector<Expression>{
            left,
            right
        };

        left = binary;
    }

    return left;
}

// Parses addition and subtraction.
static Expression parseExpression(
    const std::vector<Token>& tokens,
    size_t& position) {

    Expression left =
        parseTerm(tokens, position);

    while (tokens[position].type == TokenType::PLUS ||
           tokens[position].type == TokenType::MINUS) {

        std::string operatorSymbol =
            tokens[position].lexeme;

        ++position;

        Expression right =
            parseTerm(tokens, position);

        Expression binary{
            ExpressionType::BINARY,
            operatorSymbol
        };

        binary.children = std::vector<Expression>{
            left,
            right
        };

        left = binary;
    }

    return left;
}

// Parses a SELECT statement.
SelectStatement Parser::parseSelect() {

    // Consume SELECT.
    if (tokens_[position_].type != TokenType::SELECT) {
        throw std::runtime_error("Expected SELECT");
    }

    ++position_;

    // Parse SELECT expressions.
    std::vector<Expression> expressions;

    expressions.push_back(
        parseExpression(tokens_, position_)
    );

    // Parse additional expressions.
    while (tokens_[position_].type == TokenType::COMMA) {
        ++position_;

        expressions.push_back(
            parseExpression(tokens_, position_)
        );
    }

    // Consume FROM.
    if (tokens_[position_].type != TokenType::FROM) {
        throw std::runtime_error("Expected FROM");
    }

    ++position_;

    // Parse table name.
    if (tokens_[position_].type != TokenType::IDENTIFIER) {
        throw std::runtime_error("Expected table name");
    }

    std::string tableName =
        tokens_[position_].lexeme;

    ++position_;

    // Parse optional WHERE.
    std::optional<Condition> where;

    if (tokens_[position_].type == TokenType::WHERE) {
        ++position_;

        // Parse WHERE column.
        if (tokens_[position_].type != TokenType::IDENTIFIER) {
            throw std::runtime_error("Expected WHERE column");
        }

        std::string column =
            tokens_[position_].lexeme;

        ++position_;

        // Parse comparison operator.
        TokenType operatorType =
            tokens_[position_].type;

        if (operatorType != TokenType::EQUAL &&
            operatorType != TokenType::LESS &&
            operatorType != TokenType::LESS_EQUAL &&
            operatorType != TokenType::GREATER &&
            operatorType != TokenType::GREATER_EQUAL &&
            operatorType != TokenType::NOT_EQUAL) {

            throw std::runtime_error(
                "Expected comparison operator"
            );
        }

        std::string operatorSymbol =
            tokens_[position_].lexeme;

        ++position_;

        // Parse WHERE value.
        Expression value =
            parseExpression(tokens_, position_);

        where = Condition{
            column,
            operatorSymbol,
            value
        };
    }

    // Expect semicolon.
    if (tokens_[position_].type != TokenType::SEMICOLON) {
        throw std::runtime_error("Expected semicolon");
    }

    return SelectStatement{
        expressions,
        tableName,
        where
    };
}

// Parses an INSERT statement.
// Parses an INSERT statement.
InsertStatement Parser::parseInsert() {

    // Consume INSERT.
    if (tokens_[position_].type != TokenType::INSERT) {
        throw std::runtime_error("Expected INSERT");
    }

    ++position_;

    // Consume INTO.
    if (tokens_[position_].type != TokenType::INTO) {
        throw std::runtime_error("Expected INTO");
    }

    ++position_;

    // Parse table name.
    if (tokens_[position_].type != TokenType::IDENTIFIER) {
        throw std::runtime_error("Expected table name");
    }

    std::string tableName =
        tokens_[position_].lexeme;

    ++position_;

    // Parse optional column list.
    std::vector<std::string> columns;

    if (tokens_[position_].type == TokenType::LPAREN) {
        ++position_;

        // Parse first column.
        if (tokens_[position_].type != TokenType::IDENTIFIER) {
            throw std::runtime_error("Expected column name");
        }

        columns.push_back(
            tokens_[position_].lexeme
        );

        ++position_;

        // Parse additional columns.
        while (tokens_[position_].type == TokenType::COMMA) {
            ++position_;

            if (tokens_[position_].type != TokenType::IDENTIFIER) {
                throw std::runtime_error("Expected column name");
            }

            columns.push_back(
                tokens_[position_].lexeme
            );

            ++position_;
        }

        // Expect closing parenthesis.
        if (tokens_[position_].type != TokenType::RPAREN) {
            throw std::runtime_error("Expected ')'");
        }

        ++position_;
    }

    // Consume VALUES.
    if (tokens_[position_].type != TokenType::VALUES) {
        throw std::runtime_error("Expected VALUES");
    }

    ++position_;

    // Expect opening parenthesis.
    if (tokens_[position_].type != TokenType::LPAREN) {
        throw std::runtime_error("Expected '('");
    }

    ++position_;

    // Parse values.
    std::vector<Expression> values;

    values.push_back(
        parseExpression(tokens_, position_)
    );

    while (tokens_[position_].type == TokenType::COMMA) {
        ++position_;

        values.push_back(
            parseExpression(tokens_, position_)
        );
    }

    // Expect closing parenthesis.
    if (tokens_[position_].type != TokenType::RPAREN) {
        throw std::runtime_error("Expected ')'");
    }

    ++position_;

    // Check column and value counts.
    if (!columns.empty() &&
        columns.size() != values.size()) {
        throw std::runtime_error(
            "Column count does not match value count"
        );
    }


    // Expect semicolon.
    if (tokens_[position_].type != TokenType::SEMICOLON) {
        throw std::runtime_error("Expected semicolon");
    }

    return InsertStatement{
        tableName,
        columns,
        values
    };
}

}