#include "lexer/lexer.h"

#include <cctype>
#include <stdexcept>
#include <utility>

namespace nucleusdb {

// Lexer processes an input string and produces tokens on demand.
Lexer::Lexer(std::string source)
    : source_(std::move(source)) {}

// skipWhitespace advances over any sequence of whitespace.
void Lexer::skipWhitespace() {
    while (position_ < source_.size() &&
           std::isspace(static_cast<unsigned char>(source_[position_]))) {
        ++position_;
    }
}

// readInteger scans a sequence of digits and returns an INTEGER token.
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

// readWordOrIdentifier scans an identifier-like token.
// Returns a keyword token if the word matches a known keyword, otherwise an IDENTIFIER.
Token Lexer::readWordOrIdentifier() {
    const std::size_t start = position_;

    while (position_ < source_.size() &&
           std::isalnum(static_cast<unsigned char>(source_[position_]))) {
        ++position_;
    }

    std::string word = source_.substr(start, position_ - start);

    if (word == "SELECT") {
        return Token(TokenType::SELECT, word);
    }
    if (word == "FROM") {
        return Token(TokenType::FROM, word);
    }
    if (word == "WHERE") {
        return Token(TokenType::WHERE, word);
    }

    return Token(TokenType::IDENTIFIER, word);
}

// readString scans a single-quoted string literal.
// Returns the string value (without quotes), or throws if unterminated.
Token Lexer::readString() {
    ++position_; // consume the opening quote

    const std::size_t start = position_;

    while (position_ < source_.size() && source_[position_] != '\'') {
        ++position_;
    }

    if (position_ >= source_.size()) {
        throw std::runtime_error("Unterminated string");
    }

    std::string value = source_.substr(start, position_ - start);
    ++position_; // consume the closing quote

    return Token(TokenType::STRING, value);
}

// tokenize advances through the entire source and returns the full list of tokens.
std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (position_ < source_.size()) {
        skipWhitespace();

        if (position_ >= source_.size()) {
            break;
        }

        const char current = source_[position_];

        // Digit => integer
        if (std::isdigit(static_cast<unsigned char>(current))) {
            tokens.push_back(readInteger());
            continue;
        }

        // Letter => keyword or identifier
        if (std::isalpha(static_cast<unsigned char>(current))) {
            tokens.push_back(readWordOrIdentifier());
            continue;
        }

        // Single-quoted string
        if (current == '\'') {
            tokens.push_back(readString());
            continue;
        }

        // Operators and delimiters
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

            case ',':
                tokens.emplace_back(TokenType::COMMA, ",");
                ++position_;
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

    // Append EOF token
    tokens.emplace_back(TokenType::END_OF_FILE, "");

    return tokens;
}

}