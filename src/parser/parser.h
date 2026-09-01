#pragma once

#include <vector>
#include <optional>

#include "lexer/token.h"
#include "parser/ast.h"

namespace nucleusdb {

// Parser converts a sequence of tokens into an abstract syntax tree.
class Parser {
public:
    // Creates a new Parser from the given token list.
    explicit Parser(const std::vector<Token>& tokens);

    // Parses a SELECT statement and returns the corresponding AST node.
    SelectStatement parseSelect();

    InsertStatement parseInsert();
    
private:
    const std::vector<Token>& tokens_;
    std::size_t position_ = 0;
};

}