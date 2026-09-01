#include "parser/parser.h"

#include <stdexcept>

namespace nucleusdb {

// Creates a parser from the given token list.
Parser::Parser(const std::vector<Token>& tokens)
    : tokens_(tokens) {}


static Expression parseExpression(
    const std::vector<Token>& tokens,
    size_t& position);

// Parse a primary expression.
//
// Primary expressions are:
// INTEGER
// IDENTIFIER
// STRING
// STAR
// ( expression )
static Expression parsePrimary(
    const std::vector<Token>& tokens,
    size_t& position) {

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

    if (tokens[position].type == TokenType::LPAREN) {
        ++position;

        Expression expression = parseExpression(tokens, position);

        if (tokens[position].type != TokenType::RPAREN) {
            throw std::runtime_error("Expected ')'");
        }

        ++position;

        return expression;
    }

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

// Parse multiplication and division.
//
// * and / have higher precedence than + and -.
static Expression parseTerm(
    const std::vector<Token>& tokens,
    size_t& position) {

    Expression left = parsePrimary(tokens, position);

    while (tokens[position].type == TokenType::STAR ||
           tokens[position].type == TokenType::SLASH) {

        std::string operatorSymbol = tokens[position].lexeme;
        ++position;

        Expression right = parsePrimary(tokens, position);

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

// Parse addition and subtraction.
//
// + and - have lower precedence than * and /.
static Expression parseExpression(
    const std::vector<Token>& tokens,
    size_t& position) {

    Expression left = parseTerm(tokens, position);

    while (tokens[position].type == TokenType::PLUS ||
           tokens[position].type == TokenType::MINUS) {

        std::string operatorSymbol = tokens[position].lexeme;
        ++position;

        Expression right = parseTerm(tokens, position);

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

// Parse a SELECT statement of the form:
//
// SELECT <expr> [, <expr>]*
// FROM <table>
// [WHERE <column> <op> <value>];
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

    std::string tableName = tokens_[position_].lexeme;

    ++position_;

    // Optional WHERE clause.
    std::optional<Condition> where;

    if (tokens_[position_].type == TokenType::WHERE) {
        ++position_;

        // Parse WHERE column.
        if (tokens_[position_].type != TokenType::IDENTIFIER) {
            throw std::runtime_error("Expected WHERE column");
        }

        std::string column = tokens_[position_].lexeme;

        ++position_;

        // Parse comparison operator.
        TokenType operatorType = tokens_[position_].type;

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

}