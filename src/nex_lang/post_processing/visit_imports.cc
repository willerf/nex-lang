
#include "visit_imports.h"

#include <stdlib.h>

#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <variant>
#include <vector>

#include "ast_node.h"
#include "program_context.h"
#include "state.h"
#include "symbol_not_found_error.h"

void visit_imports(
    ASTNode root,
    SymbolTable& symbol_table,
    ProgramContext& program_context
) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::imports);

    std::vector<State> prod = root.get_production();
    if (prod == std::vector<State> {NonTerminal::imports}) {
        // No more imports
    } else if (prod == std::vector<State> {NonTerminal::imports, NonTerminal::import, NonTerminal::imports}) {
        ASTNode import = root.children.at(0);
        visit_import(import, symbol_table, program_context);

        ASTNode imports = root.children.at(1);
        visit_imports(imports, symbol_table, program_context);
    } else {
        std::cerr << "Invalid production found while processing imports."
                  << std::endl;
        exit(1);
    }
}

void visit_import(
    ASTNode root,
    SymbolTable& symbol_table,
    ProgramContext& program_context
) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::import);

    std::vector<State> prod = root.get_production();
    if (prod
        == std::vector<State> {
            NonTerminal::import,
            Terminal::IMPORT,
            Terminal::ID,
            Terminal::SEMI}) {
        ASTNode id = root.children.at(1);
        std::string name = id.lexeme;

        if (!program_context.module_table.contains(name)) {
            throw SymbolNotFoundError(name, root.children.at(0).line_no);
        }

        SymbolTable& module_symbol_table =
            program_context.module_table.at(name);
        symbol_table.insert(
            module_symbol_table.begin(),
            module_symbol_table.end()
        );
    } else {
        std::cerr << "Invalid production found while processing import."
                  << std::endl;
        exit(1);
    }
}
