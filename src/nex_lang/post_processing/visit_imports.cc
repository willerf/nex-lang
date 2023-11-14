
#include "visit_imports.h"

#include <cassert>
#include <iostream>

void visit_imports(
    ASTNode root,
    SymbolTable& symbol_table,
    ModuleTable& module_table
) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::imports);

    std::vector<State> prod = root.get_production();
    if (prod == std::vector<State> {NonTerminal::imports}) {
        // No more imports
    } else if (prod == std::vector<State> {NonTerminal::imports, NonTerminal::import, NonTerminal::imports}) {
        ASTNode import = root.children.at(0);
        visit_import(import, symbol_table, module_table);

        ASTNode imports = root.children.at(1);
        visit_imports(imports, symbol_table, module_table);
    } else {
        std::cerr << "TODO" << std::endl;
        exit(1);
    }
}

void visit_import(
    ASTNode root,
    SymbolTable& symbol_table,
    ModuleTable& module_table
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

        SymbolTable& module_symbol_table = module_table.at(name);
        symbol_table.insert(
            module_symbol_table.begin(),
            module_symbol_table.end()
        );
    } else {
        std::cerr << "TODO" << std::endl;
        exit(1);
    }
}
