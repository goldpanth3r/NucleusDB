#include <gtest/gtest.h>

#include "lexer/lexer.h"
#include "parser/parser.h"

using namespace nucleusdb;

// Parse SELECT with an integer.
TEST(ParserTest, ParsesSelectInteger) {
    Lexer lexer("SELECT 42;");

    auto tokens = lexer.tokenize();

    Parser parser(tokens);

    auto statement = parser.parseSelect();

    EXPECT_EQ(statement.value, 42);
}