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

    if (tokens_[position_].type != TokenType::INTEGER) {
        throw std::runtime_error("Expected integer");
    }

    int value = std::stoi(tokens_[position_].lexeme);

    ++position_;

    if (tokens_[position_].type != TokenType::SEMICOLON) {
        throw std::runtime_error("Expected semicolon");
    }

    return SelectStatement{value};
}

}