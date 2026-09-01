#include <gtest/gtest.h>

#include "lexer/lexer.h"

using namespace nucleusdb;

// Tokenize SELECT with an integer.
TEST(LexerTest, TokenizesSelectInteger) {
    Lexer lexer("SELECT 42;");

    auto tokens = lexer.tokenize();

    ASSERT_EQ(tokens.size(), 4);

    EXPECT_EQ(tokens[0].type, TokenType::SELECT);
    EXPECT_EQ(tokens[0].lexeme, "SELECT");

    EXPECT_EQ(tokens[1].type, TokenType::INTEGER);
    EXPECT_EQ(tokens[1].lexeme, "42");

    EXPECT_EQ(tokens[2].type, TokenType::SEMICOLON);
    EXPECT_EQ(tokens[2].lexeme, ";");

    EXPECT_EQ(tokens[3].type, TokenType::END_OF_FILE);
    EXPECT_EQ(tokens[3].lexeme, "");
}

// Tokenize an identifier.
TEST(LexerTest, TokenizesIdentifier) {
    Lexer lexer("SELECT age;");

    auto tokens = lexer.tokenize();

    ASSERT_EQ(tokens.size(), 4);

    EXPECT_EQ(tokens[0].type, TokenType::SELECT);
    EXPECT_EQ(tokens[1].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[1].lexeme, "age");
    EXPECT_EQ(tokens[2].type, TokenType::SEMICOLON);
    EXPECT_EQ(tokens[3].type, TokenType::END_OF_FILE);
}

// Tokenize operators and parentheses.
TEST(LexerTest, TokenizesOperators) {
    Lexer lexer("SELECT (12 + 35) * 2;");

    auto tokens = lexer.tokenize();

    ASSERT_EQ(tokens.size(), 10);

    EXPECT_EQ(tokens[0].type, TokenType::SELECT);
    EXPECT_EQ(tokens[1].type, TokenType::LPAREN);

    EXPECT_EQ(tokens[2].type, TokenType::INTEGER);
    EXPECT_EQ(tokens[2].lexeme, "12");

    EXPECT_EQ(tokens[3].type, TokenType::PLUS);

    EXPECT_EQ(tokens[4].type, TokenType::INTEGER);
    EXPECT_EQ(tokens[4].lexeme, "35");

    EXPECT_EQ(tokens[5].type, TokenType::RPAREN);
    EXPECT_EQ(tokens[6].type, TokenType::STAR);

    EXPECT_EQ(tokens[7].type, TokenType::INTEGER);
    EXPECT_EQ(tokens[7].lexeme, "2");

    EXPECT_EQ(tokens[8].type, TokenType::SEMICOLON);
    EXPECT_EQ(tokens[9].type, TokenType::END_OF_FILE);
}