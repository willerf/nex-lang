
#include "extract_s.h"

#include <stdlib.h>

#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <variant>
#include <vector>

#include "ast_node.h"
#include "extract_fns.h"
#include "state.h"
#include "symbol_table.h"

void extract_s(ASTNode root, ModuleTable& module_table) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::s);

    std::vector<State> prod = root.get_production();
    if (prod
        == std::vector<State> {
            NonTerminal::s,
            Terminal::BOFS,
            NonTerminal::module,
            NonTerminal::imports,
            NonTerminal::fns,
            Terminal::EOFS}) {
        // extract functions of program

        ASTNode module = root.children.at(1);
        std::string name = module.children.at(1).lexeme;

        SymbolTable symbol_table;

        ASTNode fns = root.children.at(3);
        extract_fns(fns, symbol_table);

        module_table[name] = symbol_table;
    } else {
        std::cerr << "Invalid production found while extracting s."
                  << std::endl;
        exit(1);
    };
}