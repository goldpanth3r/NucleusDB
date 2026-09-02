#pragma once

#include <optional>
#include <vector>

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

    // Parses an INSERT statement and returns the corresponding AST node.
    InsertStatement parseInsert();

    // Parses a CREATE TABLE statement and returns the corresponding AST node.
    CreateTableStatement parseCreateTable();

private:
    const std::vector<Token>& tokens_;
    std::size_t position_ = 0;
};

}