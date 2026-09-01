#include <gtest/gtest.h>

#include "lexer/token.h"

using namespace nucleusdb;

// Create an integer token.
TEST(TokenTest, CreatesIntegerToken) {
    Token token(TokenType::INTEGER, "42");

    EXPECT_EQ(token.type, TokenType::INTEGER);
    EXPECT_EQ(token.lexeme, "42");
}