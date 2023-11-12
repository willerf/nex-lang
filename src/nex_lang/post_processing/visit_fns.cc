
#include "visit_fns.h"

#include <stdlib.h>

#include <cassert>
#include <iostream>
#include <string>
#include <variant>

#include "ast_node.h"
#include "nl_type_none.h"
#include "procedure.h"
#include "scope.h"
#include "state.h"
#include "typed_variable.h"
#include "visit_params.h"
#include "visit_stmts.h"
#include "visit_type.h"

struct Code;
struct NLType;
struct Variable;

std::vector<std::shared_ptr<TypedProcedure>> visit_fns(
    ASTNode root,
    SymbolTable& symbol_table,
    std::vector<std::shared_ptr<Code>>& static_data
) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::fns);
    std::vector<std::shared_ptr<TypedProcedure>> result;

    std::vector<State> prod = root.get_production();
    if (prod == std::vector<State> {NonTerminal::fns, NonTerminal::fn}) {
        // extract singular function
        ASTNode fn = root.children.at(0);
        result.push_back(visit_fn(fn, symbol_table, static_data));
    } else if (prod == std::vector<State> {NonTerminal::fns, NonTerminal::fn, NonTerminal::fns}) {
        // extract code function
        ASTNode fn = root.children.at(0);
        result.push_back(visit_fn(fn, symbol_table, static_data));

        // extract rest of functions
        ASTNode fns = root.children.at(1);
        std::vector<std::shared_ptr<TypedProcedure>> child_result =
            visit_fns(fns, symbol_table, static_data);
        result.insert(result.end(), child_result.begin(), child_result.end());
    } else {
        std::cerr << "Invalid production found while processing fns."
                  << std::endl;
        exit(1);
    }

    return result;
}

std::shared_ptr<TypedProcedure> visit_fn(
    ASTNode root,
    SymbolTable& symbol_table,
    std::vector<std::shared_ptr<Code>>& static_data
) {
    assert(std::get<NonTerminal>(root.state) == NonTerminal::fn);
    std::shared_ptr<TypedProcedure> result;

    std::vector<State> prod = root.get_production();
    if (prod
        == std::vector<State> {
            NonTerminal::fn,
            Terminal::FN,
            Terminal::ID,
            Terminal::LPAREN,
            NonTerminal::optparams,
            Terminal::RPAREN,
            Terminal::ARROW,
            NonTerminal::type,
            NonTerminal::stmtblock,
        }) {
        result = std::make_shared<TypedProcedure>();

        // extract function name
        ASTNode id = root.children.at(1);
        std::string name = id.lexeme;

        // add identifier to symbol table
        symbol_table[name] = result;

        // make clone of symbol table to scope params
        SymbolTable symbol_table_params = symbol_table;

        // extract function parameters
        ASTNode optparams = root.children.at(3);
        std::vector<std::shared_ptr<TypedVariable>> typed_params =
            visit_optparams(optparams, symbol_table_params);

        std::vector<std::shared_ptr<Variable>> params;
        std::vector<std::shared_ptr<NLType>> param_types;
        for (auto typed_variable : typed_params) {
            params.push_back(typed_variable->variable);
            param_types.push_back(typed_variable->nl_type);
        }
        result->procedure = std::make_shared<Procedure>(name, params);
        result->param_types = param_types;

        // extract type information
        ASTNode ret_type = root.children.at(6);
        auto nl_type = visit_type(ret_type);
        result->ret_type = nl_type;

        ASTNode stmtblock = root.children.at(7);
        auto code = visit_stmtblock(
            stmtblock,
            result,
            symbol_table_params,
            static_data
        );

        result->procedure->code = code;
    } else if (prod == std::vector<State> {NonTerminal::fn, Terminal::FN, Terminal::ID, Terminal::LPAREN, NonTerminal::optparams, Terminal::RPAREN, NonTerminal::stmtblock}) {
        result = std::make_shared<TypedProcedure>();

        // extract function name
        ASTNode id = root.children.at(1);
        std::string name = id.lexeme;

        // add identifier to symbol table
        symbol_table[name] = result;

        // make clone of symbol table to scope params
        SymbolTable symbol_table_params = symbol_table;

        // extract function parameters
        ASTNode optparams = root.children.at(3);
        std::vector<std::shared_ptr<TypedVariable>> typed_params =
            visit_optparams(optparams, symbol_table_params);

        std::vector<std::shared_ptr<Variable>> params;
        std::vector<std::shared_ptr<NLType>> param_types;
        for (auto typed_variable : typed_params) {
            params.push_back(typed_variable->variable);
            param_types.push_back(typed_variable->nl_type);
        }
        result->procedure = std::make_shared<Procedure>(name, params);
        result->param_types = param_types;

        // return type of none
        result->ret_type = std::make_shared<NLTypeNone>();

        ASTNode stmtblock = root.children.at(5);
        auto code = visit_stmtblock(
            stmtblock,
            result,
            symbol_table_params,
            static_data
        );

        result->procedure->code = code;
    } else {
        std::cerr << "Invalid production found while processing fn."
                  << std::endl;
        exit(1);
    }

    assert(result);
    return result;
}
