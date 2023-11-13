
#include "visit_s.h"

#include <stdlib.h>

#include <cassert>
#include <iostream>
#include <string>
#include <variant>

#include "ast_node.h"
#include "state.h"
#include "symbol_table.h"
#include "visit_fns.h"

struct Code;

std::vector<std::shared_ptr<TypedProcedure>> visit_s(
    ASTNode root,
    ModuleTable& module_table,
    std::vector<std::shared_ptr<Code>>& static_data
) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::s);
    std::vector<std::shared_ptr<TypedProcedure>> result;

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

        SymbolTable symbol_table = module_table.at(name);

        ASTNode fns = root.children.at(4);
        result = visit_fns(fns, symbol_table, module_table, static_data);
    } else {
        std::cerr << "Invalid production found while processing s."
                  << std::endl;
        exit(1);
    };

    return result;
}
