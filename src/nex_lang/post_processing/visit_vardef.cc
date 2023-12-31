
#include "visit_vardef.h"

#include <stdlib.h>

#include <cassert>
#include <iostream>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "ast_node.h"
#include "duplicate_symbol_error.h"
#include "state.h"
#include "variable.h"
#include "visit_type.h"

struct NLType;
struct ProgramContext;

std::shared_ptr<TypedVariable> visit_vardef(
    ASTNode root,
    SymbolTable& symbol_table,
    ProgramContext& program_context
) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::vardef);
    std::shared_ptr<TypedVariable> result = nullptr;

    std::vector<State> prod = root.get_production();
    if (prod
        == std::vector<State> {
            NonTerminal::vardef,
            Terminal::ID,
            Terminal::COLON,
            NonTerminal::type}) {
        // extract variable definition
        ASTNode id = root.children.at(0);
        std::string name = id.lexeme;

        if (symbol_table.count({name, {}})) {
            throw DuplicateSymbolError(name, id.line_no);
        } else {
            ASTNode var_type = root.children.at(2);
            std::shared_ptr<NLType> nl_type =
                visit_type(var_type, program_context);

            std::shared_ptr<Variable> variable =
                std::make_shared<Variable>(name);
            result = std::make_shared<TypedVariable>(variable, nl_type);
            symbol_table[{name, {}}] = result;
        }
    } else {
        std::cerr << "Invalid production found while processing vardef."
                  << std::endl;
        exit(1);
    }

    assert(result);
    return result;
}
