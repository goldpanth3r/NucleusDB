#pragma once

#include <string>
#include <utility>

namespace nucleusdb {

enum class TokenType {
    SELECT,

    INTEGER,

    PLUS,
    MINUS,
    STAR,
    SLASH,

    LPAREN,
    RPAREN,
    SEMICOLON,

    END_OF_FILE
};

struct Token {
    TokenType type;
    std::string lexeme;

    Token(TokenType type, std::string lexeme)
        : type(type), lexeme(std::move(lexeme)) {}
};

} // namespace nucleusdb