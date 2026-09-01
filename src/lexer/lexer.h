#pragma once

#include <string>
#include <vector>

#include "lexer/token.h"

namespace nucleusdb {

// Converts SQL text into tokens.
class Lexer {
public:
    // Create a lexer.
    explicit Lexer(std::string source);

    // Tokenize the source.
    std::vector<Token> tokenize();

private:
    std::string source_;
    std::size_t position_ = 0;

    // Skip whitespace.
    void skipWhitespace();

    // Read an integer.
    Token readInteger();

    // Read a keyword or identifier.
    Token readWordOrIdentifier();
};

}