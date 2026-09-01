#pragma once

#include <string>
#include <utility>

namespace nucleusdb {

// Token types.
enum class TokenType {
    SELECT,

    INTEGER,
    IDENTIFIER,

    PLUS,
    MINUS,
    STAR,
    SLASH,

    LPAREN,
    RPAREN,
    SEMICOLON,

    END_OF_FILE
};

// A SQL token.
struct Token {
    TokenType type;
    std::string lexeme;

    Token(TokenType type, std::string lexeme)
        : type(type), lexeme(std::move(lexeme)) {}
};

}