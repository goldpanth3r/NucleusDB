#include "engine/engine.h"

#include "lexer/lexer.h"
#include "parser/parser.h"

#include <stdexcept>

namespace nucleusdb {

// Creates an engine with an empty database.
Engine::Engine() {}

// Executes a SQL statement.
void Engine::execute(const std::string& sql) {
    Lexer lexer(sql);
    std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);

    Executor executor(database_);

    switch (tokens[0].type) {
        case TokenType::CREATE:
            executor.execute(parser.parseCreateTable());
            break;

        case TokenType::INSERT:
            executor.execute(parser.parseInsert());
            break;

        default:
            throw std::runtime_error("Unsupported statement");
    }
}

// Returns the database.
Database& Engine::database() {
    return database_;
}

}