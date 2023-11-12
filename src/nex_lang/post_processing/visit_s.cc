
#include "visit_s.h"

#include <stdlib.h>
#include <cassert>
#include <iostream>
#include <variant>

#include "visit_fns.h"
#include "ast_node.h"
#include "state.h"

struct Code;

std::vector<std::shared_ptr<TypedProcedure>> visit_s(
    ASTNode root,
    SymbolTable& symbol_table,
    std::vector<std::shared_ptr<Code>>& static_data
) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::s);
    std::vector<std::shared_ptr<TypedProcedure>> result;

    std::vector<State> prod = root.get_production();
    if (prod
        == std::vector<State> {
            NonTerminal::s,
            Terminal::BOFS,
            NonTerminal::fns,
            Terminal::EOFS}) {
        // extract functions of program
        ASTNode fns = root.children.at(1);
        result = visit_fns(fns, symbol_table, static_data);
    } else {
        std::cerr << "Invalid production found while processing s."
                  << std::endl;
        exit(1);
    };

    return result;
}
