#pragma once

#include <vector>

#include "lexer/token.h"
#include "parser/ast.h"

namespace nucleusdb {

// Parses SQL tokens into an AST.
class Parser {
public:
    // Create a parser.
    explicit Parser(const std::vector<Token>& tokens);

    // Parse a SELECT statement.
    SelectStatement parseSelect();

private:
    const std::vector<Token>& tokens_;
    std::size_t position_ = 0;
};

}