
#include "extract_s.h"

#include <stdlib.h>

#include <cassert>
#include <iostream>
#include <variant>

#include "ast_node.h"
#include "extract_fns.h"
#include "state.h"

struct Code;

void extract_s(ASTNode root, ModuleTable& module_table) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::s);

    std::vector<State> prod = root.get_production();
    if (prod
        == std::vector<State> {
            NonTerminal::s,
            Terminal::BOFS,
            Terminal::MODULE,
            Terminal::ID,
            Terminal::SEMI,
            NonTerminal::fns,
            Terminal::EOFS}) {
        // extract functions of program

        ASTNode module = root.children.at(2);
        std::string name = module.lexeme;

        SymbolTable symbol_table;

        ASTNode fns = root.children.at(4);
        extract_fns(fns, symbol_table);

        module_table[name] = symbol_table;
    } else {
        std::cerr << "Invalid production found while extracting s."
                  << std::endl;
        exit(1);
    };
}
