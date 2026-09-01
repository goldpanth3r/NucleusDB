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
Token Lexer::readWordOrIdentifier() {
    const std::size_t start = position_;

    while (position_ < source_.size() &&
           std::isalnum(static_cast<unsigned char>(source_[position_]))) {
        ++position_;
    }

    std::string word = source_.substr(start, position_ - start);

    // Read SELECT keyword.
    if (word == "SELECT") {
        return Token(TokenType::SELECT, word);
    }

    // Read FROM keyword.
    if (word == "FROM") {
        return Token(TokenType::FROM, word);
    }

    // Read WHERE keyword.
    if (word == "WHERE") {
        return Token(TokenType::WHERE, word);
    }
    return Token(TokenType::IDENTIFIER, word);
}

// Read a string.
Token Lexer::readString() {
    ++position_;

    const std::size_t start = position_;

    while (position_ < source_.size() && source_[position_] != '\'') {
        ++position_;
    }

    if (position_ >= source_.size()) {
        throw std::runtime_error("Unterminated string");
    }

    std::string value = source_.substr(start, position_ - start);

    ++position_;

    return Token(TokenType::STRING, value);
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
            tokens.push_back(readWordOrIdentifier());
            continue;
        }

        // Read a string.
        if (current == '\'') {
            tokens.push_back(readString());
            continue;
        }

        // Read operators and parentheses.
        switch (current) {
            case '+':
                tokens.emplace_back(TokenType::PLUS, "+");
                break;

            case '-':
                tokens.emplace_back(TokenType::MINUS, "-");
                break;

            case '*':
                tokens.emplace_back(TokenType::STAR, "*");
                break;

            case '/':
                tokens.emplace_back(TokenType::SLASH, "/");
                break;

            case '=':
            tokens.emplace_back(TokenType::EQUAL, "=");
            break;

            case '<':
                ++position_;

                if (position_ < source_.size() && source_[position_] == '=') {
                    tokens.emplace_back(TokenType::LESS_EQUAL, "<=");
                    ++position_;
                } else {
                    tokens.emplace_back(TokenType::LESS, "<");
                }
                break;

            case '>':
                ++position_;

                if (position_ < source_.size() && source_[position_] == '=') {
                    tokens.emplace_back(TokenType::GREATER_EQUAL, ">=");
                    ++position_;
                } else {
                    tokens.emplace_back(TokenType::GREATER, ">");
                }
                break;

            case '!':
                ++position_;

                if (position_ < source_.size() && source_[position_] == '=') {
                    tokens.emplace_back(TokenType::NOT_EQUAL, "!=");
                    ++position_;
                } else {
                    throw std::runtime_error("Unexpected character: !");
                }
                break;

            case '(':
                tokens.emplace_back(TokenType::LPAREN, "(");
                break;

            case ')':
                tokens.emplace_back(TokenType::RPAREN, ")");
                break;

            case ';':
                tokens.emplace_back(TokenType::SEMICOLON, ";");
                break;

            default:
                throw std::runtime_error(
                    std::string("Unexpected character: ") + current
                );
        }

        ++position_;
    }

    // Add EOF token.
    tokens.emplace_back(TokenType::END_OF_FILE, "");

    return tokens;
}

}