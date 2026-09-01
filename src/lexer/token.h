#pragma once

#include <string>
#include <utility>

namespace nucleusdb {

// TokenType enumerates all recognized token types.
enum class TokenType {
    // Keywords
    SELECT,
    INSERT,
    INTO,
    VALUES,
    FROM,
    WHERE,

    // Literals and identifiers
    INTEGER,
    IDENTIFIER,
    STRING,

    // Arithmetic operators
    PLUS,
    MINUS,
    STAR,
    SLASH,

    // Comparison operators
    EQUAL,
    LESS,
    LESS_EQUAL,
    GREATER,
    GREATER_EQUAL,
    NOT_EQUAL,

    // Delimiters
    LPAREN,
    RPAREN,
    COMMA,
    SEMICOLON,

    END_OF_FILE
};

// Token holds data about a single token.
struct Token {
    TokenType type;
    std::string lexeme;

    Token(TokenType type, std::string lexeme)
        : type(type), lexeme(std::move(lexeme)) {}
};

}