#include <gtest/gtest.h>

#include "lexer/token.h"

using namespace nucleusdb;

// Creates an INTEGER token and verifies its type and lexeme.
TEST(TokenTest, CreatesIntegerToken) {
    Token token(TokenType::INTEGER, "42");

    EXPECT_EQ(token.type, TokenType::INTEGER);
    EXPECT_EQ(token.lexeme, "42");
}