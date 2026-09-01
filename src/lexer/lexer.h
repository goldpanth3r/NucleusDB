#pragma once

#include <string>
#include <vector>

#include "lexer/token.h"

namespace nucleusdb {

// Lexer processes an input string and produces tokens.
class Lexer {
public:
    // Creates a new Lexer from the given SQL statement.
    explicit Lexer(std::string source);

    // Advances through the source and returns the full list of tokens.
    std::vector<Token> tokenize();

private:
    std::string source_;
    std::size_t position_ = 0;

    // Advances over any sequence of whitespace.
    void skipWhitespace();

    // Scans a sequence of digits and returns an INTEGER token.
    Token readInteger();

    // Scans a single-quoted string literal.
    // Returns the string value (without quotes), or throws if unterminated.
    Token readString();

    // Scans an identifier-like token.
    // Returns a keyword token if the word matches a known keyword, otherwise an IDENTIFIER.
    Token readWordOrIdentifier();
};

}