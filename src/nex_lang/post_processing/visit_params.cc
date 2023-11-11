
#include "visit_params.h"

#include <cassert>
#include <iostream>

#include "visit_vardef.h"

std::vector<std::shared_ptr<TypedVariable>>
visit_params(ASTNode root, SymbolTable& symbol_table) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::params);
    std::vector<std::shared_ptr<TypedVariable>> result;

    std::vector<State> prod = root.get_production();
    if (prod == std::vector<State> {NonTerminal::params, NonTerminal::vardef}) {
        // extract singular parameter
        ASTNode vardef = root.children.at(0);
        result.push_back(visit_vardef(vardef, symbol_table));
    } else if (prod == std::vector<State> {NonTerminal::params, NonTerminal::vardef, Terminal::COMMA, NonTerminal::params}) {
        // extract code parameter
        ASTNode vardef = root.children.at(0);
        result.push_back(visit_vardef(vardef, symbol_table));

        // extract rest of parameters
        ASTNode params = root.children.at(2);
        std::vector<std::shared_ptr<TypedVariable>> child_result =
            visit_params(params, symbol_table);
        result.insert(result.end(), child_result.begin(), child_result.end());
    } else {
        std::cerr << "Invalid production found while processing params."
                  << std::endl;
        exit(1);
    }

    return result;
}

std::vector<std::shared_ptr<TypedVariable>>
visit_optparams(ASTNode root, SymbolTable& symbol_table) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::optparams);
    std::vector<std::shared_ptr<TypedVariable>> result;

    std::vector<State> prod = root.get_production();
    if (prod == std::vector<State> {NonTerminal::optparams}) {
        // function has no parameters
    } else if (prod == std::vector<State> {NonTerminal::optparams, NonTerminal::params}) {
        // extract parameters
        ASTNode params = root.children.at(0);
        result = visit_params(params, symbol_table);
    } else {
        std::cerr << "Invalid production found while processing optparams."
                  << std::endl;
        exit(1);
    }

    return result;
}
