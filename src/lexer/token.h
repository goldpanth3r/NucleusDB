#pragma once

#include <string>
#include <utility>

namespace nucleusdb {

// Token types.
enum class TokenType {
    SELECT,
    FROM,
    WHERE,

    INTEGER,
    IDENTIFIER,
    STRING,

    PLUS,
    MINUS,
    STAR,
    SLASH,

    EQUAL,
    LESS,
    LESS_EQUAL,
    GREATER,
    GREATER_EQUAL,
    NOT_EQUAL,

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