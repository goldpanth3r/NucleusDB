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
    EXPECT_EQ(tokens[0].lexeme, "SELECT");

    EXPECT_EQ(tokens[1].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[1].lexeme, "age");

    EXPECT_EQ(tokens[2].type, TokenType::SEMICOLON);
    EXPECT_EQ(tokens[2].lexeme, ";");

    EXPECT_EQ(tokens[3].type, TokenType::END_OF_FILE);
    EXPECT_EQ(tokens[3].lexeme, "");
}

// Tokenize a string.
TEST(LexerTest, TokenizesString) {
    Lexer lexer("SELECT 'Alice';");

    auto tokens = lexer.tokenize();

    ASSERT_EQ(tokens.size(), 4);

    EXPECT_EQ(tokens[0].type, TokenType::SELECT);
    EXPECT_EQ(tokens[0].lexeme, "SELECT");

    EXPECT_EQ(tokens[1].type, TokenType::STRING);
    EXPECT_EQ(tokens[1].lexeme, "Alice");

    EXPECT_EQ(tokens[2].type, TokenType::SEMICOLON);
    EXPECT_EQ(tokens[2].lexeme, ";");

    EXPECT_EQ(tokens[3].type, TokenType::END_OF_FILE);
    EXPECT_EQ(tokens[3].lexeme, "");
}

// Tokenize a SELECT statement.
TEST(LexerTest, TokenizesSelectFrom) {
    Lexer lexer("SELECT age FROM users;");

    auto tokens = lexer.tokenize();

    ASSERT_EQ(tokens.size(), 6);

    EXPECT_EQ(tokens[0].type, TokenType::SELECT);
    EXPECT_EQ(tokens[0].lexeme, "SELECT");

    EXPECT_EQ(tokens[1].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[1].lexeme, "age");

    EXPECT_EQ(tokens[2].type, TokenType::FROM);
    EXPECT_EQ(tokens[2].lexeme, "FROM");

    EXPECT_EQ(tokens[3].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[3].lexeme, "users");

    EXPECT_EQ(tokens[4].type, TokenType::SEMICOLON);
    EXPECT_EQ(tokens[4].lexeme, ";");

    EXPECT_EQ(tokens[5].type, TokenType::END_OF_FILE);
    EXPECT_EQ(tokens[5].lexeme, "");
}

// Tokenize WHERE and comparison operators.
TEST(LexerTest, TokenizesWhere) {
    Lexer lexer("SELECT age FROM users WHERE age > 18;");

    auto tokens = lexer.tokenize();

    ASSERT_EQ(tokens.size(), 10);

    EXPECT_EQ(tokens[0].type, TokenType::SELECT);
    EXPECT_EQ(tokens[0].lexeme, "SELECT");

    EXPECT_EQ(tokens[1].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[1].lexeme, "age");

    EXPECT_EQ(tokens[2].type, TokenType::FROM);
    EXPECT_EQ(tokens[2].lexeme, "FROM");

    EXPECT_EQ(tokens[3].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[3].lexeme, "users");

    EXPECT_EQ(tokens[4].type, TokenType::WHERE);
    EXPECT_EQ(tokens[4].lexeme, "WHERE");

    EXPECT_EQ(tokens[5].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[5].lexeme, "age");

    EXPECT_EQ(tokens[6].type, TokenType::GREATER);
    EXPECT_EQ(tokens[6].lexeme, ">");

    EXPECT_EQ(tokens[7].type, TokenType::INTEGER);
    EXPECT_EQ(tokens[7].lexeme, "18");

    EXPECT_EQ(tokens[8].type, TokenType::SEMICOLON);
    EXPECT_EQ(tokens[8].lexeme, ";");

    EXPECT_EQ(tokens[9].type, TokenType::END_OF_FILE);
    EXPECT_EQ(tokens[9].lexeme, "");
}

// Tokenize operators and parentheses.
TEST(LexerTest, TokenizesOperators) {
    Lexer lexer("SELECT (12 + 35) * 2;");

    auto tokens = lexer.tokenize();

    ASSERT_EQ(tokens.size(), 10);

    EXPECT_EQ(tokens[0].type, TokenType::SELECT);
    EXPECT_EQ(tokens[0].lexeme, "SELECT");

    EXPECT_EQ(tokens[1].type, TokenType::LPAREN);
    EXPECT_EQ(tokens[1].lexeme, "(");

    EXPECT_EQ(tokens[2].type, TokenType::INTEGER);
    EXPECT_EQ(tokens[2].lexeme, "12");

    EXPECT_EQ(tokens[3].type, TokenType::PLUS);
    EXPECT_EQ(tokens[3].lexeme, "+");

    EXPECT_EQ(tokens[4].type, TokenType::INTEGER);
    EXPECT_EQ(tokens[4].lexeme, "35");

    EXPECT_EQ(tokens[5].type, TokenType::RPAREN);
    EXPECT_EQ(tokens[5].lexeme, ")");

    EXPECT_EQ(tokens[6].type, TokenType::STAR);
    EXPECT_EQ(tokens[6].lexeme, "*");

    EXPECT_EQ(tokens[7].type, TokenType::INTEGER);
    EXPECT_EQ(tokens[7].lexeme, "2");

    EXPECT_EQ(tokens[8].type, TokenType::SEMICOLON);
    EXPECT_EQ(tokens[8].lexeme, ";");

    EXPECT_EQ(tokens[9].type, TokenType::END_OF_FILE);
    EXPECT_EQ(tokens[9].lexeme, "");
}