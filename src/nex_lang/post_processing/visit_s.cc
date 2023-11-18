
#include "visit_s.h"

#include <stdlib.h>

#include <cassert>
#include <iostream>
#include <string>
#include <variant>

#include "ast_node.h"
#include "heap.h"
#include "state.h"
#include "symbol_table.h"
#include "visit_fns.h"
#include "visit_imports.h"

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
            NonTerminal::module,
            NonTerminal::imports,
            NonTerminal::fns,
            Terminal::EOFS}) {
        // extract functions of program

        ASTNode module = root.children.at(1);
        std::string name = module.children.at(1).lexeme;

        SymbolTable symbol_table = module_table.at(name);

        if (module_table.contains(heap_module_id)) {
            SymbolTable heap_module = module_table.at(heap_module_id);
            symbol_table.insert(heap_module.begin(), heap_module.end());
        }

        ASTNode imports = root.children.at(2);
        visit_imports(imports, symbol_table, module_table);

        ASTNode fns = root.children.at(3);
        result = visit_fns(fns, symbol_table, module_table, static_data);
    } else {
        std::cerr << "Invalid production found while processing s."
                  << std::endl;
        exit(1);
    };

    return result;
}
