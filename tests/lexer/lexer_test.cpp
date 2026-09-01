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