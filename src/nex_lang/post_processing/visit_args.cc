
#include "visit_args.h"

#include <stdlib.h>

#include <cassert>
#include <iostream>
#include <variant>

#include "ast_node.h"
#include "state.h"
#include "visit_expr.h"

struct Code;

std::vector<TypedExpr> visit_args(
    ASTNode root,
    SymbolTable& symbol_table,
    std::vector<std::shared_ptr<Code>>& static_data
) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::args);
    std::vector<TypedExpr> result;

    std::vector<State> prod = root.get_production();
    if (prod == std::vector<State> {NonTerminal::args, NonTerminal::expr}) {
        // extract singular argument
        ASTNode expr = root.children.at(0);
        result.push_back(visit_expr(expr, false, symbol_table, static_data));
    } else if (prod == std::vector<State> {NonTerminal::args, NonTerminal::expr, Terminal::COMMA, NonTerminal::args}) {
        // extract code argument
        ASTNode expr = root.children.at(0);
        result.push_back(visit_expr(expr, false, symbol_table, static_data));

        // extract rest of arguments
        ASTNode args = root.children.at(2);
        auto child_result = visit_args(args, symbol_table, static_data);
        result.insert(result.end(), child_result.begin(), child_result.end());
    } else {
        std::cerr << "Invalid production found while processing args."
                  << std::endl;
        exit(1);
    }

    return result;
}

std::vector<TypedExpr> visit_optargs(
    ASTNode root,
    SymbolTable& symbol_table,
    std::vector<std::shared_ptr<Code>>& static_data
) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::optargs);
    std::vector<TypedExpr> result;

    std::vector<State> prod = root.get_production();
    if (prod == std::vector<State> {NonTerminal::optargs}) {
        // function call passing in no arguments
    } else if (prod == std::vector<State> {NonTerminal::optargs, NonTerminal::args}) {
        // extract arguments
        ASTNode args = root.children.at(0);
        result = visit_args(args, symbol_table, static_data);
    } else {
        std::cerr << "Invalid production found while processing optargs."
                  << std::endl;
        exit(1);
    }

    return result;
}
