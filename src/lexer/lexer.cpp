#include "lexer/lexer.h"

#include <cctype>
#include <stdexcept>
#include <utility>

namespace nucleusdb {

// Create a lexer.
Lexer::Lexer(std::string source)
    : source_(std::move(source)) {}

// Skip whitespace.
void Lexer::skipWhitespace() {
    while (position_ < source_.size() &&
           std::isspace(static_cast<unsigned char>(source_[position_]))) {
        ++position_;
    }
}

// Read an integer.
Token Lexer::readInteger() {
    const std::size_t start = position_;

    while (position_ < source_.size() &&
           std::isdigit(static_cast<unsigned char>(source_[position_]))) {
        ++position_;
    }

    return Token(
        TokenType::INTEGER,
        source_.substr(start, position_ - start)
    );
}

// Read a keyword or identifier.
Token Lexer::readWord() {
    const std::size_t start = position_;

    while (position_ < source_.size() &&
           std::isalpha(static_cast<unsigned char>(source_[position_]))) {
        ++position_;
    }

    std::string word = source_.substr(start, position_ - start);

    if (word == "SELECT") {
        return Token(TokenType::SELECT, word);
    }

    throw std::runtime_error("Unknown word: " + word);
}

// Tokenize the source.
std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (position_ < source_.size()) {
        skipWhitespace();

        if (position_ >= source_.size()) {
            break;
        }

        const char current = source_[position_];

        // Read an integer.
        if (std::isdigit(static_cast<unsigned char>(current))) {
            tokens.push_back(readInteger());
            continue;
        }

        // Read a keyword.
        if (std::isalpha(static_cast<unsigned char>(current))) {
            tokens.push_back(readWord());
            continue;
        }

        // Read a semicolon.
        if (current == ';') {
            tokens.emplace_back(TokenType::SEMICOLON, ";");
            ++position_;
            continue;
        }

        throw std::runtime_error(
            std::string("Unexpected character: ") + current
        );
    }

    // Add EOF token.
    tokens.emplace_back(TokenType::END_OF_FILE, "");

    return tokens;
}

}